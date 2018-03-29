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
#include "dmcf_debug.h"
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
    TickType_t xFrequency = portTICK_PERIOD_MS * 50;
#endif
    dmcf_pub_status_t       pubstatus;
    dmcf_sub_status_t       substatus;
    dmcf_nack_t             nack;

    static uint32_t pubcount = 0;

    //init bmp object with valid I2C bus
    I2C_Params params;
    I2C_init();
    I2C_Params_init(&params);
    params.transferMode = I2C_MODE_BLOCKING;
    params.bitRate = I2C_100kHz;
    I2C_Handle i2cHandle = I2C_open(0, &params);

    Adafruit_BMP280 bmp(&i2cHandle);
    bmp.begin();

    char printbuffer[256];

    alt_sts_msg_t altimeter_status = { 0, 0, 0, 0, 0 };
    pyro_sts_msg_t pyro_status;
    pyro_trigger_msg_t pyro_trigger;
    uint8_t pyroIdx = 0;

    for (;;)
    {

        clock_gettime(CLOCK_REALTIME, &altimeter_status.time);
        clock_gettime(CLOCK_REALTIME, &pyro_trigger.time);
        altimeter_status.temp = bmp.readTemperature();
        altimeter_status.press = bmp.readPressure();
        altimeter_status.alt = bmp.readAltitude(1013.25); //in hPa, equivalent to mb

        pubstatus = dmcf_pub_put(ALTIMETER_STATUS_MSG, (void *)&altimeter_status);
        dmcf_debugprintf("PubStatus: %d", pubstatus);

        substatus = dmcf_sub_get(PYRO_STATUS_MSG, (void *)&pyro_status, &nack);

        if(SUB_SUCCESS == substatus)
        {
            dmcf_debugprintf("Pyro Status Received!");
            sprintf(printbuffer, "Time = %ld\n", pyro_status.time.tv_sec);
        }

        pubcount++;

        if(pubcount%100 == 0)
        {
            pyro_trigger.trigger_val = pyroIdx++ & 0x3;
            pubstatus = dmcf_pub_put(PYRO_TRIGGER_MSG, (void *)&pyro_trigger);
        }

#ifdef FREERTOS
        vTaskDelayUntil(&xLastWaketime, xFrequency);
#else
        usleep(10000);
#endif
    }
    return NULL ;
}
