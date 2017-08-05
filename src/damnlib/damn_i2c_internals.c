/*! \file damn_i2c_internals.c

    Library private i2c file that calls the architecure specific i2c functions.
*/


/******* Includes *******/
#include "damn_i2c_internals.h"

#include <pthread.h>
#include <mqueue.h>

/******* File Defines *******/
#define I2C_STACK_DEPTH (1024) /* 1kiB of stack */
#define I2C_QUEUE_DEPTH (0x14)  /* Hold 20 i2c transactions at once */
#define I2C_QUEUE_NAME ("I2C QUEUE")
#define I2C_QUEUE_WIDTH (sizeof(damn_i2c_request_t))
#define I2C_RX_BUFFER_SIZE (0x64) /* Hold 100 words of data at once */

/******* Global Data *******/
mqd_t gI2C_TXQueue;
mqd_t gI2C_RXQueue;
uint32_t gI2C_RXBuffer[I2C_RX_BUFFER_SIZE];

extern volatile int32_t damn_arch_rxDataAvail;

/* Initalize I2C Thread and message queue  */
damn_i2c_status_t damn_i2c_init(void)
{
	damn_i2c_status_t 	retVal = I2C_FAIL;
	struct mq_attr 		queueAttr;
	pthread_t 			i2cHandle;
	pthread_attr_t 		i2cAttr;
	struct sched_param 	priorityParams;
	int32_t				detachState;
	int32_t 			retc;

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

	retc = pthread_create(&i2cHandle, &i2cAttr, i2cThread, NULL);

	if (retc != 0)
	{
		/* pthread_create() failed */
		while(1);
	}

	queueAttr.mq_flags = 0; /* flags are set by mq_open based on second argument */
	queueAttr.mq_maxmsg = I2C_QUEUE_DEPTH;
	queueAttr.mq_msgsize = sizeof(damn_i2c_request_t);
	queueAttr.mq_curmsgs = 0;

	/* RTOS implementaiton only cares about O_CREAT, O_EXCL and O_NONBLOCK */
	/* RTOS implemenation discards third argument, mode. */
	gI2C_TXQueue = mq_open(I2C_QUEUE_NAME, (O_CREAT | O_NONBLOCK), 0, &queueAttr);
	/* mq_open returns (mqd_t)-1 on error */
	if((mqd_t)-1 == gI2C_TXQueue)
	{
		/* mq_open() failed */
		while(1);
	}

	/* Create rx mutex */


	retVal = damn_arch_i2c_init();

	return retVal;
}

damn_i2c_status_t damn_arch_i2c_tx_send(damn_i2c_request_t *request)
{
#ifdef I2C_MSP432_H
    return i2c_msp432_master_send(request);
#else
    return I2C_FAIL;
#endif
}

damn_i2c_status_t damn_arch_i2c_tx_get(damn_i2c_request_t *request)
{
 #ifdef I2C_MSP432_H
    return i2c_msp432_master_recieve(request);
#else
    return I2C_FAIL;
#endif
}

damn_i2c_status_t damn_arch_i2c_init(void)
{
#ifdef I2C_MSP432_H
    return i2c_msp432_init();
#else
    return I2C_FAIL;
#endif
}



void *i2cThread(void *arg0)
{
	damn_i2c_request_t 	currRequest;
	damn_i2c_status_t 	reqStatus;
	ssize_t 			txBytes;
	//bool 				readInProgress;

	for(;;)
	{
		/* Check for application tx messages */
		/* msg_prio completely ignored by TI's implementation */
		txBytes = mq_receive(gI2C_TXQueue, (char *)&currRequest, I2C_QUEUE_WIDTH, NULL);

		if(txBytes == I2C_QUEUE_WIDTH)
		{
			/* We have a new request to service */
			if(I2C_TX_TYPE == currRequest.type)
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
		} /* Recieve i2c transmit request from queue */

		/* Check for recieved bytes on the slave */
		if(damn_arch_rxDataAvail > 0)
		{
			/* Parse packet. */
			//if !readInProgress
			//  if DAMN_MSG_HDR_BYTES available
			//		slave_read(&mypacket, DAMN_MSG_HDR_BYTES)
			//     readInProgress = true
			//     parse header
			//if readInProgresss
			//     if header.msg_size bytes available
			//			CRITICAL SECTION START
			//			read whole packet
			//			CRITICAL SECTION END
			//			if header.type is one that application publishes
			//				grab response from application queue nonblocking
			//					send error if no message available
			//					start slave transmit otherwise
			//			else if header.type is a broadcast that application subscribes to
			//				put packet in application message queue
			//			else 
			//				ignore packet
			//			readInProgress = false
			//
			//
		}
	} /* Infinite task loop */
	return NULL;
} /* i2cThread */
