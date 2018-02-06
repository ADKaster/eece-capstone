/*! \file dmcf_i2c_internals.c

    Library private i2c file that calls the architecture specific i2c functions.
*/


/******* Includes *******/
#include "dmcf_i2c_internals.h"
#include "dmcf_msgdef.h"
#include "dmcf_pubsub.h"
#include "messaging.h"
#include "dmcf_debug.h"

#include <pthread.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#ifdef FREERTOS 
    #include <FreeRTOS.h>
    #include <task.h>
#else
    #error "FreeRTOS needed to do task notification for I2C Slave. Implement other notificaiton schemes (semaphore?)"
#endif


/******* File Defines *******/
#define I2C_STACK_DEPTH (2048) /* 1kiB of stack */
#define I2C_RX_BUFFER_SIZE (0x100) /* Hold 256  words of data at once */
#define I2C_PUB_BUFFER_SIZE (0x80) /* Hold 128 words of data at once */
#define I2C_TXQUEUE_NAME ("I2C TX QUEUE")
#define I2C_TXQUEUE_DEPTH (0x14)  /* Hold 20 i2c transactions at once */
#define I2C_TXQUEUE_WIDTH (sizeof(dmcf_master_action_t))

/******* Global Data *******/
mqd_t gI2C_MasterActionQueue;
dmcf_i2c_slave_state_t gI2C_SlaveState = I2C_SLAVE_READ;

static uint32_t slaveRxHoldingBuf[I2C_RX_BUFFER_SIZE];
static uint32_t slavePubHoldingBuf[I2C_PUB_BUFFER_SIZE];

pthread_t           i2cMasterPthread;
pthread_t           i2cSlavePthread;

#ifdef FREERTOS
TaskHandle_t  slaveNotificationHandle = NULL;
#endif

static bool Slave_Hdr_Parse(dmcf_pkthdr_t *hdr, uint32_t *buf);
static void Slave_Reverse_Hdr(dmcf_pkthdr_t *dest, dmcf_pkthdr_t *src);
static dmcf_i2c_slave_state_t Slave_Process_Bytes(void);
static void dmcf_i2c_slave_send_resp(void);
static void dmcf_i2c_slave_nack(dmcf_nack_t nack_byte);


/* Initialize I2C Thread and message queue  */
dmcf_i2c_status_t dmcf_i2c_init(void)
{
    dmcf_i2c_status_t   retVal = I2C_FAIL;
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

    /* Setup Transmit queue, ie. application wants to initiate transmission to someone */
    queueAttr.mq_flags = 0; /* flags are set by mq_open based on second argument */
    queueAttr.mq_maxmsg = I2C_TXQUEUE_DEPTH;
    queueAttr.mq_msgsize = I2C_TXQUEUE_WIDTH;
    queueAttr.mq_curmsgs = 0;

    /* RTOS implementation only cares about O_CREAT, O_EXCL and O_NONBLOCK */
    /* RTOS implementation discards third argument, mode. */
    gI2C_MasterActionQueue = mq_open(I2C_TXQUEUE_NAME, (O_CREAT), 0, &queueAttr);
    /* mq_open returns (mqd_t)-1 on error */
    if((mqd_t)-1 == gI2C_MasterActionQueue)
    {
        /* mq_open() failed */
        while(1);
    }

    retVal = dmcf_arch_i2c_init();

    return retVal;
}

void *i2cMasterThread(void *arg0)
{
    dmcf_master_action_t    currTrans;
    dmcf_master_action_t    backupTrans;
    dmcf_i2c_status_t   reqStatus;
    ssize_t             txBytes;
    static uint32_t masterActionRxCount = 0;

    for(;;)
    {
        /* block waiting for application tx messages */
        /* argument 4, msg_prio completely ignored by TI's implementation for FreeRTOS */
        txBytes = mq_receive(gI2C_MasterActionQueue, (char *)&currTrans, I2C_TXQUEUE_WIDTH, NULL);

        dmcf_debugprintf("Master Action Queue %d: %d bytes", masterActionRxCount, txBytes);

        masterActionRxCount++;

        if(txBytes >= 0)
        {
            memcpy((void *)&backupTrans, (void *)&currTrans, sizeof(dmcf_master_action_t));
            /* We have a new request to service */
            reqStatus = dmcf_arch_i2c_transfer(&(currTrans.transaction));
            /* Try again to send if we failed. If we fail twice, return an error to the application */
            if(I2C_FAIL == reqStatus)
            {
                reqStatus = dmcf_arch_i2c_transfer(&(backupTrans.transaction));
            }

            DMCF_DISABLE_INTS();
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
            DMCF_ENABLE_INTS();
        }
    }
    return NULL;
} /* i2cMasterThread() */

void *i2cSlaveThread(void *arg0)
{
#ifdef FREERTOS
    slaveNotificationHandle = xTaskGetCurrentTaskHandle();
#endif

    for(;;)
    {
        switch(gI2C_SlaveState)
        {
        case I2C_SLAVE_READ:
            /* initiate read request */
            dmcf_arch_i2c_slave_read(slaveRxHoldingBuf, I2C_RX_BUFFER_SIZE);

            /* Pend on transition to ERROR or IDLE */
#ifdef FREERTOS
            ulTaskNotifyTake( pdTRUE, /* Clear the notification value before exiting. */
                              500 / portTICK_PERIOD_MS ); /* Block for 500ms */
#endif

            /* Handle bytes coming in */
            gI2C_SlaveState = Slave_Process_Bytes();
            break;
        case I2C_SLAVE_WRITE:
            /* wait for slave write to complete */
#ifdef FREERTOS
            ulTaskNotifyTake( pdTRUE, /* Clear the notification value before exiting. */
                              500 / portTICK_PERIOD_MS ); /* Block for 500ms */
#endif
            break;
        } /* switch i2c slave state */
    } /* Infinite task loop */

    return NULL;
} /* i2cSlaveThread() */


