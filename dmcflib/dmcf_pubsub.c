/*
 * dmcf_pubsub.c
 *
 *  Created on: Aug 26, 2017
 *      Author: Andrew Kaster
 */

#include "dmcf_msgdef.h"
#include "dmcf_pubsub.h"
#include "dmcf_i2c_internals.h"
#include "messaging.h"
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


void *p2p_sub_scheduler_task(void *arg0);

/******* File Defines *******/
#define P2P_SCHED_STACK_SIZE (1024) /* 1kiB of stack */
#define P2P_BUFSIZE (0x100) /* 256 words per message */
#define SUB_BUFSIZE (0x100) /* Hold 256 words */
#define PUB_BUFSIZE SUB_BUFSIZE

/******* Globals *****/
pthread_t       p2pSchedPthread;
dmcf_subinfo_t gTheSubscriptions[NUM_MSG_DEFINITONS];
dmcf_pubinfo_t gThePublications[NUM_MSG_DEFINITONS];

static dmcf_p2p_subinfo_t p2p_sublist[NUM_MSG_DEFINITONS + 1];
static uint32_t dmcf_p2p_sub_rxbuf[NUM_MSG_DEFINITONS][P2P_BUFSIZE];
static dmcf_pkthdr_t p2p_hdrs[NUM_MSG_DEFINITONS];
static uint32_t sub_holding_buf[SUB_BUFSIZE];
static uint32_t pub_holding_buf[NUM_MSG_DEFINITONS][PUB_BUFSIZE];
static volatile bool throwaway_bcast_bool;

static int32_t check_frequency(dmcf_pubsub_freq_t freq, dmcf_tx_type_t type);

void dmcf_init(void)
{
    pthread_attr_t      threadAttr;
    struct sched_param  priorityParams;
    int32_t             detachState;
    int32_t             retc;
    uint32_t            i;
    dmcf_msgdef_t       *msgdefptr;

    if(I2C_FAIL == dmcf_i2c_init())
    {
        /* dmcf_i2c_init failed */
        while(1);
    }

    /* Set priority and stack size attributes */
    pthread_attr_init(&threadAttr);
    priorityParams.sched_priority = 1;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&threadAttr, detachState);
    if (retc != 0)
    {
        /* pthread_attr_setdetachstate() failed */
        while(1);
    }

    pthread_attr_setschedparam(&threadAttr, &priorityParams);

    retc = pthread_attr_setstacksize(&threadAttr, P2P_SCHED_STACK_SIZE);
    if(retc != 0)
    {
        /* pthread_attr_setstacksize() failed */
        while(1);
    }

    retc = pthread_create(&p2pSchedPthread, &threadAttr, p2p_sub_scheduler_task, NULL);

    if(retc != 0)
    {
        /* pthread_create() failed */
        while(1);
    }

    for(i = 0; i < NUM_MSG_DEFINITONS; i++)
    {
        msgdefptr = &(gTheMessageDefinitions[i]);

        gThePublications[i].pmsg_def = msgdefptr;

        gTheSubscriptions[i].pmsg_def = msgdefptr;

    }

}


