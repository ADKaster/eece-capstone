/*
 * damn_pubsub.c
 *
 *  Created on: Aug 26, 2017
 *      Author: Andrew Kaster
 */

#include "damn_msgdef.h"
#include "damn_pubsub.h"
#include "damn_i2c_internals.h"
#include "messaging.h"
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <mqueue.h>

#ifdef FREERTOS
    #include <FreeRTOS.h>
    #include <task.h>
#endif


void *p2p_sub_scheduler_task(void *arg0);

/******* File Defines *******/
#define P2P_SCHED_STACK_SIZE (1024) /* 1kiB of stack */
#define P2P_BUFSIZE (0x100) /* 256 words per message */

/******* Globals *****/
pthread_t       p2pSchedPthread;
damn_subinfo_t gTheSubscriptions[NUM_MSG_DEFINITONS];
damn_pubinfo_t gThePublications[NUM_MSG_DEFINITONS];



static damn_p2p_subinfo_t p2p_sublist[NUM_MSG_DEFINITONS + 1];
static uint32_t damn_p2p_sub_rxbuf[NUM_MSG_DEFINITONS][P2P_BUFSIZE];
static damn_pkthdr_t p2p_hdrs[NUM_MSG_DEFINITONS];

void damn_init(void)
{
    pthread_attr_t      threadAttr;
    struct sched_param  priorityParams;
    int32_t             detachState;
    int32_t             retc;
    uint32_t            i;
    damn_msgdef_t       *msgdefptr;

    if(I2C_FAIL == damn_i2c_init())
    {
        /* damn_i2c_init failed */
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

void p2p_sub_sched_init(void)
{
    uint32_t        sublist_idx = 0;
    damn_msgdef_t  *item;
    uint32_t        i;
    struct mq_attr  queueAttr;

    for(i = 0; i < NUM_MSG_DEFINITONS; i++)
    {
        item = gTheSubscriptions[i].pmsg_def;
        if((TX_TYPE_P2P == item->message_type) &&
           (STATUS_SUBSCRIBED == gTheSubscriptions[i].status))
        {
            /* TODO move queue creation to time of subscriptions */
            p2p_sublist[i].psub_info = &(gTheSubscriptions[i]);
            p2p_sublist[i].last_sent = 0;
            p2p_sublist[i].completed = true;

            /* Setup Transmit queue, ie. application wants to initiate transmimssion to someone */
            queueAttr.mq_flags = 0; /* flags are set by mq_open based on second argument */
            queueAttr.mq_maxmsg = p2p_sublist[i].psub_info->q_depth;
            queueAttr.mq_msgsize = p2p_sublist[i].psub_info->q_width + sizeof(bool); /* Last word is used to store "completed" */
            queueAttr.mq_curmsgs = 0;
            /* RTOS implementaiton only cares about O_CREAT, O_EXCL and O_NONBLOCK */
            /* RTOS implemenation discards third argument, mode. */
            p2p_sublist[i].psub_info->queue = mq_open(&(*(char *)&(sublist_idx)), (O_CREAT | O_NONBLOCK), 0, &queueAttr);
            /* mq_open returns (mqd_t)-1 on error */
            if((mqd_t)-1 == p2p_sublist[i].psub_info->queue)
            {
                /* mq_open() failed */
                while(1);
            }
            sublist_idx++;
        }
    }
    p2p_sublist[sublist_idx].psub_info = (damn_subinfo_ptr)-1;
}


void *p2p_sub_scheduler_task(void *arg0)
{
    uint32_t i;
    struct timespec currtime;
    damn_subinfo_ptr subptr;
    uint32_t time_ms = 0;
    damn_master_action_t currAction;
    damn_i2c_trans_t    *currTrans = &(currAction.transaction);
    damn_msg_enum_t     id;
    damn_node_t         dest;
    uint32_t            len;

    sleep(5);

    p2p_sub_sched_init();

    for(;;)
    {
        for(i = 0; p2p_sublist[i].psub_info != (damn_subinfo_ptr)-1; i++)
        {
            (void)clock_gettime(CLOCK_REALTIME, &currtime);
            time_ms = (currtime.tv_sec * 1000) + (currtime.tv_nsec/1000000);
            subptr = p2p_sublist[i].psub_info;
            DAMN_DISABLE_INTS();
            if(false == p2p_sublist[i].completed)
            {
                if(false == p2p_sublist[i].in_progress)
                {
                    /* This shouldn't happen :/ */
                    p2p_sublist[i].completed = true;
                }
                else
                {
                    mq_send(subptr->queue,
                            (char *)&(damn_p2p_sub_rxbuf[subptr->pmsg_def->message_number]),
                            subptr->q_width,
                            0);
                    p2p_sublist[i].in_progress = false;
                    DAMN_ENABLE_INTS();
                }
            }
            else
            {

                if((time_ms - p2p_sublist[i].last_sent) >= p2p_sublist[i].psub_info->period_ms)
                {
                    p2p_sublist[i].in_progress = true;
                    DAMN_ENABLE_INTS();

                    dest = subptr->pmsg_def->message_dest;
                    id = subptr->pmsg_def->message_number;
                    len = subptr->pmsg_def->message_length;

                    /* Create transaction */
                    damn_msg_create_header(&(p2p_hdrs[id]),
                                           APPLICATION_WHOAMI,
                                           dest,
                                           id,
                                           len);

                    currTrans->arg = NULL;

                    currTrans->writeBuf = (void *)&(p2p_hdrs[id]);
                    currTrans->writeCount = sizeof(damn_pkthdr_t);

                    currTrans->readBuf = (void *)&(damn_p2p_sub_rxbuf[id]);
                    currTrans->readCount = len;

#ifdef FREERTOS
                    configASSERT((len % 4) == 0);
#endif
                    currTrans->slaveAddress = gTheSlaveAddresses[dest];

                    currAction.completed = &(p2p_sublist[i].completed);
                    /* put success boolean after checksum. Bool should be 1 byte *cross fingers* */
                    currAction.success = (volatile bool *)&(damn_p2p_sub_rxbuf[id][len >> 2]);

                    (void)mq_send(gI2C_MasterActionQueue, (char *)&(currAction), sizeof(damn_master_action_t), 0);

                    p2p_sublist[i].last_sent = time_ms;
                }
                else
                {
                    DAMN_ENABLE_INTS();
                }
            }
        }

        usleep(10000);
    } /* End infinite task loop*/

    return NULL;
}

