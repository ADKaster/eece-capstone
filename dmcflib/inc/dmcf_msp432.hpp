/**
 * \file 
 *
 * \brief dmcf implementations for the TI MSP432 MPU
 *
 */


#ifndef DMCF_MSP432_HPP
#define DMCF_MSP432_HPP

#ifdef __MSP432P401R__

#include <ti/drivers/I2C.h>
#include <ti/drivers/I2CSlave.h>
#include <ti/drivers/i2cslave/I2CSlaveMSP432.h>
#include <ti/drivers/i2c/I2CMSP432.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdbool.h>
#include <FreeRTOS.h>
#include <task.h>

#include "dmcf.hpp"
#include "dmcf_i2c_internals.hpp"

namespace DMCF
{

class dmcf_msp432 : public dmcf
{
public:
    dmcf_msp432(node_t appName);
    ~dmcf_msp432() { /* TODO do we need to do anything here or what */ }

    bool init(void) { return i2c_init() == I2C_SUCCESS; }

private:
    kernel_task_handle_t getSlaveTaskHandle(void) { return xTaskGetCurrentTaskHandle(); }

    i2c_status_t arch_i2c_master_transfer(i2c_trans_t *request);
    i2c_status_t arch_i2c_slave_send(void *buf, size_t size);
    i2c_status_t arch_i2c_slave_read(void *buf, size_t size);
    i2c_status_t i2c_init(void);

    void i2c_msp432_enableGeneralCall(I2CSlaveMSP432_HWAttrs const *hwAttrs);
    void i2c_msp432_enableCustomSlaveInt(I2CSlaveMSP432_HWAttrs const *hwAttrs, I2CSlaveMSP432_Object *object);
    void i2c_msp432_slaveFixups(void);
    void i2c_msp432_masterFixups(void);
    void i2c_msp432_enableCustomMasterInt(I2CMSP432_HWAttrsV1 const *hwAttrs, I2CMSP432_Object *object);

    I2C_Handle i2cMasterHandle;
    I2CSlave_Handle i2cSlaveHandle;

}; /* class DMCF_msp432 */

} /* namespace DMCF */

/* interrupt linked with C linkage because yeah */
extern "C" void dmcf_i2cmaster_hwiIntFxn(uintptr_t arg);
extern "C" void i2c_msp432_SlaveTransferCallback(I2CSlave_Handle handle, bool status);

#endif /* __MSP432P401R__ */

#endif /* DMCF_MSP432_HPP */
