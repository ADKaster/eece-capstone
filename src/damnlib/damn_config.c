/*! \file damn_config.c

    Configuration file for the messaging library. <br>
    Should be the same file for all applications in your system.
*/


#include "damn_msgdef.h"

/*! \var gTheMessageDefinitions 
 *  \brief An array to hold all defined message definitions
 *
 * Each message to be used in a system must be described here so that the
 * message library can understand how to send and recieve the message. Note
 * that the designated intializer format requires compiling with --std=c99 or
 * higher. <br>
 *
 * Be sure to fill in with your custom values for message number and message source.
 */
damn_msgdef_t gTheMessageDefinitions[NUM_MSG_DEFINITONS] =
{
    [STANDARD_PING_MSG] = 
    {
        .message_number =   STANDARD_PING_MSG,
        .message_source =   NODE_FOO,
        .message_type =     TX_TYPE_P2P,
        .message_bus =      BUS_TYPE_I2C,
        .message_length =   PING_MSG_LEN,
    },
    /* Insert your message definitions here         */
    /* [MY_FAV_MSG_NAME] =                          */
    /* {                                            */
    /*      .message_number =   MY_FAV_MSG_NAME,    */
    /*      .message_source =   NODE_MYFAVNODE      */
    /*      .message_type =     TX_TYPE_BROADCAST,  */
    /*      .message_bus =      BUS_TYPE_I2C,       */
    /*      .message_length =   MY_FAV_MSG_LEN ,    */
    /* },                                           */
    
};




