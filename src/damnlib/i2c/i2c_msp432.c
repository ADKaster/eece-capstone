/*! \file i2c_msp432.c

   implements the TI MSP432 MPU specific functions for i2c
*/

#include "damn_i2c_internals.h"
#include "damn_msgdef.h"

#ifndef I2C_MSP432_H
	#error "I2C_MSP432_H not defined!"
#endif

#ifndef DAMN_I2C_SLAVE_PORT
	#define DAMN_I2C_SLAVE_PORT (0)
#endif
#ifndef DAMN_I2C_MASTER_PORT
	#define DAMN_I2C_MASTER_PORT (1)
#endif

#ifdef FREERTOS 
	#include <FreeRTOS.h>
	#include <task.h>
#else
	#error "FreeRTOS needed to do task notification for I2C Slave. Implement other notificaiton schemes (semaphore?)"
#endif

/* Driver Header files */
#include <ti/drivers/I2C.h>
#include <ti/drivers/I2CSlave.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/i2cslave/I2CSlaveMSP432.h>

/* Example/Board Header files */
#include "Board.h"

static I2C_Handle i2cMasterHandle;
static I2CSlave_Handle i2cSlaveHandle;

damn_i2c_status_t i2c_msp432_master_send(damn_i2c_request_t *request)
{
	return I2C_FAIL; 
}

damn_i2c_status_t i2c_msp432_master_recieve(damn_i2c_request_t *request)
{
	return I2C_FAIL;
}

damn_i2c_status_t i2c_msp432_init(void)
{
	damn_i2c_status_t retVal = I2C_FAIL;

	/* Initialize master and slave i2c */
	I2C_Params masterParams;
	I2CSlave_Params slaveParams;
	HwiP_Params     interruptParams;
	I2CSlaveMSP432_Object        *object = ((I2CSlave_Handle) i2cSlaveHandle)->object;
    I2CSlaveMSP432_HWAttrs const *hwAttrs = ((I2CSlave_Handle) i2cSlaveHandle)->hwAttrs;

	I2C_init();
	I2CSlave_init();

	I2C_Params_init(&masterParams);
	masterParams.transferMode = I2C_MODE_BLOCKING;
	masterParams.bitRate = I2C_100kHz;
	i2cMasterHandle = I2C_open(DAMN_I2C_MASTER_PORT, &masterParams); 

	I2CSlave_Params_init(&slaveParams);
	slaveParams.transferMode = I2CSLAVE_MODE_CALLBACK;
	slaveParams.transferCallbackFxn = i2c_msp432_SlaveTransferCallback;
	i2cSlaveHandle = I2CSlave_open(DAMN_I2C_SLAVE_PORT, &slaveParams);

	if(NULL == i2cMasterHandle || NULL == i2cSlaveHandle)
	{
		retVal = I2C_ABORT;
	}
	else
	{
		retVal = I2C_SUCCESS;
	}
	if(retVal == I2C_SUCCESS)
	{
		/* Install our own interrupt on the I2C Slave */
    	HwiP_Params_init(&interruptParams);
    	interruptParams.arg = (uintptr_t) i2cSlaveHandle;
    	interruptParams.priority = hwAttrs->intPriority;
    	/* Get rid of TI's Interrupt handler */
		HwiP_delete(object->hwiHandle);
		/* And install ours */
		object->hwiHandle = HwiP_create( hwAttrs->intNum,
										 damn_i2cslave_hwiIntFxn,
										 &interruptParams);
	}
	
	return retVal;
}


