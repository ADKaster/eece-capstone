/* \file Example main thread



 */

#ifdef FREERTOS
    #include <FreeRTOS.h>
    #include <task.h>
#endif

#include <unistd.h>
#include <time.h>
#include "appdefs.h"
#include "dmcf.hpp"
#include "dmcf_msgdef.hpp"
#include "dmcf_pubsub.hpp"
#include <stdio.h>
#include <stddef.h>

char txBuffer[PING_MSG_LEN];
char rxBuffer[PING_MSG_LEN];

void *mainThread(void *arg0)
{
#ifdef FREERTOS
    TickType_t xLastWaketime = xTaskGetTickCount();
    TickType_t xFrequency = portTICK_PERIOD_MS * 100;
#endif
    struct timespec         currtime;
    //DMCF::dmcf_pub_status_t       pubstatus;
    DMCF::sub_status_t       substatus;
    DMCF::nack_t             nack;

    static uint32_t pubcount = 0;

    for(;;)
    {
        clock_gettime(CLOCK_REALTIME, &currtime);
        sprintf(txBuffer, "FOO%lu", pubcount%1000);

        (void)dmcf_obj.pub_put(DMCF::BROADCAST_PING_MSG, (void *)txBuffer);
        substatus = dmcf_obj.sub_get(DMCF::BROADCAST_PING_MSG_2, (void *)rxBuffer, &nack);

        pthread_mutex_lock(&gDisplayMuxtex);

        if(DMCF::SUB_SUCCESS == substatus)
        {
            rxBuffer[PING_MSG_LEN] = '\0';
            Display_printf(gTheDisplay, 0, 0, const_cast<char *>("Ping message received. Time %d.%d"), currtime.tv_sec, currtime.tv_nsec);
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
    return nullptr;
}
