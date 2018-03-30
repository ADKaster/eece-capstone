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
#include "dmcf_msgdef.h"
#include "dmcf_pubsub.h"
#include "dmcf_debug.h"
#include <stdio.h>
#include "HAL_BQ27441.h"

void *mainThread(void *arg0)
{
#ifdef FREERTOS
    TickType_t xLastWaketime = xTaskGetTickCount();
    TickType_t xFrequency = portTICK_PERIOD_MS * 1000;
#endif
   struct timespec currtime;
    //dmcf_pub_status_t       pubstatus;
//    dmcf_sub_status_t       substatus;
//    dmcf_nack_t             nack;

    static uint32_t pubcount = 0;

    dmcf_debugprintf("TEST TEST TEST");

    if (!BQ27441_initConfig())
    {
        dmcf_debugprintf("Error initializing BQ27441 Config");
        dmcf_debugprintf("Make sure BOOSTXL-BATPAKMKII is connected and switch is flipped to \"CONNECTED\"");
    }

    while (!BQ27441_initOpConfig())
    {
        dmcf_debugprintf("Clearing BIE in Operation Configuration");
        usleep(200000);
    }

    BQ27441_control(BAT_INSERT, 1000);

    short result16 = 0;
    char str[64];

    for(;;)
    {
        clock_gettime(CLOCK_REALTIME, &currtime);

         dmcf_debugprintf("");
         dmcf_debugprintf("");
         dmcf_debugprintf("*************************************");
         dmcf_debugprintf("Battery Information");
         dmcf_debugprintf("*************************************");

         /* Read Design Capacity */
         if(!BQ27441_read16(DESIGN_CAPACITY, &result16, 1000))
             dmcf_debugprintf("Error Reading Design Capacity ");
         else
         {
             sprintf(str, "Design Capacity: %dmAh", result16);
             dmcf_debugprintf(str);
         }

         /* Read Remaining Capacity */
         if(!BQ27441_read16(REMAINING_CAPACITY, &result16, 1000))
             dmcf_debugprintf("Error Reading Remaining Capacity ");
         else
         {
             sprintf(str, "Remaining Capacity: %dmAh", result16);
             dmcf_debugprintf(str);
         }

         /* Read State Of Charge */
         if(!BQ27441_read16(STATE_OF_CHARGE, &result16, 1000))
             dmcf_debugprintf("Error Reading State Of Charge ");
         else
         {
             sprintf(str, "State of Charge: %d%%", (unsigned short)result16);
             dmcf_debugprintf(str);
         }

         /* Read Temperature */
         if(!BQ27441_read16(TEMPERATURE, &result16, 1000))
             dmcf_debugprintf("Error Reading Temperature ");
         else
         {
             sprintf(str, "Temperature: %dC", result16/10 - 273);
             dmcf_debugprintf(str);
         }

         /* Read Voltage */
         if(!BQ27441_read16(VOLTAGE, &result16, 1000))
             dmcf_debugprintf("Error Reading Voltage ");
         else
         {
             sprintf(str, "Voltage: %dmV", result16);
             dmcf_debugprintf(str);
         }

         /* Read Average Current */
         if(!BQ27441_read16(AVERAGE_CURRENT, &result16, 1000))
             dmcf_debugprintf("Error Reading Average Current ");
         else
         {
             sprintf(str, "Average Current: %dmA", result16);
             dmcf_debugprintf(str);
             if (result16 > 0) {
                 dmcf_debugprintf("Status : charging");
             } else {
                 dmcf_debugprintf("Status : discharging");
             }
         }

#ifdef FREERTOS
        vTaskDelayUntil( &xLastWaketime, xFrequency );
#else
        usleep(10000);
#endif
    }
    return NULL;
}
