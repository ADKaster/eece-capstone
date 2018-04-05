/** \file System Configuration File
 *
 * DmcfLibSystem.h
 *
 *  Created on: Sep 7, 2017
 *      Author: Andrew Kaster
 */

#ifndef EXAMPLE_DMCFLIBSYSTEM_H_
#define EXAMPLE_DMCFLIBSYSTEM_H_

#include <time.h>

#ifndef NULL
#define NULL (void *)0
#endif

/*! \brief Enumeration of nodes in network
 *
 * Define an enumeration of which nodes exist in the network. <br>
 * Each application must define which node it is as well.
 *
 */

typedef enum nodeenum_tag
{
    NODE_PYRO,      /**< Pyro holds pyrotechnic triggers (LEDs for testing) */
    NODE_SENSOR,    /**< Altimeter BMP280, IMU BNO055 */
    NODE_DATA,      /**< Data contains SD card*/
    NODE_BATTERY,   /**< Battery contains main LiPo Battery */
    BROADCAST,      /**< MUST BE INCLUDED in this enum */
} dmcf_node_t;

#define NUM_NODES BROADCAST

extern dmcf_node_t currentApplication;
#define APPLICATION_WHOAMI currentApplication

/*! \brief enumeration of system slave addresses
 *
 *  Define an enumeration of slave addresses for each application/board
 *  See line 410 of MSP_EXP432P401R.c ** THESE MUST MATCH **
 *
 */
typedef enum slvaddr_tag
{
    SLAVEADDR_PYRO = 0x48,  /**< Address for Pyro */
    SLAVEADDR_SENSOR = 0x49,   /**< Address for sensors */
    SLAVEADDR_DATA = 0x50,   /**< Address for SD Card */
    SLAVEADDR_BATTERY = 0x51,  /**< Address for Batt */
} dmcf_slave_addr_t;

extern dmcf_slave_addr_t gTheSlaveAddresses[NUM_NODES];

/*! \brief enumeration of system master addresses
 *
 *  Define an enumeration of master addresses for each application/board
 *
 */
typedef enum masaddr_tag
{
    MASTERADDR_PYRO = 0x40,  /**< Address for Pyro */
    MASTERADDR_SENSOR = 0x41,  /**< Address for sensors */
    MASTERADDR_DATA = 0x42,  /**< Address for SD Card */
    MASTERADDR_BATTERY = 0x43, /**< Address for Batt */
} dmcf_master_addr_t;

extern dmcf_master_addr_t gTheMasterAddresses[NUM_NODES];

/*! \brief Message enumeration

    Define an enumeration of all possible messages that may be seen on the bus. <br>
    Note that the type name must not be changed

*/
typedef enum msgenum_tag
{
    BROADCAST_PING_MSG = 0, /**<  A simple ping message */

    /* Insert your message names here */
    PYRO_TRIGGER_MSG,
    ALTIMETER_STATUS_MSG,
    IMU_STATUS_MSG,
    PYRO_STATUS_MSG,
    BATTERY_STATUS_MSG,

    NUM_MSG_DEFINITONS, /**< The total number of possible messages. DO NOT REMOVE */
} dmcf_msg_enum_t;


/*! \brief Ping message length

    How long the ping message is, in bytes INCLUDES CHECKSUM (4 bytes).
*/
#define PING_MSG_LEN (12)

/*! \brief A simple ping message
 *
 * Simply contains an array of PING_MSG_LEN bytes that contains arbitrary values
 * to test communications.
 */
typedef struct ping_tag
{
    uint8_t ping_payload[PING_MSG_LEN]; /**< The payload array, contains debug values */
} ping_msg_t;


typedef struct pyro_trig_s
{
    struct timespec time;
    uint32_t trigger_val;
    uint32_t checksum;
} pyro_trigger_msg_t;

typedef struct alt_sts_s
{
    struct timespec time;
    float temp;
    float press;
    float alt;
    uint32_t checksum;
} alt_sts_msg_t;

typedef struct imu_sts_s
{
    struct timespec time;
    double accel[3];
    double gyro[3];
    double mag[3];
    uint8_t calibration[4]; // system, gyro, accel, mag
    uint32_t checksum;
} imu_sts_msg_t;

typedef struct pyro_sts_s
{
    struct timespec time;
    uint32_t max_current;
    uint32_t max_power;
    uint8_t enabled[4];
    uint32_t checksum;
} pyro_sts_msg_t;

#define WARN_LOW_BATT   (1 << 0)
#define WARN_HIGH_CURR  (1 << 1)
#define WARN_LOW_CURR   (1 << 2)
#define WARN_BLAH_BLAH  (1 << 3)

typedef struct bat_sts_s
{
    struct timespec time;
    uint32_t max_current;
    uint32_t max_power;
    uint32_t warnings;
    uint32_t checksum;
} battery_sts_msg_t;

#define BROADCAST_ADDRESS (0x00)

#endif /* EXAMPLE_DMCFLIBSYSTEM_H_ */
