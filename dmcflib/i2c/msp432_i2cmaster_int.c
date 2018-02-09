/* \file i2c master interrupt
    Re-implementation of TI's I2C Master interrupt because we need to handle Arbitration lost
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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <FreeRTOS.h>
#include <task.h>

/*
 * By default disable both asserts and log for this module.
 * This must be done before DebugP.h is included.
 */
#ifndef DebugP_ASSERT_ENABLED
#define DebugP_ASSERT_ENABLED 0
#endif
#ifndef DebugP_LOG_ENABLED
#define DebugP_LOG_ENABLED 4
#endif

#include <ti/devices/DeviceFamily.h>

#include <ti/drivers/I2C.h>
#include <ti/drivers/I2CSlave.h>
#include <ti/drivers/i2c/I2CMSP432.h>
#include <ti/drivers/dpl/DebugP.h>
#include <ti/drivers/dpl/HwiP.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerMSP432.h>

/* driverlib header files */
#include <ti/devices/msp432p4xx/driverlib/rom.h>
#include <ti/devices/msp432p4xx/driverlib/rom_map.h>
#include <ti/devices/msp432p4xx/driverlib/i2c.h>
#include <ti/devices/msp432p4xx/driverlib/gpio.h>
#include <ti/devices/msp432p4xx/driverlib/pmap.h>

#define ERROR_INTERRUPTS (EUSCI_B_I2C_NAK_INTERRUPT | \
    EUSCI_B_I2C_ARBITRATIONLOST_INTERRUPT)

#define ALL_INTERRUPTS (EUSCI_B_I2C_RECEIVE_INTERRUPT0  | \
    EUSCI_B_I2C_TRANSMIT_INTERRUPT0 | ERROR_INTERRUPTS)

static void completeTransfer(I2C_Handle handle);
static void primeTransfer(I2CMSP432_Object *object,
    I2CMSP432_HWAttrsV1 const *hwAttrs, I2C_Transaction *transaction);

extern TaskHandle_t slaveNotificationHandle;

