/*  \file Message helper functions 



*/

#include "Board.h"
#include "damn_msgdef.h"
#include "messaging.h"

static uint32_t calculate_checksum(uint32_t *buf, uint32_t size);


static uint32_t calculate_checksum(uint32_t *buf, uint32_t size)
{   
    uint32_t i;
    uint32_t sum = 0;
    for(i = 0; i < size; i++)
    {
        sum+= buf[i];
    }

    return ~sum;
}

bool damn_msg_verify_header(damn_pkthdr_t *hdr)
{
    bool retVal = false;
    damn_msgdef_t msg_check;

    if ( (hdr->syncword == DAMN_HDR_SYNCWORD) &&
         (0 == calculate_checksum((uint32_t *)hdr, DAMN_MSG_HDR_WORDS)) &&
         (hdr->id < NUM_MSG_DEFINITONS) )
    {
        msg_check = gTheMessageDefinitions[hdr->id];

        if( (msg_check.message_source == hdr->src) &&
            (msg_check.message_dest == hdr->dest) &&
            (msg_check.message_length == hdr->msg_size))
        {
            retVal = true;
        }
    }

    return retVal;
}

void damn_msg_create_header(damn_pkthdr_t *hdr, damn_node_t src, damn_node_t dest, damn_msg_enum_t id, uint32_t msg_size)
{
    hdr->syncword = DAMN_HDR_SYNCWORD;
    hdr->src = src;
    hdr->dest = dest;
    hdr->id = id;
    hdr->msg_size = msg_size;
    hdr->hdr_chksum = 0;

    hdr->hdr_chksum = calculate_checksum((uint32_t *)hdr, DAMN_MSG_HDR_WORDS);
}