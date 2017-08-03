/*! \file damn_i2c_internals.c

    Library private i2c file that calls the architecure specific i2c functions.
*/


/******* Includes *******/
#include "damn_i2c_internals.h"

#ifdef FREERTOS
	#include <FreeRTOS.h>
	#include <queue.h>
	#include <task.h>
#else
    #error Non-FreeRTOS Implementations unsupported
#endif

/******* File Defines *******/
#define I2C_STACK_DEPTH 0x100 /* 1kiB of stack */
#define I2C_QUEUE_DEPTH 0x20  /* Hold 32 i2c transactions at once */


/******* Global Data *******/
QueueHandle_t gI2CServiceQueue;

damn_i2c_status_t damn_i2c_init(void)
{
	damn_i2c_status_t retVal = I2C_FAIL;
	TaskHandle_t i2cHandle = NULL;

	xTaskCreate(i2cThread, "I2C SERVICE", I2C_STACK_DEPTH, NULL, tskIDLE_PRIORITY, &i2cHandle);

	configASSERT(i2cHandle);

	retVal = damn_arch_i2c_init();

	return retVal;
}

damn_i2c_status_t damn_arch_i2c_send(damn_i2c_request_t *request)
{
#ifdef I2C_MSP432_H
    return i2c_msp432_send(request);
#else
    return I2C_FAIL;
#endif
}

damn_i2c_status_t damn_arch_i2c_recieve(damn_i2c_request_t *request)
{
 #ifdef I2C_MSP432_H
    return i2c_msp432_recieve(request);
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



void i2cThread(void *arg0)
{
	static damn_i2c_request_t currRequest;
	static damn_i2c_status_t reqStatus;

	gI2CServiceQueue = xQueueCreate(I2C_QUEUE_DEPTH, sizeof(damn_i2c_request_t));

	configASSERT(gI2CServiceQueue);

	for(;;)
	{
		/* Block until queue has a new request to service */
		if(xQueueReceive(gI2CServiceQueue ,&currRequest, portMAX_DELAY))
		{
			/* We have a new request to service */
			if(I2C_TX_TYPE == currRequest.type)
			{
				reqStatus = damn_arch_i2c_send(&currRequest);
			}
			else
			{
				reqStatus = damn_arch_i2c_recieve(&currRequest);
			} /* Check i2c Request type */
			/* Suppress compiler warning... TODO notify somebody if status isn't success */
			(void)reqStatus;
		} /* Recieve i2c request from queue */
	} /* Infinite task loop */
} /* i2cThread */
