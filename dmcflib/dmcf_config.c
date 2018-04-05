/*! \file dmcf_config.c

    Configuration file for the messaging library. <br>
    Should be the same file for all applications in your system.
*/


#include "dmcf_msgdef.h"

/*! \var gTheMessageDefinitions 
 *  \brief An array to hold all defined message definitions
 *
 * Each message to be used in a system must be described here so that the
 * message library can understand how to send and receive the message. Note
 * that the designated initializer format requires compiling with --std=c99 or
 * higher. <br>
 *
 * Be sure to fill in with your custom values for message number and message source.
 */
dmcf_msgdef_t gTheMessageDefinitions[NUM_MSG_DEFINITONS] =
{
    /* PING Message. For testing purposes */
    [BROADCAST_PING_MSG] =
    {
         .message_number =   BROADCAST_PING_MSG,
         .message_source =   NODE_PYRO,
         .message_dest =     BROADCAST,
         .message_type =     TX_TYPE_BROADCAST,
         .message_bus =      BUS_TYPE_I2C,
         .message_length =   sizeof(ping_msg_t),
    },
    [PYRO_TRIGGER_MSG] =
    {
         .message_number =   PYRO_TRIGGER_MSG,
         .message_source =   NODE_SENSOR,
         .message_dest =     BROADCAST,
         .message_type =     TX_TYPE_BROADCAST,
         .message_bus =      BUS_TYPE_I2C,
         .message_length =   sizeof(pyro_trigger_msg_t),
    },
    [ALTIMETER_STATUS_MSG] =
    {
         .message_number =   ALTIMETER_STATUS_MSG,
         .message_source =   NODE_SENSOR,
         .message_dest =     BROADCAST,
         .message_type =     TX_TYPE_BROADCAST,
         .message_bus =      BUS_TYPE_I2C,
         .message_length =   sizeof(alt_sts_msg_t),
    },
    [IMU_STATUS_MSG] =
    {
         .message_number =   IMU_STATUS_MSG,
         .message_source =   NODE_SENSOR,
         .message_dest =     BROADCAST,
         .message_type =     TX_TYPE_BROADCAST,
         .message_bus =      BUS_TYPE_I2C,
         .message_length =   sizeof(imu_sts_msg_t),
    },
    [PYRO_STATUS_MSG] =
    {
         .message_number =   PYRO_STATUS_MSG,
         .message_source =   NODE_PYRO,
         .message_dest =     BROADCAST,
         .message_type =     TX_TYPE_BROADCAST,
         .message_bus =      BUS_TYPE_I2C,
         .message_length =   sizeof(pyro_sts_msg_t),
    },
    [BATTERY_STATUS_MSG] =
    {
         .message_number =   BATTERY_STATUS_MSG,
         .message_source =   NODE_BATTERY,
         .message_dest =     BROADCAST,
         .message_type =     TX_TYPE_BROADCAST,
         .message_bus =      BUS_TYPE_I2C,
         .message_length =   sizeof(battery_sts_msg_t),
    },
    /* Insert your message definitions here         */
    /* [MY_FAV_MSG_NAME] =                          */
    /* {                                            */
    /*      .message_number =   MY_FAV_MSG_NAME,    */
    /*      .message_source =   NODE_MYFAVNODE      */
    /*      .message_dest =     NODE_OTHERNODE      */
    /*      .message_type =     TX_TYPE_BROADCAST,  */
    /*      .message_bus =      BUS_TYPE_I2C,       */
    /*      .message_length =   MY_FAV_MSG_LEN ,    */
    /* },                                           */

    
};

/*! \var gTheMasterAddresses
 *  \brief Array to hold all slave addresses of the system
 */
dmcf_master_addr_t gTheMasterAddresses[NUM_NODES] =
{
    MASTERADDR_PYRO,
    MASTERADDR_SENSOR,
    MASTERADDR_DATA,
    MASTERADDR_BATTERY,
};

/*! \var gTheSlaveAddresses
 *  \brief Array to hold all slave addresses of the system
 */
dmcf_slave_addr_t gTheSlaveAddresses[NUM_NODES] =
{
    SLAVEADDR_PYRO,
    SLAVEADDR_SENSOR,
    SLAVEADDR_DATA,
    SLAVEADDR_BATTERY,
};

