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

/* Driver Header files */
#include <ti/drivers/I2C.h>
#include <ti/drivers/I2CSlave.h>

/* Example/Board Header files */
#include "Board.h"

static I2C_Handle i2cMasterHandle;
static I2CSlave_Handle i2cSlaveHandle;
int32_t damn_arch_rxDataAvail;

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

	return retVal;
}

void i2c_msp432_SlaveTransferCallback(I2CSlave_Handle handle, bool status)
{
	/* TODO: Need to implement custom I2CSlave_Open and I2C ISR to blindly store all */
	/* recieved data into a ring buffer and let the number of byes in the buffer be available */
    /* to the application*/
    damn_arch_rxDataAvail = -1;
}
