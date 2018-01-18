/*! \file damn_i2c_internals.c

    Library private i2c file that calls the architecure specific i2c functions.
*/


/******* Includes *******/
#include "damn_i2c_internals.h"
#include "damn_msgdef.h"
#include "damn_pubsub.h"
#include "messaging.h"

#include <pthread.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>

#ifdef FREERTOS 
    #include <FreeRTOS.h>
    #include <task.h>
#else
    #error "FreeRTOS needed to do task notification for I2C Slave. Implement other notificaiton schemes (semaphore?)"
#endif


/******* File Defines *******/
#define I2C_STACK_DEPTH (1024) /* 1kiB of stack */
#define I2C_RX_BUFFER_SIZE (0x64) /* Hold 100 words of data at once */
#define I2C_PUB_BUFFER_SIZE (0x80) /* Hold 128 words of data at once */
#define I2C_TXQUEUE_NAME ("I2C TX QUEUE")
#define I2C_TXQUEUE_DEPTH (0x14)  /* Hold 20 i2c transactions at once */
#define I2C_TXQUEUE_WIDTH (sizeof(damn_master_action_t))

/******* Global Data *******/
mqd_t gI2C_MasterActionQueue;
volatile i2c_slave_ringbuf_t gI2C_SlaveRingbuf;
damn_i2c_slave_state_t gI2C_SlaveState = I2C_SLAVE_WAIT_HDR;

static uint32_t slaveRxHoldingBuf[I2C_RX_BUFFER_SIZE];
static uint32_t slavePubHoldingBuf[I2C_PUB_BUFFER_SIZE];

pthread_t           i2cMasterPthread;
pthread_t           i2cSlavePthread;

static damn_i2c_slave_state_t Slave_Hdr_Parse(damn_pkthdr_t *hdr, uint32_t *buf, damn_nack_t *nackptr);
static void Slave_Reverse_Hdr(damn_pkthdr_t *dest, damn_pkthdr_t *src);


static void i2c_slave_ringbuf_init(volatile i2c_slave_ringbuf_t *rbuf, uint16_t size)
{
    rbuf->size = size;
}


/* Initalize I2C Thread and message queue  */
damn_i2c_status_t damn_i2c_init(void)
{
    damn_i2c_status_t   retVal = I2C_FAIL;
    struct mq_attr      queueAttr;
    pthread_attr_t      i2cAttr;
    struct sched_param  priorityParams;
    int32_t             detachState;
    int32_t             retc;

    /* Set priority and stack size attributes */
    pthread_attr_init(&i2cAttr);
    priorityParams.sched_priority = 1;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&i2cAttr, detachState);
    if (retc != 0)
    {
        /* pthread_attr_setdetachstate() failed */
        while(1);
    }

    pthread_attr_setschedparam(&i2cAttr, &priorityParams);

    retc = pthread_attr_setstacksize(&i2cAttr, I2C_STACK_DEPTH);
    if(retc != 0)
    {
        /* pthread_attr_setstacksize() failed */
        while(1);
    }

    retc = pthread_create(&i2cMasterPthread, &i2cAttr, i2cMasterThread, NULL);
    retc |= pthread_create(&i2cSlavePthread, &i2cAttr, i2cSlaveThread, NULL);

    if (retc != 0)
    {
        /* pthread_create() failed */
        while(1);
    }

    /* Setup Transmit queue, ie. application wants to initiate transmimssion to someone */
    queueAttr.mq_flags = 0; /* flags are set by mq_open based on second argument */
    queueAttr.mq_maxmsg = I2C_TXQUEUE_DEPTH;
    queueAttr.mq_msgsize = I2C_TXQUEUE_WIDTH;
    queueAttr.mq_curmsgs = 0;

    /* RTOS implementaiton only cares about O_CREAT, O_EXCL and O_NONBLOCK */
    /* RTOS implemenation discards third argument, mode. */
    gI2C_MasterActionQueue = mq_open(I2C_TXQUEUE_NAME, (O_CREAT | O_NONBLOCK), 0, &queueAttr);
    /* mq_open returns (mqd_t)-1 on error */
    if((mqd_t)-1 == gI2C_MasterActionQueue)
    {
        /* mq_open() failed */
        while(1);
    }

    /* initalize ring buffer **MUST BE DONE BEFORE SLAVE INITALIZED** */
    i2c_slave_ringbuf_init(&gI2C_SlaveRingbuf, I2C_SLAVE_RINGBUF_SIZE);

    retVal = damn_arch_i2c_init();

    return retVal;
}