void i2c_msp432_SlaveTransferCallback(I2CSlave_Handle handle, bool status)
{
    int32_t                     xHigherPriorityTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR(slaveNotificationHandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


/*
 *  ======== completeTransfer =======
 */
static void completeTransfer(I2C_Handle handle)
{
    I2CMSP432_Object          *object = ((I2C_Handle) handle)->object;
    I2CMSP432_HWAttrsV1 const *hwAttrs = ((I2C_Handle) handle)->hwAttrs;

    DebugP_log1("I2C:(%p) ISR Transfer Complete", hwAttrs->baseAddr);

    /*
     * Perform callback in a HWI context, thus any tasks or SWIs
     * made ready to run won't start until the interrupt has
     * finished
     */
    object->transferCallbackFxn(handle, object->currentTransaction,
        (object->mode == I2CMSP432_IDLE_MODE));

    /* See if we need to process any other transactions */
    if (object->headPtr == object->tailPtr) {
        /* No other transactions need to occur */
        object->currentTransaction = NULL;
        object->headPtr = NULL;

        /* Remove constraints set during transfer */
#if DeviceFamily_ID == DeviceFamily_ID_MSP432P401x
        Power_releaseConstraint(PowerMSP432_DISALLOW_DEEPSLEEP_0);
#endif
        Power_releaseConstraint(PowerMSP432_DISALLOW_PERF_CHANGES);

        DebugP_log1("I2C:(%p) ISR No other I2C transaction in queue",
            hwAttrs->baseAddr);
    }
    else {
        /* This should never happen if you are in blocking mode */
        DebugP_assert(object->transferMode == I2C_MODE_CALLBACK);

        /* Another transfer needs to take place */
        object->headPtr = object->headPtr->nextPtr;

        DebugP_log2("I2C:(%p) ISR Priming next I2C transaction (%p) from queue",
            hwAttrs->baseAddr, (uintptr_t) object->headPtr);

        primeTransfer(object, hwAttrs, object->headPtr);
    }
}

/*
 *  ======== primeTransfer =======
 */
static void primeTransfer(I2CMSP432_Object *object,
    I2CMSP432_HWAttrsV1 const *hwAttrs, I2C_Transaction *transaction)
{
    /* Store the new internal counters and pointers */
    object->currentTransaction = transaction;

    object->writeBufIdx = transaction->writeBuf;
    object->writeCountIdx = transaction->writeCount;

    object->readBufIdx = transaction->readBuf;
    object->readCountIdx = transaction->readCount;

    DebugP_log2("I2C:(%p) Starting transaction to slave: 0x%x",
        hwAttrs->baseAddr, object->currentTransaction->slaveAddress);

    MAP_I2C_setSlaveAddress(hwAttrs->baseAddr,
        object->currentTransaction->slaveAddress);

    /* Start transfer in Transmit mode */
    if (object->writeCountIdx) {
        object->mode = I2CMSP432_WRITE_MODE;
        MAP_I2C_setMode(hwAttrs->baseAddr, EUSCI_B_I2C_TRANSMIT_MODE);

        /* Start the I2C transfer by sending the start bit */
        MAP_I2C_masterSendStart(hwAttrs->baseAddr);

        DebugP_log1("I2C:(%p) I2CMSP432_IDLE_MODE: -> I2CMSP432_WRITE_MODE; "
            "Writing w/ START", hwAttrs->baseAddr);
    }
    /* Start transfer in Receive mode */
    else {
        object->mode = I2CMSP432_READ_MODE;
        MAP_I2C_setMode(hwAttrs->baseAddr, EUSCI_B_I2C_RECEIVE_MODE);
        MAP_I2C_masterReceiveStart(hwAttrs->baseAddr);

        if (object->readCountIdx > 1) {
            DebugP_log1("I2C:(%p) I2CMSP432_IDLE_MODE: -> I2CMSP432_READ_MODE; "
                "Reading w/ ACK", hwAttrs->baseAddr);
        }
        else {
            /*
             * Need to wait for the start bit to have transmitted before we can
             * tell it to send a stop bit.
             */
            while (MAP_I2C_masterIsStartSent(hwAttrs->baseAddr)) {}

            MAP_I2C_masterReceiveMultiByteStop(hwAttrs->baseAddr);

            DebugP_log1("I2C:(%p) I2CMSP432_IDLE_MODE: -> I2CMSP432_READ_MODE; "
                "Reading w/ NACK", hwAttrs->baseAddr);
        }
    }
}

/*
 *  Replacement for TI's hardware interrupt that handles Arbitration loss by resetting peripheral to master mode
 */
void dmcf_i2cmaster_hwiIntFxn(uintptr_t arg)
{
    uint8_t                    intStatus;
    unsigned int               key;
    I2CMSP432_Object          *object = ((I2C_Handle) arg)->object;
    I2CMSP432_HWAttrsV1 const *hwAttrs = ((I2C_Handle) arg)->hwAttrs;
    bool status = true;


    /* Get the interrupt status of the I2C controller */
    intStatus = MAP_I2C_getEnabledInterruptStatus(hwAttrs->baseAddr);
    MAP_I2C_clearInterruptFlag(hwAttrs->baseAddr, intStatus);

    /* Filter any spurious interrupts */
    if (!(intStatus & ALL_INTERRUPTS)) {
        return;
    }


    /* Check for I2C Errors */
    if ((intStatus & ERROR_INTERRUPTS) || (object->mode == I2CMSP432_ERROR)) {

        /* Some sort of error occurred */
        object->mode = I2CMSP432_ERROR;

        /* Try to send a STOP bit to end all I2C communications immediately. */
        MAP_I2C_masterReceiveMultiByteStop(hwAttrs->baseAddr);

/******** BEGIN CHANGES ********/
        if((intStatus & EUSCI_B_I2C_ARBITRATIONLOST_INTERRUPT))
        {
            /* Check for arbitration lost and re-enable UCMST bit in CTLW0 */
            EUSCI_B_CMSIS(hwAttrs->baseAddr)->CTLW0 |= EUSCI_B_CTLW0_MST;
        }
/******** END CHANGES ********/

        DebugP_log2("I2C:(%p) ISR I2C Bus fault (Status Reg: 0x%x)",
            hwAttrs->baseAddr, intStatus);
    }

    /* No errors, now check what we need to do next */
    switch (object->mode) {
        /*
         * ERROR case is OK because if an Error is detected, a STOP bit is
         * sent; which in turn will call another interrupt. This interrupt
         * call will then post the transferComplete semaphore to unblock the
         * I2C_transfer function
         */
        case I2CMSP432_ERROR:
        case I2CMSP432_IDLE_MODE:
            completeTransfer((I2C_Handle) arg);
            break;

        case I2CMSP432_WRITE_MODE:
            if (object->writeCountIdx) {
                /* If on the last byte to write and have NO data to read */
                if ((object->writeCountIdx == 1) && (!object->readCountIdx)) {
                    /* Next state: Idle mode */
                    object->mode = I2CMSP432_IDLE_MODE;

                    /* Send last byte with STOP bit */
                    status = MAP_I2C_masterSendMultiByteFinish(hwAttrs->baseAddr,
                        *(object->writeBufIdx));

                    if (status == false) {
                        object->mode = I2CMSP432_ERROR;
                        EUSCI_B_CMSIS(hwAttrs->baseAddr)->IFG &= ~(EUSCI_B_IFG_NACKIFG);
                    }

                    //Clear transmit interrupt flag before enabling interrupt again
                    EUSCI_B_CMSIS(hwAttrs->baseAddr)->IFG &= ~(EUSCI_B_IFG_TXIFG);

                    MAP_I2C_clearInterruptFlag(hwAttrs->baseAddr,
                        EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

                    DebugP_log2("I2C:(%p) ISR I2CMSP432_WRITE_MODE:"
                        "Data to write: 0x%x; Writing w/ STOP bit",
                        hwAttrs->baseAddr, *(object->writeBufIdx));

                    completeTransfer((I2C_Handle) arg);
                }
                else {
                    /* Write data contents into data register */
                    MAP_I2C_masterSendMultiByteNext(hwAttrs->baseAddr,
                        *(object->writeBufIdx));

                    DebugP_log3("I2C:(%p) ISR I2CMSP432_WRITE_MODE: "
                        "Data to write: 0x%x; To slave: 0x%x",
                        hwAttrs->baseAddr, *(object->writeBufIdx),
                        object->currentTransaction->slaveAddress);

                    object->writeBufIdx++;
                    object->writeCountIdx--;
                }
            }
            /*
             * Done writing data to the I2C slave. If no data need be read,
             * the ISR would not get here as it would have finished the I2C
             * transfer in the logical check above.
             */
            else {
                /* Next state: Receive mode */
                object->mode = I2CMSP432_READ_MODE;
                MAP_I2C_setMode(hwAttrs->baseAddr, EUSCI_B_I2C_RECEIVE_MODE);

                if (object->readCountIdx > 1) {
                    /* Start the I2C transfer in master receive mode */
                    MAP_I2C_masterReceiveStart(hwAttrs->baseAddr);
                    DebugP_log1("I2C:(%p) I2CMSP432_WRITE_MODE: -> I2CMSP432_READ_MODE; "
                        "Reading w/ RESTART", hwAttrs->baseAddr);
                }
                else {
                    /*
                     * Start the I2C transfer in master receive mode by sending
                     * a START and STOP bit since its the last byte
                     */
                    MAP_I2C_masterReceiveStart(hwAttrs->baseAddr);

                    while (MAP_I2C_masterIsStartSent(hwAttrs->baseAddr)) {}
                    MAP_I2C_masterReceiveMultiByteStop(hwAttrs->baseAddr);

                    DebugP_log1("I2C:(%p) I2CMSP432_WRITE_MODE: -> I2CMSP432_READ_MODE; "
                        "Reading w/ RESTART and STOP", hwAttrs->baseAddr);
                }
            }
            break; /* I2CMSP432_WRITE_MODE */

        case I2CMSP432_READ_MODE:
            /* Data read from RXBUF and next byte has already been shifted */
            object->readCountIdx--;

            if (object->readCountIdx == 1) {
                /* Timing critical block which mustn't be simplified - START */
                key = HwiP_disable();

                MAP_I2C_masterReceiveMultiByteStop(hwAttrs->baseAddr);
                *(object->readBufIdx) =
                    MAP_I2C_masterReceiveMultiByteNext(hwAttrs->baseAddr);

                HwiP_restore(key);
                /* Timing critical block which mustn't be simplified - END */

                DebugP_log2("I2C:(%p) ISR I2CMSP432_READ_MODE: Read data byte: 0x%x",
                    hwAttrs->baseAddr, *(object->readBufIdx));

                object->readBufIdx++;
            }
            else {
                *(object->readBufIdx) =
                    MAP_I2C_masterReceiveMultiByteNext(hwAttrs->baseAddr);

                DebugP_log2("I2C:(%p) ISR I2CMSP432_READ_MODE: "
                    "Read data byte: 0x%x ", hwAttrs->baseAddr,
                    *(object->readBufIdx));

                if (object->readCountIdx) {
                    /* Increment buffer pointer */
                    object->readBufIdx++;
                }
                else {
                    /* Next state: Idle mode */
                    object->mode = I2CMSP432_IDLE_MODE;
                    completeTransfer((I2C_Handle) arg);
                }
            }
            break; /* I2CMSP432_READ_MODE */

        default:
            object->mode = I2CMSP432_ERROR;
            break;
    }
}

#ifdef __cplusplus
}
#endif
