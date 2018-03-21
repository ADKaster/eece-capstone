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
    TickType_t xFrequency = portTICK_PERIOD_MS * 100;
#endif
    struct timespec         currtime;
    //dmcf_pub_status_t       pubstatus;
    dmcf_sub_status_t       substatus;
    dmcf_nack_t             nack;

    PyroDriver_setTime(GPIO_PYRO_FIRST);
    PyroDriver_setTime(GPIO_PYRO_SECOND);
    PyroDriver_setTime(GPIO_PYRO_THIRD);
    PyroDriver_setTime(GPIO_PYRO_FOURTH);

    static uint32_t pubcount = 0;

    for(;;)
    {
        clock_gettime(CLOCK_REALTIME, &currtime);
        sprintf((void *)txBuffer, "FOO%lu", pubcount%1000);

        (void)dmcf_pub_put(BROADCAST_PING_MSG, (void *)txBuffer);
        substatus = dmcf_sub_get(BROADCAST_PING_MSG_2, (void *)rxBuffer, &nack);

        pthread_mutex_lock(&gDisplayMuxtex);

        if(SUB_SUCCESS == substatus)
        {
            rxBuffer[PING_MSG_LEN] = '\0';
            Display_printf(gTheDisplay, 0, 0, "Ping message received. Time %d.%d", currtime.tv_sec, currtime.tv_nsec);
            Display_printf(gTheDisplay, 0, 0, rxBuffer);
        }

        pthread_mutex_unlock(&gDisplayMuxtex);

        pubcount++;

#ifdef FREERTOS
        vTaskDelayUntil( &xLastWaketime, xFrequency );
#else
        usleep(10000);
#endif
    }
    return NULL;
}
