/*! \file damn_msgdef.h

    Header file that defines all configuration parameters for the messaging library
*/

#ifndef DAMN_MSG_CONFIG_H
#define DAMN_MSG_CONFIG_H

#include <stdint.h>


/*! @brief Message enumeration

    Enumeration of all possible messages that may be seen on the bus.

*/
typedef enum msgenum_tag
{
    STANDARD_PING_MSG = 0, /**<  A simple ping message */
    /* Insert your message names here */
    /* MY_FAV_MSG_NAME, */

    NUM_MSG_DEFINITONS, /**< The total number of possible messages */
} damn_msg_enum_t;

/*! @brief Message transmission type
    
    Determines how a message will be sent over the common bus.
*/
typedef enum tsmit_tag
{
    TX_TYPE_P2P = 0,        /**< A point-to-point message that must be requested by the subscriber. */
    TX_TYPE_BROADCAST,      /**< A broadcast message that will be sent to all nodes. */
    NUM_TRANSMIT_TYPES,     /**< The total number of transmission types. */
} damn_tx_type_t;

/*! @brief Message definition structure

    This structure is used with the gobal array #gTheMessageDefinitions to define
    What messages are valid in the system and their properties.
*/
typedef struct msgdef_tag
{
    damn_msg_enum_t message_number;     /**< Which message is it? Using the enum type */
    damn_tx_type_t message_type;        /**< Point-to-point or broadcast */
    uint32_t message_length;            /**< Message length in bytes */
} damn_msgdef_t;

/*! @brief Ping message length
    
    How long the ping message is, in bytes. Default is 8 bytes.
*/
#define PING_MSG_LEN (8)

extern damn_msgdef_t gTheMessageDefinitions[NUM_MSG_DEFINITONS];

#endif
