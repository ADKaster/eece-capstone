/*! \file damn_msgdef.h

    Header file that defines all configuration parameters for the messaging library
*/

#ifndef DAMN_MSG_CONFIG_H
#define DAMN_MSG_CONFIG_H

#include <stdint.h>

#ifndef NOAPPLICATION 
    #include "DamnLibSystem.h"
    #ifndef APPLICATION_WHOAMI
        #error "Please define APPLICATION_WHOAMI as a valid damn_node_t value"
    #endif 
#else
    #include "SampleSystem.h"
#endif

/*! \brief Message transmission type
    
    Determines how a message will be sent over the common bus.
*/
typedef enum tsmit_tag
{
    TX_TYPE_P2P = 0,        /**< A point-to-point message that must be requested by the subscriber. */
    TX_TYPE_BROADCAST,      /**< A broadcast message that will be sent to all nodes. */
    NUM_TRANSMIT_TYPES,     /**< The total number of transmission types. */
} damn_tx_type_t;

/*! \brief Bus enumeration

    Defines the types of communications busses supported by the messaging library.
*/
typedef enum busenum_tag
{
    BUS_TYPE_I2C = 0,       /**< i2c bus */
    BUS_TYPE_SPI,           /**< unsupported */
    BUS_TYPE_UART,          /**< unsupported */
    BUS_TYPE_USB,           /**< unsupported */
    NUM_BUS_TYPES,          /**< The total number of bus types. */
} damn_bus_type_t;

/*! \brief Message definition structure

    This structure is used with the gobal array #gTheMessageDefinitions to define
    What messages are valid in the system and their properties.
*/
typedef struct msgdef_tag
{
    damn_msg_enum_t message_number;     /**< Which message is it? Using the enum type */
    damn_node_t message_source;          /**< What node is the source of this message? */
    damn_tx_type_t message_type;        /**< Point-to-point or broadcast */
    damn_bus_type_t message_bus;        /**< Only I2C is supported */
    uint32_t message_length;            /**< Message length in bytes */
} damn_msgdef_t;

extern damn_msgdef_t gTheMessageDefinitions[NUM_MSG_DEFINITONS];

#endif
