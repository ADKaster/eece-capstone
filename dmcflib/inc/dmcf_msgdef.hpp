/*
 * dmcf_msgdef.hpp
 *
 *  Created on: Feb 8, 2018
 *      Author: Andrew
 */

#ifndef INC_DMCF_MSGDEF_HPP_
#define INC_DMCF_MSGDEF_HPP_

#include <stdint.h>
#include <mqueue.h>
#include "DmcfSystem.hpp"

namespace DMCF
{

/*! \brief Message transmission type

    Determines how a message will be sent over the common bus.
*/
typedef enum tsmit_tag
{
    TX_TYPE_P2P = 0,        /**< A point-to-point message that must be requested by the subscriber. */
    TX_TYPE_BROADCAST,      /**< A broadcast message that will be sent to all nodes. */
    NUM_TRANSMIT_TYPES,     /**< The total number of transmission types. */
} tx_type_t;

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
} bus_type_t;

/*! \brief Message definition structure

    This structure is used with the gobal array #gTheMessageDefinitions to define
    What messages are valid in the system and their properties.
*/
typedef struct msgdef_tag
{
    msg_enum_t     message_number;     /**< Which message is it? Using the enum type */
    node_t         message_source;     /**< What node is the source of this message? */
    node_t         message_dest;       /**< Where is this message going? */
    tx_type_t      message_type;       /**< Point-to-point or broadcast */
    bus_type_t     message_bus;        /**< Only I2C is supported */
    uint32_t       message_length;     /**< Message length in bytes **** INCLUDES CHECKSUM at end of payload *****/
} msgdef_t;

extern msgdef_t gTheMessageDefinitions[NUM_MSG_DEFINITONS];

typedef enum nack_tag
{
    ACK = 0,
    NACK_BAD_CHECKSUM,
    NACK_INVLD_DEST,
    NACK_NO_MSG,
} nack_t;

/* \brief Packet definition

    This is the structure of the packet header that will actually be sent out on the bus <br>
    The checksum is the one's complement of the one's complement word-sum with the checksum field zero'd out.
    In this way the sum can be verified by comparing the sum that includes the checksum to zero.

    The packet will contain the header, followed by the payload and finally the checksum.
    header.msg_size will be the payload length + 4 bytes for the checksum
*/
typedef struct dmcf_pkthdr_s
{
    uint32_t            syncword;       /**< 0x5A5ACAFE */
    node_t         src;            /**< Enum of the source node */
    node_t         dest;           /**< Enum of the destination node */
    msg_enum_t     id;             /**< Packet id from dmcf_config.c */
    uint32_t            msg_size;       /**< Message size in *BYTES* */
    nack_t         ack;            /**< ACK/NACK Status */
    uint32_t            hdr_chksum;     /**< 32-bit version of IP header checksum */
}__attribute__((aligned(4))) pkthdr_t;



#define MSG_HDR_BYTES (sizeof(pkthdr_t))
#define MSG_HDR_WORDS (MSG_HDR_BYTES/4)
#define HDR_SYNCWORD (0x5A5ACAFEUL)


}



#endif /* INC_DMCF_MSGDEF_HPP_ */
