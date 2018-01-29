/*! \file i2c_msp432.c

   implements the TI MSP432 MPU specific functions for i2c
*/

#include "dmcf_i2c_internals.h"
#include "dmcf_msgdef.h"
#include <stdbool.h>

/* Driver Header files */
#include <ti/drivers/I2C.h>
#include <ti/drivers/I2CSlave.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/i2cslave/I2CSlaveMSP432.h>
#include <ti/drivers/i2c/I2CMSP432.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/devices/msp432p4xx/driverlib/i2c.h>

#ifndef I2C_MSP432_H
    #error "I2C_MSP432_H not defined!"
#endif

#ifndef DMCF_I2C_SLAVE_PORT
    #define DMCF_I2C_SLAVE_PORT (0)
#endif
#ifndef DMCF_I2C_MASTER_PORT
    #define DMCF_I2C_MASTER_PORT (1)
#endif

#ifdef FREERTOS 
    #include <FreeRTOS.h>
    #include <task.h>
#else
    #error "FreeRTOS needed to do task notification for I2C Slave. Implement other notificaiton schemes (semaphore?)"
#endif

#define ALL_READ_INTERRUPTS (EUSCI_B_I2C_RECEIVE_INTERRUPT0  | \
    EUSCI_B_I2C_ARBITRATIONLOST_INTERRUPT | EUSCI_B_I2C_STOP_INTERRUPT | \
    EUSCI_B_I2C_START_INTERRUPT )

static I2C_Handle i2cMasterHandle;
static I2CSlave_Handle i2cSlaveHandle;

dmcf_i2c_status_t i2c_msp432_master_transfer(dmcf_i2c_trans_t *request)
{
    bool i2cRet = false;

    i2cRet = I2C_transfer(i2cMasterHandle, request);

    return (true == i2cRet) ? I2C_SUCCESS : I2C_FAIL; 
}

dmcf_i2c_status_t i2c_msp432_slave_send(void *buf, size_t size)
{
    bool i2cRet = false;

    i2cRet = I2CSlave_write(i2cSlaveHandle, buf, size);

    return (true == i2cRet) ? I2C_SUCCESS : I2C_FAIL;
}

dmcf_i2c_status_t i2c_msp432_init(void)
{
    dmcf_i2c_status_t retVal = I2C_FAIL;

    /* Initialize master and slave i2c */
    I2C_Params masterParams;
    I2CSlave_Params slaveParams;

    I2C_init();
    I2CSlave_init();

    I2C_Params_init(&masterParams);
    masterParams.transferMode = I2C_MODE_BLOCKING;
    masterParams.bitRate = I2C_100kHz;
    i2cMasterHandle = I2C_open(DMCF_I2C_MASTER_PORT, &masterParams); 

    I2CSlave_Params_init(&slaveParams);
    slaveParams.transferMode = I2CSLAVE_MODE_CALLBACK;
    slaveParams.transferCallbackFxn = i2c_msp432_SlaveTransferCallback;
    i2cSlaveHandle = I2CSlave_open(DMCF_I2C_SLAVE_PORT, &slaveParams);

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
        /* Install our own interrupt on the I2C Slave, enable general call */
        i2c_msp432_slaveFixups();

        /* Enable multi-master mode, install our own interrupt */
        i2c_msp432_masterFixups();
    }
    
    return retVal;
}

void i2c_msp432_slaveFixups(void)
{
    I2CSlaveMSP432_HWAttrs const *hwAttrs = ((I2CSlave_Handle) i2cSlaveHandle)->hwAttrs;
    I2CSlaveMSP432_Object        *object = ((I2CSlave_Handle) i2cSlaveHandle)->object;

    /* Disable module by setting UCSoftWareReSeT = 1 in ConTroLWord0 */
    MAP_I2C_disableModule(hwAttrs->baseAddr);

    i2c_msp432_enableGeneralCall(hwAttrs);

    i2c_msp432_enableCustomSlaveInt(hwAttrs, object);

    /* re-enable by clearing UCSoftWareReSeTinConTroLWord0 */
    MAP_I2C_enableModule(hwAttrs->baseAddr);

    /* Enable read interrupts, we always want to listen */
    MAP_I2C_enableInterrupt(hwAttrs->baseAddr, ALL_READ_INTERRUPTS);
}


/* Enable the general call register on the i2cSlave. Don't want i2c master as gcen because then it will
 * reply to general call if arbitration lost (not what we want)
 */
void i2c_msp432_enableGeneralCall(I2CSlaveMSP432_HWAttrs const *hwAttrs)
{
    /* Set General Call Response Enable in I2COwnAddress0 */
    HWREG16((uint32_t) &EUSCI_B_CMSIS(hwAttrs->baseAddr)->I2COA0) |= EUSCI_B_I2COA0_GCEN;
}

void i2c_msp432_enableCustomSlaveInt(I2CSlaveMSP432_HWAttrs const *hwAttrs, I2CSlaveMSP432_Object *object)
{
    HwiP_Params     interruptParams;

    HwiP_Params_init(&interruptParams);
    interruptParams.arg = (uintptr_t) i2cSlaveHandle;
    interruptParams.priority = hwAttrs->intPriority;
    /* Get rid of TI's Interrupt handler */
    HwiP_delete(object->hwiHandle);
    /* And install ours */
    object->hwiHandle = HwiP_create( hwAttrs->intNum,
                                     dmcf_i2cslave_hwiIntFxn,
                                     &interruptParams);
}

void i2c_msp432_masterFixups(void)
{
    I2CMSP432_HWAttrsV1 const *hwAttrs = ((I2C_Handle) i2cMasterHandle)->hwAttrs;
    I2CMSP432_Object *object = ((I2C_Handle) i2cMasterHandle)->object;

    MAP_I2C_disableModule(hwAttrs->baseAddr);

    I2C_enableMultiMasterMode(hwAttrs->baseAddr);

    i2c_msp432_enableCustomMasterInt(hwAttrs, object);

    MAP_I2C_enableModule(hwAttrs->baseAddr);  
}

void i2c_msp432_enableCustomMasterInt(I2CMSP432_HWAttrsV1 const *hwAttrs, I2CMSP432_Object *object)
{
    HwiP_Params     interruptParams;

    HwiP_Params_init(&interruptParams);
    interruptParams.arg = (uintptr_t) i2cMasterHandle;
    interruptParams.priority = hwAttrs->intPriority;
    /* Get rid of TI's Interrupt handler */
    HwiP_delete(object->hwiHandle);
    /* And install ours */
    object->hwiHandle = HwiP_create( hwAttrs->intNum,
                                     dmcf_i2cmaster_hwiIntFxn,
                                     &interruptParams);
}
