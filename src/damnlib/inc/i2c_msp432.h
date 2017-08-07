/**
 * \file 
 *
 * \brief I2C implementations for the TI MSP432 MPU
 *
 */


#ifndef I2C_MSP432_H
#define I2C_MSP432_H

#ifndef DAMN_I2C_INTERNALS_H
	#error "Don't include this file directly! Include damn_i2c_internals.h instead"
#endif

#include <ti/drivers/I2C.h>
#include <ti/drivers/I2CSlave.h>


damn_i2c_status_t i2c_msp432_master_send(damn_i2c_request_t *request);

damn_i2c_status_t i2c_msp432_master_recieve(damn_i2c_request_t *request);

damn_i2c_status_t i2c_msp432_init(void);

extern void i2c_msp432_SlaveTransferCallback(I2CSlave_Handle handle, bool status);
extern void damn_i2cslave_hwiIntFxn(uintptr_t arg);

#endif /* I2C_MSP432_H */
