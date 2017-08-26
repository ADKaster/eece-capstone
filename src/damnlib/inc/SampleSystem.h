/**
 * \file 
 *
 * \brief System template configuration. 
 *
 * Use this file as a template for your file named DamnLibSystem.h. That file
 * Will contain the necessary configuration and message definitions for the library
 * to process your publish-subscribe things properly.
 *
 */

#ifndef SAMPLE_SYSTEM_H
#define SAMPLE_SYSTEM_H

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
} damn_node_t;

#define APPLICATION_WHOAMI (NODE_FOO)

/*! \brief Message enumeration

    Define an enumeration of all possible messages that may be seen on the bus. <br>
	Note that the type name must not be changed

*/
typedef enum msgenum_tag
{
    STANDARD_PING_MSG = 0, /**<  A simple ping message */
    /* Insert your message names here */
    /* MY_FAV_MSG_NAME, */

    NUM_MSG_DEFINITONS, /**< The total number of possible messages */
} damn_msg_enum_t;


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

#endif