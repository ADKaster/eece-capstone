
#include "damn_msgdef.h"


/* Designated intializer format requires compiling with --std=c99 or higher */
damn_msgdef_t gTheMessageDefinitions[NUM_MSG_DEFINITONS] =
{
    [STANDARD_PING_MSG] = 
    {
        .message_number =   STANDARD_PING_MSG,
        .message_type =     TX_TYPE_P2P,
        .message_length =   PING_MSG_LEN,
    },
    /* Insert your message definitions here         */
    /* [MY_FAV_MSG_NAME] =                          */
    /* {                                            */
    /*      .message_number =   MY_FAV_MSG_NAME,    */
    /*      .message_type =     TX_TYPE_BROADCAST,  */
    /*      .message_length =   MY_FAV_MSG_LEN ,    */
    /* },                                           */
    
};




