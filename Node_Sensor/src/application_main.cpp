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
#include "Adafruit_BNO055.h"

Adafruit_BNO055 bno;
Adafruit_BMP280 bmp;
I2C_Handle sensorHandle;

bool init_altimeter(void);
bool init_imu(void);
void run_altimeter(void);
void run_imu(void);

void *mainThread(void *arg0)
{
    TickType_t xLastWaketime = xTaskGetTickCount();
    TickType_t xFrequency = portTICK_PERIOD_MS * 50;

    bool alt_good = true;
    bool imu_good = true;

    if(!init_altimeter())
    {
        dmcf_debugprintf(const_cast<char *>("Failed to initialize altimeter!"));
        alt_good = false;
    }

    if(!init_imu())
    {
        dmcf_debugprintf(const_cast<char *>("Failed to initialize imu!"));
        imu_good = false;
    }

    for (;;)
    {

        if(alt_good)
            run_altimeter();

        if(imu_good)
            run_imu();

        vTaskDelayUntil(&xLastWaketime, xFrequency);
    }
    return NULL ;
}

bool init_altimeter(void)
{
    //init bmp object with valid I2C bus
    I2C_Params params;
    I2C_init();
    I2C_Params_init(&params);
    params.transferMode = I2C_MODE_BLOCKING;
    params.bitRate = I2C_100kHz;
    sensorHandle = I2C_open(0, &params);

    bmp.setBus(&sensorHandle);
    return bmp.begin();
}

bool init_imu(void)
{
    bool ret = false;

    if(sensorHandle)
    {
        bno.setBus(sensorHandle);
        ret = bno.begin();
        if(ret)
        {
            usleep(1000000);
            dmcf_debugprintf(const_cast<char *>("temperature: %d C"), bno.getTemp());
            bno.setExtCrystalUse(true);
        }
    }

    return ret;

}

void run_altimeter(void)
{
    static char printbuffer[256];
    static uint32_t pubcount = 0;

    alt_sts_msg_t altimeter_status = { 0, 0, 0, 0, 0 };
    pyro_sts_msg_t pyro_status;
    pyro_trigger_msg_t pyro_trigger;
    uint8_t pyroIdx = 0;
    time_t prev_tv_sec;
    dmcf_pub_status_t       pubstatus;
    dmcf_sub_status_t       substatus;
    dmcf_nack_t             nack;

    clock_gettime(CLOCK_REALTIME, &altimeter_status.time);
    clock_gettime(CLOCK_REALTIME, &pyro_trigger.time);
    altimeter_status.temp = bmp.readTemperature();
    altimeter_status.press = bmp.readPressure();
    altimeter_status.alt = bmp.readAltitude(1013.25); //in hPa, equivalent to mb

    pubstatus = dmcf_pub_put(ALTIMETER_STATUS_MSG, (void *)&altimeter_status);

    substatus = dmcf_sub_get(PYRO_STATUS_MSG, (void *)&pyro_status, &nack);

    if(SUB_SUCCESS == substatus && pyro_status.time.tv_sec != prev_tv_sec)
    {
        dmcf_debugprintf(const_cast<char *>("Pyro Status Received!"));
        sprintf(printbuffer, "Time = %ld\n", pyro_status.time.tv_sec);
        sprintf(printbuffer + strlen(printbuffer), "Pyro Status: max_I %lu max_P %lu", pyro_status.max_current, pyro_status.max_power);
        sprintf(printbuffer + strlen(printbuffer), "Enabled pyros: %d", pyro_status.enabled[0] + pyro_status.enabled[1] * 10 + pyro_status.enabled[2] * 100 + pyro_status.enabled[3] * 1000);
        dmcf_debugprintf(printbuffer);
        prev_tv_sec = pyro_status.time.tv_sec;
    }

    pubcount++;

    if(pubcount%100 == 0)
    {
        pyro_trigger.trigger_val = pyroIdx++;
        if(pyroIdx > 3) pyroIdx = 0;
        pubstatus = dmcf_pub_put(PYRO_TRIGGER_MSG, (void *)&pyro_trigger);
    }

}

void run_imu(void)
{
    dmcf_pub_status_t pubstatus;
    imu_sts_msg_t imu_status = {0};
    // calibrations
    uint8_t *system = &imu_status.calibration[0];
    uint8_t *gyro = &imu_status.calibration[1];
    uint8_t *accel = &imu_status.calibration[2];
    uint8_t *mag = &imu_status.calibration[3];

    clock_gettime(CLOCK_REALTIME, &imu_status.time);

    // Possible vector values can be:
    // - VECTOR_ACCELEROMETER - m/s^2
    // - VECTOR_MAGNETOMETER  - uT
    // - VECTOR_GYROSCOPE     - rad/s
    // - VECTOR_EULER         - degrees
    // - VECTOR_LINEARACCEL   - m/s^2
    // - VECTOR_GRAVITY       - m/s^2

    imu::Vector<3> val = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);

    for(int i = 0; i < 3; i++)
        imu_status.accel[i] = val[i];

    dmcf_debugprintf(const_cast<char *>("X: %f Y: %f Z: %f"), val.x(), val.y(), val.z());

    val = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);

    for(int i = 0; i < 3; i++)
        imu_status.gyro[i] = val[i];

    val = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

    for(int i = 0; i < 3; i++)
        imu_status.mag[i] = val[i];

    /* Get calibration status for each sensor. */
    bno.getCalibration(system, gyro, accel, mag);

    dmcf_debugprintf(const_cast<char *>("CALIBRATION: Sys=%d Gyro=%d Accel=%d Mag=%d"), *system, *gyro, *accel, *mag);

    pubstatus = dmcf_pub_put(IMU_STATUS_MSG, (void *)&imu_status);
    if(pubstatus != PUB_SUCCESS)
        dmcf_debugprintf(const_cast<char *>("Pubstatus: %d"), pubstatus);


}
