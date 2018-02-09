/*
 * DmcfSystem.h
 *
 *  Created on: Feb 8, 2018
 *      Author: Andrew
 */

#ifndef INC_DMCFSYSTEM_H_
#define INC_DMCFSYSTEM_H_

#include <stdint.h>

#ifdef __cplusplus
namespace DMCF
{
#endif

/*! \brief Enumeration of nodes in network
 *
 * Define an enumeration of which nodes exist in the network. <br>
 * Each application must define which node it is as well.
 *
 */
typedef enum nodeenum_tag
{
    NODE_FOO, /**< Foo contains feature x */
    NODE_BAR, /**< Bar contains feature y */
    NODE_BAZ, /**< Baz contains feature z */
    BROADCAST, /**< MUST BE INCLUDED in this enum */
} node_t;

#define NUM_NODES BROADCAST

/*! \brief enumeration of system slave addresses
 *
 *  Define an enumeration of slave addresses for each application/board
 *  See line 410 of MSP_EXP432P401R.c ** THESE MUST MATCH **
 *
 */
typedef enum slvaddr_tag
{
    SLAVEADDR_FOO = 0x48, /**< Address for FOO */
    SLAVEADDR_BAR = 0x49, /**< Address for BAR */
    SLAVEADDR_BAZ = 0x50, /**< Address for BAZ */
} slave_addr_t;

extern slave_addr_t gTheSlaveAddresses[NUM_NODES];

/*! \brief enumeration of system master addresses
 *
 *  Define an enumeration of master addresses for each application/board
 *
 */
typedef enum masaddr_tag
{
    MASTERADDR_FOO = 0x40, /**< Address for FOO */
    MASTERADDR_BAR = 0x41, /**< Address for BAR */
    MASTERADDR_BAZ = 0x42, /**< Address for BAZ */
} master_addr_t;

extern master_addr_t gTheMasterAddresses[NUM_NODES];

/*! \brief Message enumeration

    Define an enumeration of all possible messages that may be seen on the bus. <br>
    Note that the type name must not be changed

*/
typedef enum msgenum_tag
{
    STANDARD_PING_MSG = 0, /**<  A simple ping message */
    /* Insert your message names here */
    /* MY_FAV_MSG_NAME, */
    BROADCAST_PING_MSG,
    BROADCAST_PING_MSG_2,
    NUM_MSG_DEFINITONS, /**< The total number of possible messages. DO NOT REMOVE */
} msg_enum_t;


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
} sample_ping_msg_t;


#define BROADCAST_ADDRESS (0x00)

#ifdef __cplusplus
} /* namespace */
#endif

#endif /* INC_DMCFSYSTEM_H_ */
