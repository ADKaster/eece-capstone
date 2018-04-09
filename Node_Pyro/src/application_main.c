/* \file Example main thread



 */

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
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <PyroDriver.hpp>
#include "PyroDriver_interface.hpp"

#ifdef __cplusplus
}
#endif

void *mainThread(void *arg0)
{
#ifdef FREERTOS
    TickType_t xLastWaketime = xTaskGetTickCount();
    TickType_t xFrequency = portTICK_PERIOD_MS * 50;
#endif

    //dmcf_pub_status_t       pubstatus;
    dmcf_sub_status_t       substatus;
    dmcf_nack_t             nack;

    time_t prev_sec_alt = 0;
    time_t prev_sec_imu = 0;
    time_t prev_sec_bat = 0;

    char printbuffer[256];
    pyro_sts_msg_t pyro_status = {0};
    alt_sts_msg_t altimeter_status;
    imu_sts_msg_t imu_status;
    battery_sts_msg_t battery_status;
    pyro_trigger_msg_t trig;

    for (;;)
    {
        clock_gettime(CLOCK_REALTIME, &pyro_status.time);

        pyro_status.max_current = 5;
        pyro_status.max_power = 5;

        pubstatus = dmcf_pub_put(PYRO_STATUS_MSG, (void *)&pyro_status);

#if 0
        substatus = dmcf_sub_get(ALTIMETER_STATUS_MSG, (void *)&altimeter_status, &nack);
        if(substatus == SUB_SUCCESS && altimeter_status.time.tv_sec != prev_sec_alt)
        {
            sprintf(printbuffer, "Time = %ld\n", altimeter_status.time.tv_sec);
            sprintf(printbuffer + strlen(printbuffer), "Temperature = %f *C\n", altimeter_status.temp);
            //sprintf(printbuffer + strlen(printbuffer), "Pressure = %f Pa\n", altimeter_status.press);
            //sprintf(printbuffer + strlen(printbuffer), "Approx altitude = %f m\n", altimeter_status.alt);  // this should be adjusted to your local forecast
            dmcf_debugprintf(printbuffer);
            prev_sec_alt = altimeter_status.time.tv_sec;
        }

        substatus = dmcf_sub_get(IMU_STATUS_MSG, (void *)&imu_status, &nack);
        if(substatus == SUB_SUCCESS && imu_status.time.tv_sec != prev_sec_imu)
        {
            sprintf(printbuffer, "Time = %ld\n", imu_status.time.tv_sec);
            sprintf(printbuffer + strlen(printbuffer), "Accel X = %f\n", imu_status.accel[0]);
            //sprintf(printbuffer + strlen(printbuffer), "Gyro Y = %f\n", imu_status.gyro[1]);
            //sprintf(printbuffer + strlen(printbuffer), "Mag Z = %f\n", imu_status.mag[2]);
            dmcf_debugprintf(printbuffer);
            prev_sec_imu = imu_status.time.tv_sec;
        }
        else if(imu_status.time.tv_sec != prev_sec_imu)
        {
            dmcf_debugprintf("IMU Sub status is %d", substatus);
            prev_sec_imu = imu_status.time.tv_sec;
        }

        substatus = dmcf_sub_get(BATTERY_STATUS_MSG, (void *)&battery_status, &nack);
        if(substatus == SUB_SUCCESS && battery_status.time.tv_sec != prev_sec_bat)
        {
            sprintf(printbuffer, "Time = %ld\n", battery_status.time.tv_sec);
            sprintf(printbuffer + strlen(printbuffer), "Capacity: %dmA\n", battery_status.remaining_capacity);
            //sprintf(printbuffer + strlen(printbuffer), "Status: %d pct\n", battery_status.state_of_charge);
            //sprintf(printbuffer + strlen(printbuffer), "Avg Curr: %d\n", battery_status.avg_current);
            dmcf_debugprintf(printbuffer);
            prev_sec_bat = battery_status.time.tv_sec;
        }
#endif
        substatus = dmcf_sub_get(PYRO_TRIGGER_MSG, (void *)&trig, &nack);
        if(substatus == SUB_SUCCESS)
        {
            if(trig.trigger_val < 4)
            {
                PyroDriver_activate(trig.trigger_val);
                pyro_status.enabled[trig.trigger_val] = 1;
            }
            else
            {
                dmcf_debugprintf("Error, invalid trigger value: %d", trig.trigger_val);
            }
        }

        vTaskDelayUntil( &xLastWaketime, xFrequency );
    }
    return NULL;
}
