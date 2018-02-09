/*
 * dmcf.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: Andrew
 */


#include <pthread.h>
#include "dmcf.hpp"

namespace DMCF
{

/* Thread wrappers to get around pthread limitations. Pass object as arg0,
 * call member function there */
void *p2p_threadWrapper(void *arg0)
{
    dmcf *obj = reinterpret_cast<dmcf *>(arg0);
    return obj->p2p_sub_scheduler_task(NULL);
}

void *i2cM_threadWrapper(void *arg0)
{
    dmcf *obj = reinterpret_cast<dmcf *>(arg0);
    return obj->i2cMasterThread(NULL);
}
void *i2cS_threadWrapper(void *arg0)
{
    dmcf *obj = reinterpret_cast<dmcf *>(arg0);
    return obj->i2cSlaveThread(NULL);
}


dmcf::dmcf(node_t appName, bus_type_t busType)
{
    this->appName = appName;
    this->busType = busType;


    pthread_attr_t      threadAttr;
    struct sched_param  priorityParams;
    int32_t             detachState;
    int32_t             retc;
    uint32_t            i;
    msgdef_t       *msgdefptr;

    if(I2C_FAIL == i2c_init())
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

    retc = pthread_create(&p2pSchedPthread, &threadAttr, p2p_threadWrapper, this);

    if(retc != 0)
    {
        /* pthread_create() failed */
        while(1);
    }

    for(i = 0; i < NUM_MSG_DEFINITONS; i++)
    {
        msgdefptr = &(gTheMessageDefinitions[i]);

        thePublications[i].pmsg_def = msgdefptr;

        theSubscriptions[i].pmsg_def = msgdefptr;

    }

}

/* Initialize I2C Threads and message queue  */
i2c_status_t dmcf::i2c_init(void)
{
    i2c_status_t        retVal = I2C_FAIL;
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

    retc = pthread_create(&i2cMasterPthread, &i2cAttr, i2cM_threadWrapper, this);
    retc |= pthread_create(&i2cSlavePthread, &i2cAttr, i2cS_threadWrapper, this);

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
    I2C_MasterActionQueue = mq_open(I2C_TXQUEUE_NAME, (O_CREAT), 0, &queueAttr);
    /* mq_open returns (mqd_t)-1 on error */
    if((mqd_t)-1 == I2C_MasterActionQueue)
    {
        /* mq_open() failed */
        while(1);
    }

    return retVal;
}

} /* namespace DMCF */