void *i2cMasterThread(void *arg0)
{
    damn_master_action_t    currTrans;
    damn_master_action_t    backupTrans;
    damn_i2c_status_t   reqStatus;
    ssize_t             txBytes;

    for(;;)
    {
        /* block waiting for application tx messages */
        /* argument 4, msg_prio completely ignored by TI's implementation for FreeRTOS */
        txBytes = mq_receive(gI2C_MasterActionQueue, (char *)&currTrans, I2C_TXQUEUE_WIDTH, NULL);

        if(txBytes >= 0)
        {
            memcpy((void *)&backupTrans, (void *)&currTrans, sizeof(damn_master_action_t));
            /* We have a new request to service */
            reqStatus = damn_arch_i2c_transfer(&(currTrans.transaction));
            /* Try again to send if we failed. If we fail twice, return an error to the application */
            if(I2C_FAIL == reqStatus)
            {
                reqStatus = damn_arch_i2c_transfer(&(backupTrans.transaction));
            }

            DAMN_DISABLE_INTS();
            /* Send the result of this to the application. */
            if(I2C_SUCCESS == reqStatus)
            {
                *(currTrans.success) = true;
            }
            else
            {
                *(currTrans.success) = false;
            }
            /* Set last for "safety"? */
            *(currTrans.completed) = true;
            DAMN_ENABLE_INTS();
        }
    }
    return NULL;
} /* i2cMasterThread() */

void *i2cSlaveThread(void *arg0)
{
    /* Use the thread stack to store some variables we'll need often */
    volatile ringbuf_t      *pSlaveRingbuf = (volatile ringbuf_t *)&gI2C_SlaveRingbuf;
    uint32_t                rbufCount = 0;
    damn_pkthdr_t           *currHdr = (damn_pkthdr_t *)slaveRxHoldingBuf; /* Need header to be able to be sent to pubsub layer */
    uint32_t                num_words_avail = 0;
    uint32_t                rbufRead;
    uint32_t                rbufFirstWord;
    damn_nack_t             slaveNack = ACK;
    damn_pkthdr_t           *pubHdr = (damn_pkthdr_t *)slavePubHoldingBuf;

    for(;;)
    {
        switch(gI2C_SlaveState)
        {
            case I2C_SLAVE_WAIT_HDR:
                /* Take data available semaphore, gets released by ISR (callback, to be precise) */
#ifdef FREERTOS
                /* Block indefinitely waiting for ISR to tell us a new word is available */
                num_words_avail += ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
#endif
                /* Wait for enough words to be available in the ring buffer.
                   Keep analyzing the words until we see the start of a valid packet.
                   I2C slave interrupt will manage any expected responses we miss.
                 */
                while (num_words_avail >= DAMN_MSG_HDR_WORDS)
                {
                    rbufFirstWord = ringbuf_peek(pSlaveRingbuf);
                    /* Buffer contains valid header at beginning */
                    if(rbufFirstWord == DAMN_HDR_SYNCWORD)
                    {
                        /* Retrieve the header from the ring buffer */
                        rbufRead = ringbuf_get(pSlaveRingbuf, (uint8_t *)currHdr, DAMN_MSG_HDR_BYTES);
                        /* Recalculate how many words are available */
                        num_words_avail = (rbufCount - rbufRead) >> 2;
                        /* Proceed to the next state */
                        gI2C_SlaveState = I2C_SLAVE_PARSE_HDR;
                    }
                    else
                    {
                        /* Discard first byte, it's not the start of a packet */
                        rbufRead = ringbuf_get(pSlaveRingbuf, (uint8_t *)&rbufFirstWord, 1);
                        /* guaranteed to have more than 1 word in the ring buffer. */
                        num_words_avail -= 1;
                    }
                } /* DAMN_MSG_HDR_WORDS in buffer */
                break;

            case I2C_SLAVE_PARSE_HDR:
                /* Check how many words are available */
                /* verify header --> if fail, tell master on this mcu to send nack packet and flush slave ring buffer */
                if(damn_msg_verify_header(currHdr) && (currHdr->msg_size <= I2C_RX_BUFFER_SIZE))
                {
                    /* HDR is verified and We have space for message */
                    gI2C_SlaveState = I2C_SLAVE_WAIT_MSG;
                }
                else
                {
                    /* Ignore that shit and go to the beginning. 
                     * Ain't nobody got time for packets that are too big or with invalid checksums
                    */
                    gI2C_SlaveState = I2C_SLAVE_WAIT_HDR;
                }
                break;

            case I2C_SLAVE_WAIT_MSG:
                /* Wait until enough words are available, waking up one word at a time */
                if(num_words_avail < ((currHdr->msg_size) >> 2))
                {
#ifdef FREERTOS
                    /* Wait until we've got words enough for the message */
                    num_words_avail += ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
#endif
                }
                else
                {
                    /* doublecheck how many bytes are available in case someone was bad and put */
                    /* non-word aligned number of bytes in their message  */
                    rbufCount = ringbuf_getcount(pSlaveRingbuf);
                    num_words_avail = rbufCount >> 2;
                    if(rbufCount >= currHdr->msg_size)
                    {
                        gI2C_SlaveState = I2C_SLAVE_PROCESS_MSG;
                    }
                }
                break;

            case I2C_SLAVE_PROCESS_MSG:
                /* read whole packet */
                rbufRead = ringbuf_get(pSlaveRingbuf, (uint8_t *)&(slaveRxHoldingBuf[DAMN_MSG_HDR_WORDS]), currHdr->msg_size);

                /* Update number of words available */
                num_words_avail = ringbuf_getcount(pSlaveRingbuf) >> 2;

                /* Only verify the checksum of p2p messages, where the master is expecting a reply. */
                if( (TX_TYPE_P2P == gTheMessageDefinitions[currHdr->id].message_type) &&
                    (!damn_msg_verify_msg(&(slaveRxHoldingBuf[DAMN_MSG_HDR_WORDS]), (currHdr->msg_size >> 2))))
                {
                    slaveNack = NACK_BAD_CHECKSUM;
                    gI2C_SlaveState = I2C_SLAVE_SEND_NACK;
                }
                else
                {
                    gI2C_SlaveState = Slave_Hdr_Parse(currHdr, slaveRxHoldingBuf, &slaveNack);
                }
                break;

            case I2C_SLAVE_SEND_RESP:
                /* create response packet using slavePubHoldingBuf, and message info data. */
                Slave_Reverse_Hdr(pubHdr, currHdr); /* Copy header from rx to tx */
                /* Send the completed message */
                damn_arch_i2c_slave_send((void *)slavePubHoldingBuf, DAMN_MSG_HDR_BYTES + pubHdr->msg_size);
                break;

            case I2C_SLAVE_SEND_NACK:
                Slave_Reverse_Hdr(pubHdr, currHdr); /* Copy header from rx to tx */
                currHdr->ack = slaveNack;
                damn_arch_i2c_slave_send((void *)slavePubHoldingBuf, DAMN_MSG_HDR_BYTES);
                gI2C_SlaveState = I2C_SLAVE_WAIT_HDR;
                break;

            default:
                gI2C_SlaveState = I2C_SLAVE_WAIT_HDR;
                break;

        } /* Switch gI2C_SlaveState */
    } /* Infinite task loop */
    /* If we get here there's some serious problems */
    return NULL; 
} /* i2cSlaveThread() */

