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
#include <stdbool.h>

bool timespec_compare(struct timespec *start, struct timespec *stop,
                   int32_t milliseconds);

void *mainThread(void *arg0)
{
#ifdef FREERTOS
    TickType_t xLastWaketime = xTaskGetTickCount();
    TickType_t xFrequency = portTICK_PERIOD_MS * 50;
#endif

    //dmcf_pub_status_t       pubstatus;
    dmcf_sub_status_t       substatus;
    dmcf_nack_t             nack;

    char printbuffer[1024];
    alt_sts_msg_t altimeter_status;
    imu_sts_msg_t imu_status;
    battery_sts_msg_t battery_status;
    struct timespec currtime;
    struct timespec prevtime;

    clock_gettime(CLOCK_REALTIME, &prevtime);

    for (;;)
    {

        clock_gettime(CLOCK_REALTIME, &currtime);

        substatus = dmcf_sub_get(ALTIMETER_STATUS_MSG, (void *)&altimeter_status, &nack);

        substatus = dmcf_sub_get(IMU_STATUS_MSG, (void *)&imu_status, &nack);

       // dmcf_debugprintf("Substatus: %d", substatus);

        substatus = dmcf_sub_get(BATTERY_STATUS_MSG, (void *)&battery_status, &nack);

        if(timespec_compare(&prevtime, &currtime, 200))
        {
            sprintf(printbuffer, "ALT,%ld,%ld,%f\r", currtime.tv_sec, currtime.tv_nsec, altimeter_status.alt);  // this should be adjusted to your local forecast
            dmcf_debugprintf(printbuffer);
            sprintf(printbuffer, "ACCEL,%ld,%ld,%f,%f,%f\r", currtime.tv_sec, currtime.tv_nsec, imu_status.accel[0], imu_status.accel[1], imu_status.accel[2]);
            dmcf_debugprintf(printbuffer);
            //sprintf(printbuffer, "BATT,%d,%d,%d\r", currtime.tv_sec, currtime.tv_nsec, battery_status.remaining_capacity);
            //dmcf_debugprintf(printbuffer);
            prevtime.tv_sec = currtime.tv_sec;
            prevtime.tv_nsec = currtime.tv_nsec;
        }

        vTaskDelayUntil( &xLastWaketime, xFrequency );
    }
    return NULL;
}

bool timespec_compare(struct timespec *start, struct timespec *stop,
                   int32_t milliseconds)
{
    struct timespec result;
    bool ret = false;

    /* store difference */
    if ((stop->tv_nsec - start->tv_nsec) < 0) {
        result.tv_sec = stop->tv_sec - start->tv_sec - 1;
        result.tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    } else {
        result.tv_sec = stop->tv_sec - start->tv_sec;
        result.tv_nsec = stop->tv_nsec - start->tv_nsec;
    }

    /* compare to input */
    if((1000*result.tv_sec + (result.tv_nsec)/1000000) - milliseconds > 0)
    {
        ret = true;
    }

    return ret;
}
