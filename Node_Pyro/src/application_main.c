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

char txBuffer[PING_MSG_LEN];
char rxBuffer[PING_MSG_LEN];

void *mainThread(void *arg0)
{
#ifdef FREERTOS
    TickType_t xLastWaketime = xTaskGetTickCount();
    TickType_t xFrequency = portTICK_PERIOD_MS * 50;
#endif

    dmcf_pub_status_t       pubstatus;
    dmcf_sub_status_t       substatus;
    dmcf_nack_t             nack;

    char printbuffer[256];
    pyro_sts_msg_t pyro_status = {0};
    alt_sts_msg_t altimeter_status;
    pyro_trigger_msg_t trig;

    for (;;)
    {
        clock_gettime(CLOCK_REALTIME, &pyro_status.time);

        pyro_status.max_current = 5;
        pyro_status.max_power = 5;

        pubstatus = dmcf_pub_put(PYRO_STATUS_MSG, (void *)&pyro_status);

        substatus = dmcf_sub_get(ALTIMETER_STATUS_MSG, (void *)&altimeter_status, &nack);
        if(substatus == SUB_SUCCESS)
        {
            sprintf(printbuffer, "Time = %ld\n", altimeter_status.time.tv_sec);
            sprintf(printbuffer + strlen(printbuffer), "Temperature = %f *C\n", altimeter_status.temp);
            sprintf(printbuffer + strlen(printbuffer), "Pressure = %f Pa\n", altimeter_status.press);
            sprintf(printbuffer + strlen(printbuffer), "Approx altitude = %f m\n", altimeter_status.alt);  // this should be adjusted to your local forecast
            dmcf_debugprintf(printbuffer);
        }

        substatus = dmcf_sub_get(PYRO_TRIGGER_MSG, (void *)&trig, &nack);
        if(substatus == SUB_SUCCESS)
        {
            if(trig.trigger_val < 4)
                PyroDriver_activate(trig.trigger_val);
            else
                dmcf_debugprintf("Error, invalid trigger value: %d", trig.trigger_val);
        }

        vTaskDelayUntil( &xLastWaketime, xFrequency );
    }
    return NULL;
}