/* Determines what state to go to after the message has been read */
static damn_i2c_slave_state_t Slave_Hdr_Parse(damn_pkthdr_t *hdr, uint32_t *buf, damn_nack_t *nackptr)
{
    damn_i2c_slave_state_t newState = I2C_SLAVE_PROCESS_MSG;
    damn_msg_enum_t             id = hdr->id;
    ssize_t                     bytes_rx = -1;
    size_t                      bytes_to_send;
    mqd_t                       queue;
    uint32_t                    queue_width;

    if(APPLICATION_WHOAMI == hdr->dest)
    {
        if((STATUS_PUBLISHING == gThePublications[id].status))
        {
            queue = gThePublications[id].queue;
            queue_width = gThePublications[id].q_width;
            if((queue != (mqd_t)-1) && (queue != NULL))
            {
                bytes_rx = mq_receive(queue, (char *)&(slavePubHoldingBuf[DAMN_MSG_HDR_WORDS]), queue_width, 0);
            }
            
            if(-1 != bytes_rx)
            {
                newState = I2C_SLAVE_SEND_RESP;
            }
            else
            {
                *nackptr = NACK_NO_MSG;
                newState = I2C_SLAVE_SEND_NACK;
            }
        }
        else
        {
            newState = I2C_SLAVE_SEND_NACK;
        }
    }
    else if(BROADCAST == hdr->dest)
    {
        if(STATUS_SUBSCRIBED == gTheSubscriptions[id].status)
        {
            queue = gTheSubscriptions[id].queue;
            queue_width = gTheSubscriptions[id].q_width;
            /* Send HEADER along with PAYLOAD */
            /********************FORMAT*****************
             *
             *  DAMN_MSG_HDR_BYTES|  hdr.message_len
             *  -------------------------------------------
             *  | HDR |hdrchksum  |    PAYLOAD    | chksum|
             *  -------------------------------------------
             *
             */
            bytes_to_send = DAMN_MSG_HDR_BYTES + hdr->msg_size;
            /* Ignore return value. msg_prio is ignored by function. */
            (void)mq_send(queue, (char *)buf, bytes_to_send, 0);
        }

        newState = I2C_SLAVE_WAIT_HDR;
    }
    else
    {
        *nackptr = NACK_INVLD_DEST;
        newState = I2C_SLAVE_SEND_NACK;
    }
    return newState;
}

static void Slave_Reverse_Hdr(damn_pkthdr_t *dest, damn_pkthdr_t *src)
{
    damn_node_t    tempDest;

    memcpy((void *)dest, (void *)src, DAMN_MSG_HDR_BYTES);
    tempDest = dest->dest;
    dest->dest = dest->src;
    dest->src = tempDest;
    dest->hdr_chksum = 0; /* recalculate checksum */
    dest->hdr_chksum = damn_calculate_checksum((uint32_t *)dest, DAMN_MSG_HDR_WORDS);
}
