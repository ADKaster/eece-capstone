/*! \file dmcf_config.cpp

    Configuration file for the messaging library. <br>
    Should be the same file for all applications in your system.
*/


#include "DmcfSystem.h"
#include "dmcf.hpp"
#include "dmcf_msgdef.hpp"
#include "dmcf_i2c_internals.hpp"

namespace DMCF
{

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
msgdef_t gTheMessageDefinitions[NUM_MSG_DEFINITONS] =
{
    /* PING Message. For testing purposes */
    [STANDARD_PING_MSG] = 
    {
        .message_number =   STANDARD_PING_MSG,
        .message_source =   NODE_FOO,
        .message_dest =     NODE_BAR,
        .message_type =     TX_TYPE_P2P,
        .message_bus =      BUS_TYPE_I2C,
        .message_length =   PING_MSG_LEN,
    },
    [BROADCAST_PING_MSG] =
    {
         .message_number =   BROADCAST_PING_MSG,
         .message_source =   NODE_FOO,
         .message_dest =     BROADCAST,
         .message_type =     TX_TYPE_BROADCAST,
         .message_bus =      BUS_TYPE_I2C,
         .message_length =   PING_MSG_LEN,
    },
    [BROADCAST_PING_MSG_2] =
        {
             .message_number =   BROADCAST_PING_MSG_2,
             .message_source =   NODE_BAR,
             .message_dest =     BROADCAST,
             .message_type =     TX_TYPE_BROADCAST,
             .message_bus =      BUS_TYPE_I2C,
             .message_length =   PING_MSG_LEN,
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
master_addr_t gTheMasterAddresses[NUM_NODES] =
{
    MASTERADDR_FOO,
    MASTERADDR_BAR,
    MASTERADDR_BAZ
};

/*! \var gTheSlaveAddresses
 *  \brief Array to hold all slave addresses of the system
 */
slave_addr_t gTheSlaveAddresses[NUM_NODES] =
{
    SLAVEADDR_FOO,
    SLAVEADDR_BAR,
    SLAVEADDR_BAZ
};

} /* namespace DMCF */
