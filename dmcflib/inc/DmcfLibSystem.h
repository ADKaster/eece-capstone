/** \file System Configuration File
 *
 * DmcfLibSystem.h
 *
 *  Created on: Sep 7, 2017
 *      Author: Andrew Kaster
 */

#ifndef EXAMPLE_DMCFLIBSYSTEM_H_
#define EXAMPLE_DMCFLIBSYSTEM_H_


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
    NODE_FOO, /**< Foo contains feature x */
    NODE_BAR, /**< Bar contains feature y */
    NODE_BAZ, /**< Baz contains feature z */
    BROADCAST, /**< MUST BE INCLUDED in this enum */
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
    SLAVEADDR_FOO = 0x48, /**< Address for FOO */
    SLAVEADDR_BAR = 0x49, /**< Address for BAR */
    SLAVEADDR_BAZ = 0x50, /**< Address for BAZ */
} dmcf_slave_addr_t;

extern dmcf_slave_addr_t gTheSlaveAddresses[NUM_NODES];

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
} dmcf_master_addr_t;

extern dmcf_master_addr_t gTheMasterAddresses[NUM_NODES];

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
    NUM_MSG_DEFINITONS, /**< The total number of possible messages. DO NOT REMOVE */
} dmcf_msg_enum_t;


/*! \brief Ping message length

    How long the ping message is, in bytes.
*/
#define PING_MSG_LEN (8)

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

#endif /* EXAMPLE_DMCFLIBSYSTEM_H_ */
