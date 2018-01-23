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
         .message_dest =     NODE_BAR,
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
dmcf_master_addr_t gTheMasterAddresses[NUM_NODES] =
{
    MASTERADDR_FOO,
    MASTERADDR_BAR,
    MASTERADDR_BAZ
};

/*! \var gTheSlaveAddresses
 *  \brief Array to hold all slave addresses of the system
 */
dmcf_slave_addr_t gTheSlaveAddresses[NUM_NODES] =
{
    SLAVEADDR_FOO,
    SLAVEADDR_BAR,
    SLAVEADDR_BAZ
};

