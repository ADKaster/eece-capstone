/*! \file damn_i2c_internals.c

    Library private i2c file that calls the architecure specific i2c functions.
*/


/******* Includes *******/
#include "damn_i2c_internals.h"
#include "damn_msgdef.h"
#include "messaging.h"

#include <pthread.h>
#include <mqueue.h>
#include <unistd.h>

#ifdef FREERTOS 
    #include <FreeRTOS.h>
    #include <task.h>
#else
    #error "FreeRTOS needed to do task notification for I2C Slave. Implement other notificaiton schemes (semaphore?)"
#endif


/******* File Defines *******/
#define I2C_STACK_DEPTH (1024) /* 1kiB of stack */
#define I2C_TXQUEUE_DEPTH (0x14)  /* Hold 20 i2c transactions at once */
#define I2C_TXQUEUE_NAME ("I2C TX QUEUE")
#define I2C_TXQUEUE_WIDTH (sizeof(damn_i2c_request_t))
#define I2C_RX_BUFFER_SIZE (0x64) /* Hold 100 words of data at once */

/******* Global Data *******/
mqd_t gI2C_TXQueue;
mqd_t gI2C_RXQueue;
volatile i2c_slave_ringbuf_t gI2C_SlaveRingbuf;
damn_i2c_slave_state_t gI2C_SlaveState = I2C_SLAVE_WAIT_HDR;

static uint32_t slaveRxHoldingBuf[I2C_RX_BUFFER_SIZE];

pthread_t           i2cMasterPthread;
pthread_t           i2cSlavePthread;

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
    gI2C_TXQueue = mq_open(I2C_TXQUEUE_NAME, (O_CREAT), 0, &queueAttr);
    /* mq_open returns (mqd_t)-1 on error */
    if((mqd_t)-1 == gI2C_TXQueue)
    {
        /* mq_open() failed */
        while(1);
    }

    /* initalize ring buffer **MUST BE DONE BEFORE SLAVE INITALIZED** */
    i2c_slave_ringbuf_init(&gI2C_SlaveRingbuf);

    retVal = damn_arch_i2c_init();

    return retVal;
}

void *i2cMasterThread(void *arg0)
{
    damn_i2c_request_t  currRequest;
    damn_i2c_status_t   reqStatus;
    ssize_t             txBytes;

    for(;;)
    {
        /* block waiting for application tx messages */
        /* argument 4, msg_prio completely ignored by TI's implementation for FreeRTOS */
        txBytes = mq_receive(gI2C_TXQueue, (char *)&currRequest, I2C_TXQUEUE_WIDTH, NULL);

        if(txBytes >= 0)
        {
            // TODO Add semaphore/task notification, something to let application know request is done
            /* We have a new request to service */
            if(I2C_TX_SEND_TYPE == currRequest.type)
            {
                /* Application wants to send its own data */
                reqStatus = damn_arch_i2c_tx_send(&currRequest);
            }
            else
            {
                /* Application wants to get someone else's data (P2P) */
                reqStatus = damn_arch_i2c_tx_get(&currRequest);
            } /* Check i2c Request type */
            /* Suppress compiler warning... TODO notify somebody if status isn't success */
            (void)reqStatus;
        }
    }
    return NULL;
} /* i2cMasterThread() */

void *i2cSlaveThread(void *arg0)
{
    /* Use the thread stack to store some variables we'll need often */
    volatile ringbuf_t      *pSlaveRingbuf = (volatile ringbuf_t *)&gI2C_SlaveRingbuf;
    uint32_t                rbufCount = 0;
    damn_pkthdr_t           currHdr;
    uint32_t                num_words_avail = 0;
    uint32_t                rbufRead;

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
                /* Wait for enough words to be available in the ring buffer */
                if(num_words_avail >= DAMN_MSG_HDR_WORDS)
                {
                    /* Double check that there's enough words*/
                    rbufCount = ringbuf_getcount(pSlaveRingbuf);

                    if( rbufCount >= DAMN_MSG_HDR_BYTES)
                    {
                        /* Retrieve the header from the ring buffer */
                        rbufRead = ringbuf_get(pSlaveRingbuf, (uint8_t *)&currHdr, DAMN_MSG_HDR_BYTES);
                        /* Recalculate how many words are available */
                        num_words_avail = (rbufCount - rbufRead)/4;
                        /* Proceed to the next state */
                        gI2C_SlaveState = I2C_PARSE_HDR;
                    } /* DAMN_MSG_HDR_BYTES actually in buffer*/
                } /* DAMN_MSG_HDR_WORDS Allegedly in buffer */
                break;

            case I2C_PARSE_HDR:
                /* Check how many words are available */
                /* verify header --> if fail, tell master on this mcu to send nack packet and flush slave ring buffer */
                if(!damn_msg_verify_header(&currHdr))
                {
                    // SEND NACK and FLUSH RINGBUF
                    gI2C_SlaveState = I2C_SLAVE_SEND_NACK;
                }
                else if (currHdr.msg_size > I2C_RX_BUFFER_SIZE)
                {
                    // SEND "packet too big" nack and FLUSH RINGBUF
                    gI2C_SlaveState = I2C_SLAVE_SEND_NACK;
                }
                else
                {
                    gI2C_SlaveState = I2C_SLAVE_WAIT_MSG;
                }
                break;

            case I2C_SLAVE_WAIT_MSG:
                /* Wait until enough words are available, waking up one word at a time */
                if(num_words_avail < ((currHdr.msg_size)/4))
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
                    if(rbufCount >= currHdr.msg_size)
                    {
                        gI2C_SlaveState = I2C_SLAVE_WAIT_MSG;
                    }
                }
                break;

            case I2C_PROCESS_MSG:
                    /* read whole packet */
                    rbufRead = ringbuf_get(pSlaveRingbuf, (uint8_t *)slaveRxHoldingBuf, currHdr.msg_size);

                    num_words_avail = ringbuf_getcount(pSlaveRingbuf);
                    //if header.id is one that application publishes/isconnfigured to respond to
                    //      gI2C_SlaveState = I2C_SLAVE_SEND_RESP;
                    //  else if header.type is a broadcast that application subscribes to
                    //      put packet in application message queue
                    //  else 
                    //      ignore packet
                    gI2C_SlaveState = I2C_SLAVE_WAIT_HDR;
                break;

            case I2C_SLAVE_SEND_RESP:
                    // grab response from application-defined queue (using standard table/types)
                    //      i.e. gAppMsgTable[header.id].appQueue
                    //  send error if no message available
                    //  block on slave transmit otherwise
                    gI2C_SlaveState = I2C_SLAVE_WAIT_HDR;
                break;

            case I2C_SLAVE_SEND_NACK:
                    // create nack packet
                    //      Fill with global error values, header info
                    //       i.e. gI2CSlave_NackVal
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

