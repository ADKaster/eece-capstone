/*
 * dmcf_pubsub.c
 *
 *  Created on: Aug 26, 2017
 *      Author: Andrew Kaster
 */

#include <dmcf.cpp>
#include "dmcf_msgdef.hpp"
#include "dmcf_pubsub.hpp"
#include "dmcf_i2c_internals.hpp"
#include "dmcf_debug.h"
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <mqueue.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#ifdef FREERTOS
    #include <FreeRTOS.h>
    #include <task.h>
#endif

namespace DMCF
{

int32_t dmcf::check_frequency(pubsub_freq_t freq, tx_type_t type)
{
    int32_t period_ms = -1;

    if(TX_TYPE_P2P == type)
    {
        switch(freq)
        {
        case FREQ_TEN_HZ:
            period_ms = 100;
            break;
        case FREQ_TWENTY_HZ:
            period_ms = 50;
            break;
        case FREQ_ONE_HUNDRED_HZ:
            period_ms = 10;
            break;
        /* Unlimited not valid for p2p */
        case FREQ_UNLIMITED:
        default:
            break;
        }
    }
    else /* TX_TYPE_BOROADCAST */
    {
        period_ms = 0;
    } /* End check of transmit type */

    return period_ms;
}


// If a subscriber tries to poll at a frequency faster than the publisher is
// publishing, the publisher will notify the subscriber that it's going too fast.
// by way of a NO_MSG NACK response
pub_status_t dmcf::publish_configure(msg_enum_t id,
                                         pubsub_freq_t frequency,
                                         uint32_t queue_depth)
{
    pub_status_t status = PUB_FAIL;
    int32_t period_ms = -1;
    struct mq_attr  queueAttr;
    char q_name[10];

    if(id < NUM_MSG_DEFINITONS)
    {
        period_ms = check_frequency(frequency, gTheMessageDefinitions[id].message_type);

        if(-1 != period_ms && (queue_depth > 0) && (queue_depth < MAX_QUEUE_DEPTH))
        {
            thePublications[id].period_ms = period_ms;
            /* gThePublications[id].pmsg_def set during initialization */
            thePublications[id].q_depth = queue_depth;
            thePublications[id].q_width = thePublications[id].pmsg_def->message_length;
            thePublications[id].status = STATUS_PUBLISHING;

            /* NOTE: Broadcast messages use master action queue to transmit, not individual queues */
            if(TX_TYPE_P2P == gTheMessageDefinitions[id].message_type)
            {
                /* Setup Transmit queue, ie. application wants to initiate transmimssion to someone */
                queueAttr.mq_flags = 0; /* flags are set by mq_open based on second argument */
                queueAttr.mq_maxmsg = thePublications[id].q_depth;
                queueAttr.mq_msgsize = thePublications[id].q_width;
                queueAttr.mq_curmsgs = 0;
                /* RTOS implementaiton only cares about O_CREAT, O_EXCL and O_NONBLOCK */
                /* RTOS implemenation discards third argument, mode. */
                sprintf(q_name, "PUB_Q_%d", id);
                thePublications[id].queue = mq_open(q_name, (O_CREAT | O_NONBLOCK), 0, &queueAttr);
                /* mq_open returns (mqd_t)-1 on error */
                if((mqd_t)-1 == thePublications[id].queue)
                {
                    /* mq_open() failed */
                    while(1);
                }
            } /* check for P2P */
            status = PUB_SUCCESS;
        } /* Valid Queue depth and period */
    } /* Valid ID */

    return status;
}

// When frequency is set to unlimited, the message will be populated in the
// subscriber's receive buffer when published by the publisher.
// This can only happen for broadcast messages. For point to point messages,
// the subscriber must configure a frequency to poll the publisher at.
sub_status_t dmcf::subscribe_configure(msg_enum_t id,
                                           pubsub_freq_t frequency,
                                           uint32_t queue_depth)
{
    sub_status_t status = SUB_FAIL;
    int32_t period_ms = -1;
    struct mq_attr  queueAttr;
    char q_name[10];

    if(id < NUM_MSG_DEFINITONS)
    {
        period_ms = check_frequency(frequency, gTheMessageDefinitions[id].message_type);

        if(-1 != period_ms && (queue_depth > 0) && (queue_depth < MAX_QUEUE_DEPTH))
         {

            theSubscriptions[id].period_ms = period_ms;
            /* gThePublications[id].pmsg_def set during initialization */
            theSubscriptions[id].q_depth = queue_depth;
            theSubscriptions[id].q_width = MSG_HDR_BYTES + theSubscriptions[id].pmsg_def->message_length;
            theSubscriptions[id].status = STATUS_SUBSCRIBED;
            /* Setup Transmit queue, ie. application wants to initiate transmimssion to someone */
            queueAttr.mq_flags = 0; /* flags are set by mq_open based on second argument */
            queueAttr.mq_maxmsg = theSubscriptions[id].q_depth;
            queueAttr.mq_msgsize = theSubscriptions[id].q_width;
            queueAttr.mq_curmsgs = 0;

            /* RTOS implementaiton only cares about O_CREAT, O_EXCL and O_NONBLOCK */
            /* RTOS implemenation discards third argument, mode. */
            sprintf(q_name, "SUB_Q_%d", id);
            theSubscriptions[id].queue = mq_open(q_name, (O_CREAT | O_NONBLOCK), 0, &queueAttr);
            /* mq_open returns (mqd_t)-1 on error */
            if((mqd_t)-1 == theSubscriptions[id].queue)
            {
                /* mq_open() failed */
                while(1);
            }
            status = SUB_SUCCESS;
         } /* Valid Queue depth and period */
    } /* Valid ID*/

    return status;
}

/* send_buff contains the payload ONLY */
pub_status_t dmcf::pub_put(msg_enum_t id,
                               void *send_buff)
{
    /* Assume P2P for locals, broadcast will set them if needed */
    pub_status_t status = PUB_FAIL;
    msgdef_t *pmsgdef = thePublications[id].pmsg_def;
    tx_type_t type = pmsgdef->message_type;
    char *queue_payload = (char *)send_buff;
    uint32_t queuemsg_size = pmsgdef->message_length;
    ssize_t q_ret;

    /* used for broadcast */
    pkthdr_t *hdr;
    uint32_t *msgloc = pub_holding_buf[id];
    master_action_t action;
    i2c_trans_t *trans = &(action.transaction);

    mqd_t queue = (TX_TYPE_P2P == type) ? thePublications[id].queue : I2C_MasterActionQueue;

    if((queue == (mqd_t)-1) || (queue == NULL) )
    {
        status = PUB_INVALID;
    }
    else
    {
        if(TX_TYPE_BROADCAST == type)
        {
            memset((void *)msgloc, 0, MSG_HDR_BYTES + pmsgdef->message_length);
            hdr = (pkthdr_t *)&(pub_holding_buf[id]);
            msg_create_header(hdr, appName, pmsgdef->message_dest, id, pmsgdef->message_length);
            msgloc = &(pub_holding_buf[id][MSG_HDR_WORDS]);

            memcpy((void *)msgloc, send_buff, pmsgdef->message_length - sizeof(uint32_t));

            /* message_length is in bytes, convert to words */
            msgloc[(pmsgdef->message_length / sizeof(uint32_t)) - 1] = calculate_checksum(msgloc, pmsgdef->message_length / sizeof(uint32_t));

            trans->arg = NULL;
            trans->readBuf = NULL;
            trans->readCount = 0;
            trans->writeBuf = pub_holding_buf[id];
            trans->writeCount  = MSG_HDR_BYTES + pmsgdef->message_length;
            trans->slaveAddress = BROADCAST_ADDRESS;

            action.completed = &throwaway_bcast_bool;
            action.success = &throwaway_bcast_bool;

            queue_payload = (char *)&action;
            queuemsg_size = sizeof(master_action_t);
        } /* broadcast :) */

        q_ret = mq_send(queue, queue_payload, queuemsg_size, 0);

        if(q_ret == -1)
        {
            //dmcf_debugprintf("Pub Put failed: %d", errno);
            status = PUB_FULL;
        }
        else
        {
            status = PUB_SUCCESS;
        }

    } /* queue exists */

    return status;
}

/* recvbuff expects the payload ONLY */
sub_status_t dmcf::sub_get(msg_enum_t id,
                           void *recvbuff,
                           nack_t *ack_chk)
{
    sub_status_t status = SUB_FAIL;
    ssize_t q_ret = 0;
    pkthdr_t hdr;
    bool msg_status = false;
    uint32_t bytes_to_copy;

    /********************FORMAT*****************
     *
     *  MSG_HDR_BYTES     |  hdr.message_len
     *  -------------------------------------------
     *  | HDR |hdrchksum  |    PAYLOAD    | chksum|
     *  -------------------------------------------
     *
     */

    if((theSubscriptions[id].queue == (mqd_t)-1) || (theSubscriptions[id].queue == NULL))
    {
        status = SUB_INVALID;
    }
    else
    {
        q_ret = mq_receive(theSubscriptions[id].queue,
                           (char *)sub_holding_buf,
                           theSubscriptions[id].q_width, 0);
        if(q_ret == -1)
        {
            //dmcf_debugprintf("mq_receive in sub_get failed: %d", errno);
            status = SUB_NONE;
        }
        else
        {
            /* verify header */
            memcpy((void *)&hdr, (void *)sub_holding_buf, MSG_HDR_BYTES);

            if(msg_verify_header(&hdr))
            {
                /* verify message */
                msg_status = msg_verify_msg(&(sub_holding_buf[MSG_HDR_WORDS]), (hdr.msg_size >> 2));
                msg_status &= ((hdr.dest == appName) || (hdr.dest = BROADCAST));

                if(msg_status)
                {
                    /* don't send checksum!!! msg_size includes checksum*/
                    bytes_to_copy = (hdr.msg_size) - 4;
                    memcpy(recvbuff, (void *)&(sub_holding_buf[MSG_HDR_WORDS]), bytes_to_copy);
                    status = SUB_SUCCESS;
                }
                else
                {
                    status = SUB_ERR;
                }
                *ack_chk = hdr.ack;
            } /* Good header */
        } /* message rx from queue*/
    } /* queue exists */

    return status;
}

/* Initializes the publish scheduler for p2p messages after all messages have been subscribed to */
void dmcf::p2p_sub_sched_init(void)
{
    uint32_t   sublist_idx = 0;
    msgdef_t  *item;
    uint32_t   i;

    for(i = 0; i < NUM_MSG_DEFINITONS; i++)
    {
        item = theSubscriptions[i].pmsg_def;
        if((TX_TYPE_P2P == item->message_type) &&
           (STATUS_SUBSCRIBED == theSubscriptions[i].status))
        {
            p2p_sublist[i].psub_info = &(theSubscriptions[i]);
            p2p_sublist[i].last_sent = 0;
            p2p_sublist[i].completed = true;
            p2p_sublist[i].in_progress = false;

            sublist_idx++;
        }
    }
    p2p_sublist[sublist_idx].psub_info = (subinfo_ptr)-1;
}


void *dmcf::p2p_sub_scheduler_task(void *arg0)
{
    uint32_t i;
    struct timespec currtime;
    subinfo_ptr subptr;
    uint32_t time_ms = 0;
    master_action_t currAction;
    i2c_trans_t    *currTrans = &(currAction.transaction);
    msg_enum_t     id;
    node_t         dest;
    uint32_t       len;

    sleep(5);

    /* MUST be done after application has a chance to subscribe.
     * TODO ADD requirement to finish ALL initialization within 5 seconds.
     */
    p2p_sub_sched_init();

    for(;;)
    {
        for(i = 0; p2p_sublist[i].psub_info != (subinfo_ptr)-1; i++)
        {
            (void)clock_gettime(CLOCK_REALTIME, &currtime);
            time_ms = (currtime.tv_sec * 1000) + (currtime.tv_nsec/1000000);

            subptr = p2p_sublist[i].psub_info;

            /* TODO Investigate if need disable int or just critical section or what. todo muxtex? */
            if(true == p2p_sublist[i].completed)
            {
                if(true == p2p_sublist[i].in_progress && true == p2p_sublist[i].success)
                {
                    /* Send the message to be available for sub_get */
                    mq_send(subptr->queue,
                            (char *)&(p2p_sub_rxbuf[subptr->pmsg_def->message_number]),
                            subptr->q_width,
                            0);
                    p2p_sublist[i].in_progress = false;
                    p2p_sublist[i].completed = false;
                }
                else
                {
                    /* No success or invalid state */
                    p2p_sublist[i].completed = false;
                }
            }
            else
            {

                if((time_ms - p2p_sublist[i].last_sent) >= p2p_sublist[i].psub_info->period_ms)
                {
                    p2p_sublist[i].in_progress = true;

                    dest = subptr->pmsg_def->message_dest;
                    id = subptr->pmsg_def->message_number;
                    len = MSG_HDR_BYTES + subptr->pmsg_def->message_length;

                    /* Create transaction */
                    msg_create_header(&(p2p_hdrs[id]),
                                        appName,
                                        dest,
                                        id,
                                        len);

                    currTrans->arg = NULL;

                    currTrans->writeBuf = (void *)&(p2p_hdrs[id]);
                    currTrans->writeCount = sizeof(pkthdr_t);

                    currTrans->readBuf = (void *)&(p2p_sub_rxbuf[id]);
                    currTrans->readCount = len;

#ifdef FREERTOS
                    configASSERT((len % 4) == 0);
#endif
                    currTrans->slaveAddress = gTheSlaveAddresses[dest];

                    currAction.completed = &(p2p_sublist[i].completed);
                    currAction.success = &(p2p_sublist[i].success);

                    /* If master action queue is full, need to increase depth or decrease rate of messages. timing analysis needed..... */
                    (void)mq_send(I2C_MasterActionQueue, (char *)&(currAction), sizeof(master_action_t), 0);

                    p2p_sublist[i].last_sent = time_ms;
                } /* time to send */
                else
                {
                    /* TODO Mutex? */
                } /* not time to send */
            } /* check for master telling us it's done */
        } /* end loop over pubs to request */

        usleep(10000);
    } /* End infinite task loop*/

    return NULL;
}

}
