/* \file re-implemtation of TI's I2C Slave interrupt 
 *
 * Their interrupt doesn't do everything we want, so... yeah. time to re-implement! :D
 * 
 */


/*
 * Copyright (c) 2015-2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef __MSP432P401R__

#include <stdint.h>
#include <stdbool.h>

/*
 * By default disable both asserts and log for this module.
 * This must be done before DebugP.h is included.
 */
#ifndef DebugP_ASSERT_ENABLED
#define DebugP_ASSERT_ENABLED 0
#endif
#ifndef DebugP_LOG_ENABLED
#define DebugP_LOG_ENABLED 0
#endif

#include <ti/devices/DeviceFamily.h>

#include <ti/drivers/dpl/DebugP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/I2CSlave.h>
#include <ti/drivers/i2cslave/I2CSlaveMSP432.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerMSP432.h>


/* driverlib header files */
#include <ti/devices/msp432p4xx/driverlib/rom.h>
#include <ti/devices/msp432p4xx/driverlib/rom_map.h>
#include <ti/devices/msp432p4xx/driverlib/i2c.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>
#include <ti/devices/msp432p4xx/driverlib/pmap.h>


#define ERROR_INTERRUPTS (EUSCI_B_I2C_ARBITRATIONLOST_INTERRUPT)

#define ALL_INTERRUPTS (EUSCI_B_I2C_RECEIVE_INTERRUPT0  | \
    EUSCI_B_I2C_TRANSMIT_INTERRUPT0 | ERROR_INTERRUPTS | \
    EUSCI_B_I2C_STOP_INTERRUPT | EUSCI_B_I2C_START_INTERRUPT )

#define ALL_READ_INTERRUPTS (EUSCI_B_I2C_RECEIVE_INTERRUPT0  | \
    ERROR_INTERRUPTS | EUSCI_B_I2C_STOP_INTERRUPT | \
    EUSCI_B_I2C_START_INTERRUPT )

#define ALL_WRITE_INTERRUPTS ( EUSCI_B_I2C_TRANSMIT_INTERRUPT0 | \
    ERROR_INTERRUPTS |     EUSCI_B_I2C_STOP_INTERRUPT | \
    EUSCI_B_I2C_START_INTERRUPT )

/*********************BEGIN CHANGES*****************/

#include "ringbuf.h"
#include "damn_i2c_internals.h"
#include <FreeRTOS.h>
#include <task.h>
#include <pthread.h>



static volatile ringbuf_t *pSlaveRingbuf = (volatile ringbuf_t *)&gI2C_SlaveRingbuf;
static volatile uint32_t num_bytes_rx = 0;
static volatile TaskHandle_t *slaveTaskPtr = (TaskHandle_t *)(&i2cSlavePthread);

void i2c_msp432_SlaveTransferCallback(I2CSlave_Handle handle, bool status)
{
}


/*
 *  ======== completeTransfer =======
 */
static void completeTransfer(I2CSlave_Handle handle)
{
    I2CSlaveMSP432_Object *object = ((I2CSlave_Handle) handle)->object;
    DebugP_log1("I2CSlave:(%p) ISR Transfer Complete",
        ((I2CSlaveMSP432_HWAttrs const *)(handle->hwAttrs))->baseAddr);

    object->transferInProgress = false;
    /*
     * Perform callback in a HWI context, thus any tasks or SWIs
     * made ready to run won't start until the interrupt has
     * finished
     */
    object->transferCallbackFxn(handle, (object->mode == I2CSLAVE_IDLE_MODE));

    /* never disallow deep sleep, we want to always listen */
}

/*
 * Re-implementation of TI's hardware interrupt with custom features for our project
 */
