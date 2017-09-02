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
#include <ti/drivers/i2cslave/I2CSlaveMSP432.h>
#include <ti/drivers/i2c/I2CMSP432.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


damn_i2c_status_t i2c_msp432_master_transfer(damn_i2c_trans_t *request);

damn_i2c_status_t i2c_msp432_init(void);

extern void i2c_msp432_SlaveTransferCallback(I2CSlave_Handle handle, bool status);
extern void damn_i2cslave_hwiIntFxn(uintptr_t arg);
extern void damn_i2cmaster_hwiIntFxn(uintptr_t arg);

void i2c_msp432_enableGeneralCall(I2CSlaveMSP432_HWAttrs const *hwAttrs);
void i2c_msp432_enableCustomSlaveInt(I2CSlaveMSP432_HWAttrs const *hwAttrs, I2CSlaveMSP432_Object *object);
void i2c_msp432_slaveFixups(void);

void i2c_msp432_masterFixups(void);
void i2c_msp432_enableCustomMasterInt(I2CMSP432_HWAttrsV1 const *hwAttrs, I2CMSP432_Object *object);

#endif /* I2C_MSP432_H */
