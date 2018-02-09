/*! \file dmcf_i2c_internals.c

    Library private i2c file that calls the architecture specific i2c functions.
*/


/******* Includes *******/
#include "dmcf.hpp"
#include "dmcf_i2c_internals.hpp"
#include "dmcf_msgdef.hpp"
#include "dmcf_pubsub.hpp"
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

/* MUST be visible from internal C code. Do not touch from application!!!! */
kernel_task_handle_t slaveNotificationHandle = NULL;

namespace DMCF
{

void *dmcf::i2cMasterThread(void *arg0)
{
    master_action_t    currTrans;
    master_action_t    backupTrans;
    i2c_status_t       reqStatus;
    ssize_t            txBytes;
    static uint32_t masterActionRxCount = 0;

    for(;;)
    {
        /* block waiting for application tx messages */
        /* argument 4, msg_prio completely ignored by TI's implementation for FreeRTOS */
        txBytes = mq_receive(I2C_MasterActionQueue, (char *)&currTrans, I2C_TXQUEUE_WIDTH, NULL);

        //dmcf_debugprintf("Master Action Queue %d: %d bytes", masterActionRxCount, txBytes);

        masterActionRxCount++;

        if(txBytes >= 0)
        {
            memcpy((void *)&backupTrans, (void *)&currTrans, sizeof(master_action_t));
            /* We have a new request to service */
            reqStatus = arch_i2c_master_transfer(&(currTrans.transaction));
            /* Try again to send if we failed. If we fail twice, return an error to the application */
            if(I2C_FAIL == reqStatus)
            {
                reqStatus = arch_i2c_master_transfer(&(backupTrans.transaction));
            }

            /* TODO same mutex from p2p sub task */
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
            /* TODO unlock mutex from p2p sub task */
        }
    }
    return NULL;
} /* i2cMasterThread() */

void *dmcf::i2cSlaveThread(void *arg0)
{
    slaveNotificationHandle = getSlaveTaskHandle();

    for(;;)
    {
        switch(I2C_SlaveState)
        {
        case I2C_SLAVE_READ:
            /* initiate read request */
            arch_i2c_slave_read(slaveRxHoldingBuf, I2C_RX_BUFFER_SIZE);

            /* Pend on transition to ERROR or IDLE */
#ifdef FREERTOS
            ulTaskNotifyTake( pdTRUE, /* Clear the notification value before exiting. */
                              500 / portTICK_PERIOD_MS ); /* Block for 500ms */
#endif

            /* Handle bytes coming in */
            I2C_SlaveState = Slave_Process_Bytes();
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


i2c_slave_state_t dmcf::Slave_Process_Bytes(void)
{
    pkthdr_t          *currHdr = (pkthdr_t *)slaveRxHoldingBuf; /* Need header to be able to be sent to pubsub layer */
    i2c_slave_state_t  nextState = I2C_SLAVE_READ;
    bool               messageSent = false;

    if(slaveRxHoldingBuf[0] == HDR_SYNCWORD)
    {
        if(msg_verify_header(currHdr) && (currHdr->msg_size <= I2C_RX_BUFFER_SIZE))
        {
            /* Only verify the checksum of p2p messages, where the master is expecting a reply. */
            if( (TX_TYPE_P2P == gTheMessageDefinitions[currHdr->id].message_type) )
            {
                if( msg_verify_msg(&(slaveRxHoldingBuf[MSG_HDR_WORDS]), (currHdr->msg_size >> 2)) )
                {
                    messageSent = Slave_Hdr_Parse(currHdr, slaveRxHoldingBuf);
                }
                else
                {
                    i2c_slave_nack(NACK_BAD_CHECKSUM);
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

void dmcf::i2c_slave_nack(nack_t nack_byte)
{
    pkthdr_t      *pubHdr = (pkthdr_t *)slavePubHoldingBuf;
    pkthdr_t      *currHdr = (pkthdr_t *)slaveRxHoldingBuf; /* Need header to be able to be sent to pubsub layer */

    Slave_Reverse_Hdr(pubHdr, currHdr); /* Copy header from rx to tx */

    currHdr->ack = nack_byte;

    arch_i2c_slave_send((void *)slavePubHoldingBuf, MSG_HDR_BYTES);

}

void dmcf::i2c_slave_send_resp(void)
{
    pkthdr_t           *pubHdr = (pkthdr_t *)slavePubHoldingBuf;
    pkthdr_t           *currHdr = (pkthdr_t *)slaveRxHoldingBuf; /* Need header to be able to be sent to pubsub layer */

    /* create response packet using slavePubHoldingBuf, and message info data. */
    Slave_Reverse_Hdr(pubHdr, currHdr); /* Copy header from rx to tx */
    /* Send the completed message */
    arch_i2c_slave_send((void *)slavePubHoldingBuf, MSG_HDR_BYTES + pubHdr->msg_size);

}

/* Parses header and sends response, if necessary */
bool dmcf::Slave_Hdr_Parse(pkthdr_t *hdr, uint32_t *buf)
{
    msg_enum_t             id = hdr->id;
    ssize_t                bytes_rx = -1;
    size_t                 bytes_to_send;
    mqd_t                  queue;
    uint32_t               queue_width;
    bool                   messageSent = false;

    if(appName == hdr->dest)
    {
        if((STATUS_PUBLISHING == thePublications[id].status))
        {
            queue = thePublications[id].queue;
            queue_width = thePublications[id].q_width;
            if((queue != (mqd_t)-1) && (queue != NULL))
            {
                bytes_rx = mq_receive(queue, (char *)&(slavePubHoldingBuf[MSG_HDR_WORDS]), queue_width, 0);
            }
            
            if(-1 != bytes_rx)
            {
                i2c_slave_send_resp();
            }
            else
            {
                i2c_slave_nack(NACK_NO_MSG);
            }
            messageSent = true;
        }
    }
    else if(BROADCAST == hdr->dest)
    {
        if(STATUS_SUBSCRIBED == theSubscriptions[id].status)
        {
            queue = theSubscriptions[id].queue;
            queue_width = theSubscriptions[id].q_width;
            /* Send HEADER along with PAYLOAD */
            /********************FORMAT*****************
             *
             *  MSG_HDR_BYTES     |  hdr.message_len
             *  -------------------------------------------
             *  | HDR |hdrchksum  |    PAYLOAD    | chksum|
             *  -------------------------------------------
             *
             */
            bytes_to_send = MSG_HDR_BYTES + hdr->msg_size;
            /* Ignore return value. msg_prio is ignored by function. */
            (void)mq_send(queue, (char *)buf, bytes_to_send, 0);
        }
    }
    else
    {
        i2c_slave_nack(NACK_INVLD_DEST);
        messageSent = true;
    }

    return messageSent;
}

void dmcf::Slave_Reverse_Hdr(pkthdr_t *dest, pkthdr_t *src)
{
    node_t    tempDest;

    memcpy((void *)dest, (void *)src, MSG_HDR_BYTES);
    tempDest = dest->dest;
    dest->dest = dest->src;
    dest->src = tempDest;
    dest->hdr_chksum = 0; /* recalculate checksum */
    dest->hdr_chksum = calculate_checksum((uint32_t *)dest, MSG_HDR_WORDS);
}

} /* namespace DMCF */

