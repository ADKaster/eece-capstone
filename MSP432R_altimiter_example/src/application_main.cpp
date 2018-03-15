/* \file Example main thread



 */

#ifdef FREERTOS
#include <FreeRTOS.h>
#include <task.h>
#endif

#include <unistd.h>
#include <time.h>
#include "appdefs.h"
extern "C"
{
#include "dmcf_msgdef.h"
#include "dmcf_pubsub.h"
#include <ti/drivers/I2C.h>
}
#include <stdio.h>
#include <string.h>
#include "Adafruit_BMP280.h"
char txBuffer[PING_MSG_LEN];
char rxBuffer[PING_MSG_LEN];

void *mainThread(void *arg0)
{
#ifdef FREERTOS
    TickType_t xLastWaketime = xTaskGetTickCount();
    TickType_t xFrequency = portTICK_PERIOD_MS * 100;
#endif
    struct timespec currtime;
//    dmcf_pub_status_t       pubstatus;
//    dmcf_sub_status_t       substatus;
//    dmcf_nack_t             nack;

//    static uint32_t pubcount = 0;

    //init bmp object with valid I2C bus
    I2C_Params params;
    I2C_init();
    I2C_Params_init(&params);
    params.transferMode = I2C_MODE_BLOCKING;
    params.bitRate = I2C_100kHz;
    I2C_Handle i2cHandle = I2C_open(0, &params);

    Adafruit_BMP280 bmp(&i2cHandle);

    for (;;)
    {
        char printbuffer[256];

        clock_gettime(CLOCK_REALTIME, &currtime);
        sprintf(printbuffer, "Time = %ld\n", currtime.tv_sec);

        sprintf(printbuffer + strlen(printbuffer), "Temperature = %g*C\n", bmp.readTemperature());

        sprintf(printbuffer + strlen(printbuffer), "Pressure = %gPa\n", bmp.readPressure());

        sprintf(printbuffer + strlen(printbuffer), "Approx altitude = %gm\n", bmp.readAltitude(1013.25));  // this should be adjusted to your local forecast

        Display_printf(gTheDisplay, 0, 0, printbuffer);

//        sprintf(txBuffer, "FOO%lu", pubcount%1000);
//
//        (void)dmcf_pub_put(BROADCAST_PING_MSG, (void *)txBuffer);
//        substatus = dmcf_sub_get(BROADCAST_PING_MSG_2, (void *)rxBuffer, &nack);
//
//        pthread_mutex_lock(&gDisplayMuxtex);
//
//        if(SUB_SUCCESS == substatus)
//        {
//            rxBuffer[PING_MSG_LEN] = '\0';
//            Display_printf(gTheDisplay, 0, 0, const_cast<char *>("Ping message received. Time %d.%d"), currtime.tv_sec, currtime.tv_nsec);
//            Display_printf(gTheDisplay, 0, 0, rxBuffer);
//        }
//
//        pthread_mutex_unlock(&gDisplayMuxtex);
//
//        pubcount++;

#ifdef FREERTOS
        vTaskDelayUntil(&xLastWaketime, xFrequency);
#else
        usleep(10000);
#endif
    }
    return NULL ;
}
