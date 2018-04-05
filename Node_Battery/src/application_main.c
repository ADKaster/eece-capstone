/* \file Example main thread



 */
/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
//****************************************************************************
//
// main.c - MSP-EXP432P401R + Battery Boosterpack MkII - Fuel Guage demo
//
//          Initializes BQ27441 Fuel Gauge configurations suitable for the
//          included battery and periodically prints information to serial
//          backchannel uart.
//
//****************************************************************************


#ifdef FREERTOS
    #include <FreeRTOS.h>
    #include <task.h>
#endif

#include <unistd.h>
#include <time.h>
#include "appdefs.h"
#include "DmcfLibSystem.h"
#include "dmcf_pubsub.h"
#include "dmcf_debug.h"
#include <stdio.h>
#include "HAL_BQ27441.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void *mainThread(void *arg0)
{
    TickType_t xLastWaketime = xTaskGetTickCount();
    TickType_t xFrequency = portTICK_PERIOD_MS * 1000;

    dmcf_pub_status_t       pubstatus;
    battery_sts_msg_t       battery_status;

    if (!BQ27441_initConfig())
    {
        dmcf_debugprintf("Error initializing BQ27441 Config");
        dmcf_debugprintf("Make sure BOOSTXL-BATPAKMKII is connected and switch is flipped to \"CONNECTED\"");
    }

    if(!BQ27441_initOpConfig())
    {
        dmcf_debugprintf("Clearing BIE in Operation Configuration");
        usleep(200000);
        // time to reset baby
        MAP_WDT_A_startTimer();
        while(1);
    }

    BQ27441_control(BAT_INSERT, 1000);

    for(;;)
    {
        clock_gettime(CLOCK_REALTIME, &battery_status.time);

        /* Read Design Capacity */
        if(!BQ27441_read16(DESIGN_CAPACITY, &battery_status.design_capacity, 1000))
        {
            battery_status.design_capacity = (~0);
        }

        /* Read Remaining Capacity */
        if(!BQ27441_read16(REMAINING_CAPACITY, &battery_status.remaining_capacity, 1000))
        {
            battery_status.remaining_capacity = (~0);
        }

        /* Read State Of Charge */
        if(!BQ27441_read16(STATE_OF_CHARGE, &battery_status.state_of_charge, 1000))
        {
            battery_status.state_of_charge = (~0);
        }

        /* Read Temperature */
        if(!BQ27441_read16(TEMPERATURE, &battery_status.temperature, 1000))
        {
            battery_status.temperature = (~0);
        }
        else
        {
            battery_status.temperature = battery_status.temperature/10 - 273;
        }

        /* Read Voltage */
        if(!BQ27441_read16(VOLTAGE, &battery_status.voltage, 1000))
        {
            battery_status.voltage = (~0);
        }

        /* Read Average Current */
        if(!BQ27441_read16(AVERAGE_CURRENT, &battery_status.avg_current, 1000))
        {
            battery_status.avg_current = (~0);
        }

        pubstatus = dmcf_pub_put(BATTERY_STATUS_MSG, (void *)&battery_status);

        vTaskDelayUntil( &xLastWaketime, xFrequency );
    }
    return NULL;
}