void damn_i2cslave_hwiIntFxn(uintptr_t arg)
{
    int32_t                     xHigherPriorityTaskWoken = pdFALSE;
/*****************************END CHANGES****************************************/
    uint16_t                     intStatus;
    I2CSlaveMSP432_Object        *object = ((I2CSlave_Handle) arg)->object;
    I2CSlaveMSP432_HWAttrs const *hwAttrs = ((I2CSlave_Handle) arg)->hwAttrs;

    /* Get the interrupt status of the I2CSlave controller */
    intStatus = MAP_I2C_getEnabledInterruptStatus(hwAttrs->baseAddr);
    MAP_I2C_clearInterruptFlag(hwAttrs->baseAddr, intStatus);

    /* Filter any spurious interrupts */
    if (!(intStatus & ALL_INTERRUPTS)) {
        return;
    }

    /* Check for I2CSlave Errors */
    if ((intStatus & ERROR_INTERRUPTS)) {
        /* Some sort of error occurred */
        object->mode = I2CSLAVE_ERROR;

        DebugP_log2("I2CSlave:(%p) ISR I2CSlave Bus fault (Status Reg: 0x%x)",
            hwAttrs->baseAddr, intStatus);
    }

    if(intStatus & EUSCI_B_I2C_RECEIVE_INTERRUPT0 )
            object->mode = I2CSLAVE_READ_MODE;
         else if(intStatus & EUSCI_B_I2C_TRANSMIT_INTERRUPT0 )
                object->mode = I2CSLAVE_WRITE_MODE;
              else if(intStatus & EUSCI_B_I2C_START_INTERRUPT )
                           object->mode = I2CSLAVE_START_MODE;
                   else if(intStatus & EUSCI_B_I2C_STOP_INTERRUPT )
                       object->mode = I2CSLAVE_IDLE_MODE;


    /* No errors, now check what we need to do next */
    switch (object->mode) {
        case I2CSLAVE_ERROR:
        case I2CSLAVE_IDLE_MODE:
            completeTransfer((I2CSlave_Handle) arg);
            /* Make sure read interrupts are enabled so we don't miss anything :) */
            MAP_I2C_enableInterrupt(hwAttrs->baseAddr, ALL_READ_INTERRUPTS);
            break;
        case I2CSLAVE_START_MODE:
            if(object->transferInProgress)
            {
                completeTransfer((I2CSlave_Handle) arg);
                object->mode = I2CSLAVE_IDLE_MODE;
                /* Make sure read interrupts are enabled so we don't miss anything :) */
                MAP_I2C_enableInterrupt(hwAttrs->baseAddr, ALL_READ_INTERRUPTS);
            }
                break;

        case I2CSLAVE_WRITE_MODE:
            object->transferInProgress = true;
              if (object->countIdx){
                  /* Write data contents into data register */
                MAP_I2C_clearInterruptFlag(hwAttrs->baseAddr,
                    EUSCI_B_I2C_TRANSMIT_INTERRUPT0);
                MAP_I2C_slavePutData(hwAttrs->baseAddr,
                                     *(object->writeBufferIdx));
                DebugP_log2("I2CSlave:(%p) ISR I2CSLAVE_WRITE_MODE: "
                    "Data to write: 0x%x",
                    hwAttrs->baseAddr, *(object->writeBufferIdx));

                object->writeBufferIdx++;
                object->countIdx--;
            }
            else {
                    DebugP_log1("I2CSlave:(%p) ISR I2CSLAVE_WRITE_MODE: "
                        "No more bytes available to write From slave: 0xFF",
                        hwAttrs->baseAddr);
                    object->mode = I2CSLAVE_ERROR;
                    MAP_I2C_clearInterruptFlag(hwAttrs->baseAddr,
                        EUSCI_B_I2C_TRANSMIT_INTERRUPT0);
                    MAP_I2C_slavePutData(hwAttrs->baseAddr, 0xFF);
                    DebugP_log2("I2CSlave:(%p) ISR I2CSLAVE_WRITE_MODE: "
                        "Data to write: 0x%x",
                        hwAttrs->baseAddr, *(object->writeBufferIdx));
            }
            break; /* I2CSLAVE_WRITE_MODE */
/***********************************BEGIN CHANGES**********************************/
        case I2CSLAVE_READ_MODE:
            object->transferInProgress = true;
            /* Data read from RXBUF and next byte has already been shifted */
            /* Put all data in ring buffer. don't care if it overruns. that's why it's a RING */
            ringbuf_put(pSlaveRingbuf, MAP_I2C_slaveGetData(hwAttrs->baseAddr));

            DebugP_log2("I2CSlave:(%p) ISR I2CSLAVE_READ_MODE: "
                "Read data byte: 0x%x",
                hwAttrs->baseAddr, pSlaveRingbuf->buf[head]);

            num_bytes_rx++;
            /* Let the i2c task know how many words we've found; */
            if(num_bytes_rx >= 4)
            {
                num_bytes_rx -= 4;
                vTaskNotifyGiveFromISR(*slaveTaskPtr, &xHigherPriorityTaskWoken);
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }
            break; /* I2CSLAVE_READ_MODE */
/*************************************END CHANGES*************************************/

        default:
            object->mode = I2CSLAVE_ERROR;
            break;
    }
}

#endif /* __MSP432P401R__ */
