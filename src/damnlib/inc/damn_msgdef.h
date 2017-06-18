#ifndef DAMN_MSG_CONFIG_H
#define DAMN_MSG_CONFIG_H

#include <stdint.h>

typedef enum msgenum_tag
{
    STANDARD_PING_MSG = 0,
    /* Insert your message names here */
    /* MY_FAV_MSG_NAME, */

    NUM_MSG_DEFINITONS,
} damn_msg_enum_t;

typedef enum tsmit_tag
{
    TX_TYPE_P2P = 0,
    TX_TYPE_BROADCAST,
    NUM_TRANSMIT_TYPES,
} damn_tx_type_t;

typedef struct msgdef_tag
{
    damn_msg_enum_t message_number;
    damn_tx_type_t message_type;
    uint32_t message_length; /* In bytes */
} damn_msgdef_t;

#define PING_MSG_LEN (8)

extern damn_msgdef_t gTheMessageDefinitions[NUM_MSG_DEFINITONS];

#endif