static int32_t check_frequency(dmcf_pubsub_freq_t freq, dmcf_tx_type_t type)
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
dmcf_pub_status_t dmcf_publish_configure(dmcf_msg_enum_t id,
                                         dmcf_pubsub_freq_t frequency,
                                         uint32_t queue_depth)
{
    dmcf_pub_status_t status = PUB_FAIL;
    uint32_t period_ms = -1;
    struct mq_attr  queueAttr;
    char q_name[10];

    if(id < NUM_MSG_DEFINITONS)
    {
        period_ms = check_frequency(frequency, gTheMessageDefinitions[id].message_type);

        if(-1 != period_ms && (queue_depth > 0) && (queue_depth < MAX_QUEUE_DEPTH))
        {
            gThePublications[id].period_ms = period_ms;
            /* gThePublications[id].pmsg_def set during initialization */
            gThePublications[id].q_depth = queue_depth;
            gThePublications[id].q_width = gThePublications[id].pmsg_def->message_length;
            gThePublications[id].status = STATUS_PUBLISHING;

            /* NOTE: Broadcast messages use master action queue to transmit, not individual queues */
            if(TX_TYPE_P2P == gTheMessageDefinitions[id].message_type)
            {
                /* Setup Transmit queue, ie. application wants to initiate transmimssion to someone */
                queueAttr.mq_flags = 0; /* flags are set by mq_open based on second argument */
                queueAttr.mq_maxmsg = gThePublications[id].q_depth;
                queueAttr.mq_msgsize = gThePublications[id].q_width;
                queueAttr.mq_curmsgs = 0;
                /* RTOS implementaiton only cares about O_CREAT, O_EXCL and O_NONBLOCK */
                /* RTOS implemenation discards third argument, mode. */
                sprintf(q_name, "PUB_Q_%d", id);
                gThePublications[id].queue = mq_open(q_name, (O_CREAT | O_NONBLOCK), 0, &queueAttr);
                /* mq_open returns (mqd_t)-1 on error */
                if((mqd_t)-1 == gThePublications[id].queue)
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
dmcf_sub_status_t dmcf_subscribe_configure(dmcf_msg_enum_t id,
                                           dmcf_pubsub_freq_t frequency,
                                           uint32_t queue_depth)
{
    dmcf_sub_status_t status = SUB_FAIL;
    int32_t period_ms = -1;
    struct mq_attr  queueAttr;
    char q_name[10];

    if(id < NUM_MSG_DEFINITONS)
    {
        period_ms = check_frequency(frequency, gTheMessageDefinitions[id].message_type);

        if(-1 != period_ms && (queue_depth > 0) && (queue_depth < MAX_QUEUE_DEPTH))
         {

            gTheSubscriptions[id].period_ms = period_ms;
            /* gThePublications[id].pmsg_def set during initialization */
            gTheSubscriptions[id].q_depth = queue_depth;
            gTheSubscriptions[id].q_width = DMCF_MSG_HDR_BYTES + gTheSubscriptions[id].pmsg_def->message_length;
            gTheSubscriptions[id].status = STATUS_SUBSCRIBED;
            /* Setup Transmit queue, ie. application wants to initiate transmimssion to someone */
            queueAttr.mq_flags = 0; /* flags are set by mq_open based on second argument */
            queueAttr.mq_maxmsg = gTheSubscriptions[id].q_depth;
            queueAttr.mq_msgsize = gTheSubscriptions[id].q_width;
            queueAttr.mq_curmsgs = 0;

            /* RTOS implementaiton only cares about O_CREAT, O_EXCL and O_NONBLOCK */
            /* RTOS implemenation discards third argument, mode. */
            sprintf(q_name, "SUB_Q_%d", id);
            gTheSubscriptions[id].queue = mq_open(q_name, (O_CREAT | O_NONBLOCK), 0, &queueAttr);
            /* mq_open returns (mqd_t)-1 on error */
            if((mqd_t)-1 == gTheSubscriptions[id].queue)
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
dmcf_pub_status_t dmcf_pub_put(dmcf_msg_enum_t id,
                               void *send_buff)
{
    /* Assume P2P for locals, broadcast will set them if needed */
    dmcf_pub_status_t status = PUB_FAIL;
    dmcf_msgdef_t *pmsgdef = gThePublications[id].pmsg_def;
    dmcf_tx_type_t type = pmsgdef->message_type;
    char *queue_payload = (char *)send_buff;
    uint32_t queuemsg_size = pmsgdef->message_length;
    size_t q_ret;

    /* used for broadcast */
    dmcf_pkthdr_t *hdr;
    uint32_t *msgloc = pub_holding_buf[id];
    dmcf_master_action_t action;
    dmcf_i2c_trans_t *trans = &(action.transaction);

    mqd_t queue = (TX_TYPE_P2P == type) ? gThePublications[id].queue : gI2C_MasterActionQueue;

    if((queue == (mqd_t)-1) || (queue == NULL) )
    {
        status = PUB_INVALID;
    }
    else
    {
        if(TX_TYPE_BROADCAST == type)
        {
            memset((void *)msgloc, 0, DMCF_MSG_HDR_BYTES + pmsgdef->message_length);
            hdr = (dmcf_pkthdr_t *)&(pub_holding_buf[id]);
            dmcf_msg_create_header(hdr, APPLICATION_WHOAMI, pmsgdef->message_dest, id, pmsgdef->message_length);
            msgloc = &(pub_holding_buf[id][DMCF_MSG_HDR_WORDS]);

            memcpy((void *)msgloc, send_buff, pmsgdef->message_length - sizeof(uint32_t));

            /* message_length is in bytes, convert to words */
            msgloc[(pmsgdef->message_length / sizeof(uint32_t)) - 1] = dmcf_calculate_checksum(msgloc, pmsgdef->message_length / sizeof(uint32_t));

            trans->arg = NULL;
            trans->readBuf = NULL;
            trans->readCount = 0;
            trans->writeBuf = pub_holding_buf[id];
            trans->writeCount  = DMCF_MSG_HDR_BYTES + pmsgdef->message_length;
            trans->slaveAddress = BROADCAST_ADDRESS;

            action.completed = &throwaway_bcast_bool;
            action.success = &throwaway_bcast_bool;

            queue_payload = (char *)&action;
            queuemsg_size = sizeof(dmcf_master_action_t);
        } /* broadcast :) */

        q_ret = mq_send(queue, queue_payload, queuemsg_size, 0);

        if(q_ret == -1)
        {
            dmcf_debugprintf("Pub Put failed: %d", errno);
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
dmcf_sub_status_t dmcf_sub_get(dmcf_msg_enum_t id,
                               void *recvbuff,
                               dmcf_nack_t *ack_chk)
{
    dmcf_sub_status_t status = SUB_FAIL;
    ssize_t q_ret = 0;
    dmcf_pkthdr_t hdr;
    bool msg_status = false;
    uint32_t bytes_to_copy;

    /********************FORMAT*****************
     *
     *  DMCF_MSG_HDR_BYTES|  hdr.message_len
     *  -------------------------------------------
     *  | HDR |hdrchksum  |    PAYLOAD    | chksum|
     *  -------------------------------------------
     *
     */

    if((gTheSubscriptions[id].queue == (mqd_t)-1) || (gTheSubscriptions[id].queue == NULL))
    {
        status = SUB_INVALID;
    }
    else
    {
        q_ret = mq_receive(gTheSubscriptions[id].queue,
                           (char *)sub_holding_buf,
                           gTheSubscriptions[id].q_width, 0);
        if(q_ret == -1)
        {
            //dmcf_debugprintf("mq_receive in sub_get failed: %d", errno);
            status = SUB_NONE;
        }
        else
        {
            /* verify header */
            memcpy((void *)&hdr, (void *)sub_holding_buf, DMCF_MSG_HDR_BYTES);

            if(dmcf_msg_verify_header(&hdr))
            {
                /* verify message */
                msg_status = dmcf_msg_verify_msg(&(sub_holding_buf[DMCF_MSG_HDR_WORDS]), (hdr.msg_size >> 2));
                msg_status &= ((hdr.dest == APPLICATION_WHOAMI) || (hdr.dest = BROADCAST));

                if(msg_status)
                {
                    /* don't send checksum!!! msg_size includes checksum*/
                    bytes_to_copy = (hdr.msg_size) - 4;
                    memcpy(recvbuff, (void *)&(sub_holding_buf[DMCF_MSG_HDR_WORDS]), bytes_to_copy);
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
void p2p_sub_sched_init(void)
{
    uint32_t        sublist_idx = 0;
    dmcf_msgdef_t  *item;
    uint32_t        i;

    for(i = 0; i < NUM_MSG_DEFINITONS; i++)
    {
        item = gTheSubscriptions[i].pmsg_def;
        if((TX_TYPE_P2P == item->message_type) &&
           (STATUS_SUBSCRIBED == gTheSubscriptions[i].status))
        {
            p2p_sublist[i].psub_info = &(gTheSubscriptions[i]);
            p2p_sublist[i].last_sent = 0;
            p2p_sublist[i].completed = true;
            p2p_sublist[i].in_progress = false;

            sublist_idx++;
        }
    }
    p2p_sublist[sublist_idx].psub_info = (dmcf_subinfo_ptr)-1;
}


void *p2p_sub_scheduler_task(void *arg0)
{
    uint32_t i;
    struct timespec currtime;
    dmcf_subinfo_ptr subptr;
    uint32_t time_ms = 0;
    dmcf_master_action_t currAction;
    dmcf_i2c_trans_t    *currTrans = &(currAction.transaction);
    dmcf_msg_enum_t     id;
    dmcf_node_t         dest;
    uint32_t            len;

    sleep(5);

    /* MUST be done after application has a chance to subscribe.
     * TODO ADD requirement to finish ALL initialization within 5 seconds.
     */
    p2p_sub_sched_init();

    for(;;)
    {
        for(i = 0; p2p_sublist[i].psub_info != (dmcf_subinfo_ptr)-1; i++)
        {
            (void)clock_gettime(CLOCK_REALTIME, &currtime);
            time_ms = (currtime.tv_sec * 1000) + (currtime.tv_nsec/1000000);

            subptr = p2p_sublist[i].psub_info;

            DMCF_DISABLE_INTS();
            if(true == p2p_sublist[i].completed)
            {
                if(true == p2p_sublist[i].in_progress && true == p2p_sublist[i].success)
                {
                    /* Send the message to be available for sub_get */
                    mq_send(subptr->queue,
                            (char *)&(dmcf_p2p_sub_rxbuf[subptr->pmsg_def->message_number]),
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
                DMCF_ENABLE_INTS();
            }
            else
            {

                if((time_ms - p2p_sublist[i].last_sent) >= p2p_sublist[i].psub_info->period_ms)
                {
                    DMCF_ENABLE_INTS();
                    p2p_sublist[i].in_progress = true;

                    dest = subptr->pmsg_def->message_dest;
                    id = subptr->pmsg_def->message_number;
                    len = DMCF_MSG_HDR_BYTES + subptr->pmsg_def->message_length;

                    /* Create transaction */
                    dmcf_msg_create_header(&(p2p_hdrs[id]),
                                           APPLICATION_WHOAMI,
                                           dest,
                                           id,
                                           len);

                    currTrans->arg = NULL;

                    currTrans->writeBuf = (void *)&(p2p_hdrs[id]);
                    currTrans->writeCount = sizeof(dmcf_pkthdr_t);

                    currTrans->readBuf = (void *)&(dmcf_p2p_sub_rxbuf[id]);
                    currTrans->readCount = len;

#ifdef FREERTOS
                    configASSERT((len % 4) == 0);
#endif
                    currTrans->slaveAddress = gTheSlaveAddresses[dest];

                    currAction.completed = &(p2p_sublist[i].completed);
                    currAction.success = &(p2p_sublist[i].success);

                    /* If master action queue is full, need to increase depth or decrease rate of messages. timing analysis needed..... */
                    (void)mq_send(gI2C_MasterActionQueue, (char *)&(currAction), sizeof(dmcf_master_action_t), 0);

                    p2p_sublist[i].last_sent = time_ms;
                } /* time to send */
                else
                {
                    DMCF_ENABLE_INTS();
                } /* not time to send */
            } /* check for master telling us it's done */
        } /* end loop over pubs to request */

        usleep(10000);
    } /* End infinite task loop*/

    return NULL;
}

