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
#include <stdio.h>

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
    TickType_t xFrequency = portTICK_PERIOD_MS * 5000;
#endif

    for(;;)
    {
        int i;
        for(i = 0; i < 4; i++)
        {
            PyroDriver_activate(i);

#ifdef FREERTOS
            vTaskDelayUntil( &xLastWaketime, xFrequency );
#else
            usleep(10000);
#endif
        };
    }
    return NULL;
}
