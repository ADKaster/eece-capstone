/*
 * BNO055Task.cpp
 *
 *  Created on: Feb 11, 2018
 *      Author: Andrew
 */


#include <ti/drivers/I2C.h>
#include <stdint.h>
#include "inc/Adafruit_BNO055.h"

#include "inc/Board.h"
#include "inc/IMUTask.h"

extern "C" {
#include "dmcf_debug.h"
#include "dmcf_pubsub.h"
#include <unistd.h>
#include <FreeRTOS.h>
#include <task.h>
}

#define IMU_I2C Board_I2C0

I2C_Handle imuI2CHandle;
Adafruit_BNO055 bno;

bool init_imu_task(void)
{
    bool ret = false;
    I2C_Params masterParams;

    I2C_init();

    I2C_Params_init(&masterParams);
    masterParams.transferMode = I2C_MODE_BLOCKING;
    masterParams.bitRate = I2C_100kHz;
    imuI2CHandle = I2C_open(IMU_I2C, &masterParams);


    if(imuI2CHandle)
    {
        bno.setBus(imuI2CHandle);
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


void *IMUTask(void *arg0)
{
    if(!init_imu_task())
        while(1);

    dmcf_pub_status_t pubstatus;
    imu_sts_msg_t imu_status = {0};
    // calibrations
    uint8_t *system = &imu_status.calibration[0];
    uint8_t *gyro = &imu_status.calibration[1];
    uint8_t *accel = &imu_status.calibration[2];
    uint8_t *mag = &imu_status.calibration[3];

    TickType_t xLastWaketime = xTaskGetTickCount();
    TickType_t xFrequency = portTICK_PERIOD_MS * 100;

    for(;;)
    {
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

        vTaskDelayUntil(&xLastWaketime, xFrequency);
    }

    return NULL;

}