static dmcf_i2c_slave_state_t Slave_Process_Bytes(void)
{
    dmcf_pkthdr_t          *currHdr = (dmcf_pkthdr_t *)slaveRxHoldingBuf; /* Need header to be able to be sent to pubsub layer */
    dmcf_i2c_slave_state_t  nextState = I2C_SLAVE_READ;
    bool                    messageSent = false;

    if(slaveRxHoldingBuf[0] == DMCF_HDR_SYNCWORD)
    {
        if(dmcf_msg_verify_header(currHdr) && (currHdr->msg_size <= I2C_RX_BUFFER_SIZE))
        {
            /* Only verify the checksum of p2p messages, where the master is expecting a reply. */
            if( (TX_TYPE_P2P == gTheMessageDefinitions[currHdr->id].message_type) )
            {
                if( dmcf_msg_verify_msg(&(slaveRxHoldingBuf[DMCF_MSG_HDR_WORDS]), (currHdr->msg_size >> 2)) )
                {
                    messageSent = Slave_Hdr_Parse(currHdr, slaveRxHoldingBuf);
                }
                else
                {
                    dmcf_i2c_slave_nack(NACK_BAD_CHECKSUM);
                    messageSent = true;
                } /* Bad checksum */
            } /* TX_TYPE_P2P */
            else
            {
                messageSent = Slave_Hdr_Parse(currHdr, slaveRxHoldingBuf);
            } /* TX_TYPE_BCAST */
        } /* header valid, message fits in buffer */
    } /* Sync word */
    if(messageSent){
        nextState = I2C_SLAVE_WRITE;
    }

    return nextState;
}

static void dmcf_i2c_slave_nack(dmcf_nack_t nack_byte)
{
    dmcf_pkthdr_t           *pubHdr = (dmcf_pkthdr_t *)slavePubHoldingBuf;
    dmcf_pkthdr_t           *currHdr = (dmcf_pkthdr_t *)slaveRxHoldingBuf; /* Need header to be able to be sent to pubsub layer */

    Slave_Reverse_Hdr(pubHdr, currHdr); /* Copy header from rx to tx */

    currHdr->ack = nack_byte;

    dmcf_arch_i2c_slave_send((void *)slavePubHoldingBuf, DMCF_MSG_HDR_BYTES);

}

static void dmcf_i2c_slave_send_resp(void)
{
    dmcf_pkthdr_t           *pubHdr = (dmcf_pkthdr_t *)slavePubHoldingBuf;
    dmcf_pkthdr_t           *currHdr = (dmcf_pkthdr_t *)slaveRxHoldingBuf; /* Need header to be able to be sent to pubsub layer */

    /* create response packet using slavePubHoldingBuf, and message info data. */
    Slave_Reverse_Hdr(pubHdr, currHdr); /* Copy header from rx to tx */
    /* Send the completed message */
    dmcf_arch_i2c_slave_send((void *)slavePubHoldingBuf, DMCF_MSG_HDR_BYTES + pubHdr->msg_size);

}

/* Parses header and sends response, if necessary */
static bool Slave_Hdr_Parse(dmcf_pkthdr_t *hdr, uint32_t *buf)
{
    dmcf_msg_enum_t             id = hdr->id;
    ssize_t                     bytes_rx = -1;
    size_t                      bytes_to_send;
    mqd_t                       queue;
    uint32_t                    queue_width;
    bool                        messageSent = false;

    if(APPLICATION_WHOAMI == hdr->dest)
    {
        if((STATUS_PUBLISHING == gThePublications[id].status))
        {
            queue = gThePublications[id].queue;
            queue_width = gThePublications[id].q_width;
            if((queue != (mqd_t)-1) && (queue != NULL))
            {
                bytes_rx = mq_receive(queue, (char *)&(slavePubHoldingBuf[DMCF_MSG_HDR_WORDS]), queue_width, 0);
            }
            
            if(-1 != bytes_rx)
            {
                dmcf_i2c_slave_send_resp();
            }
            else
            {
                dmcf_i2c_slave_nack(NACK_NO_MSG);
            }
            messageSent = true;
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
             *  DMCF_MSG_HDR_BYTES|  hdr.message_len
             *  -------------------------------------------
             *  | HDR |hdrchksum  |    PAYLOAD    | chksum|
             *  -------------------------------------------
             *
             */
            bytes_to_send = DMCF_MSG_HDR_BYTES + hdr->msg_size;
            /* Ignore return value. msg_prio is ignored by function. */
            (void)mq_send(queue, (char *)buf, bytes_to_send, 0);
        }
    }
    else
    {
        dmcf_i2c_slave_nack(NACK_INVLD_DEST);
        messageSent = true;
    }

    return messageSent;
}

static void Slave_Reverse_Hdr(dmcf_pkthdr_t *dest, dmcf_pkthdr_t *src)
{
    dmcf_node_t    tempDest;

    memcpy((void *)dest, (void *)src, DMCF_MSG_HDR_BYTES);
    tempDest = dest->dest;
    dest->dest = dest->src;
    dest->src = tempDest;
    dest->hdr_chksum = 0; /* recalculate checksum */
    dest->hdr_chksum = dmcf_calculate_checksum((uint32_t *)dest, DMCF_MSG_HDR_WORDS);
}
