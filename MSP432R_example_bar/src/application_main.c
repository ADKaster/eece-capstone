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

static uint8_t txBuffer[PING_MSG_LEN];
static uint8_t rxBuffer[PING_MSG_LEN];

void *mainThread(void *arg0)
{
#ifdef FREERTOS
    TickType_t xLastWaketime = xTaskGetTickCount();
    TickType_t xFrequency = portTICK_PERIOD_MS * 10;
#endif
    struct timespec currtime;
    //dmcf_pub_status_t       pubstatus;
    dmcf_sub_status_t       substatus;
    dmcf_nack_t             nack;

    sprintf((void *)txBuffer, "ImNode%d", APPLICATION_WHOAMI);

    for(;;)
    {
        clock_gettime(CLOCK_REALTIME, &currtime);

        if(NODE_FOO == currentApplication)
        {
            (void)dmcf_pub_put(BROADCAST_PING_MSG, (void *)txBuffer);
            //substatus = dmcf_sub_get(STANDARD_PING_MSG, (void *)rxBuffer, &nack);
        }
        else
        {
            //(void)dmcf_pub_put(STANDARD_PING_MSG, (void *)txBuffer);
            substatus = dmcf_sub_get(BROADCAST_PING_MSG, (void *)rxBuffer, &nack);
        }

        pthread_mutex_lock(&gDisplayMuxtex);

        if(SUB_SUCCESS == substatus)
        {
            rxBuffer[PING_MSG_LEN] = '\0';
            Display_printf(gTheDisplay, 0, 0, "Ping message received: %s\n", rxBuffer);
        }

        if(0 == currtime.tv_nsec)
        {
            Display_printf(gTheDisplay, 0, 0, "Hello from main: Time: %dsec, %d ns\n", currtime.tv_sec, currtime.tv_nsec);
        }

        pthread_mutex_unlock(&gDisplayMuxtex);


#ifdef FREERTOS
        vTaskDelayUntil( &xLastWaketime, xFrequency );
#else
        usleep(10000);
#endif
    }
    return NULL;
}
