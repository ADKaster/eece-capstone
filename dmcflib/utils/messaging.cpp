/*
 * messaging.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: Andrew
 */

#include "dmcf.hpp"
#include "dmcf_msgdef.hpp"

namespace DMCF
{

uint32_t dmcf::calculate_checksum(uint32_t *buf, uint32_t size)
{
    uint32_t i;
    uint32_t sum = 0;

    if(0 == size)
    {
        sum = -1;
    }

    for(i = 0; i < size; i++)
    {
        sum += buf[i];
    }

    return ~sum;
}


bool dmcf::msg_verify_header(pkthdr_t *hdr)
{
    bool retVal = false;
    msgdef_t *msg_check;

    if ( (hdr->syncword == HDR_SYNCWORD) &&
         (0 == calculate_checksum((uint32_t *)hdr, MSG_HDR_WORDS)) &&
         (hdr->id < NUM_MSG_DEFINITONS) )
    {
        msg_check = &(gTheMessageDefinitions[hdr->id]);

        if( (msg_check->message_source == hdr->src) &&
            (msg_check->message_dest == hdr->dest) &&
            (msg_check->message_length == hdr->msg_size))
        {
            retVal = true;
        }
    }

    return retVal;
}

/* LEN IN WORDS */
bool dmcf::msg_verify_msg(uint32_t *buf, uint32_t len)
{
    bool retVal = false;
    if(!buf)
    {
        retVal = false;
    }
    else if( 0 == calculate_checksum(buf, len))
    {
        retVal = true;
    }

    return retVal;
}

void dmcf::msg_create_header(pkthdr_t *hdr, node_t src, node_t dest, msg_enum_t id, uint32_t msg_size)
{
    hdr->syncword = HDR_SYNCWORD;
    hdr->src = src;
    hdr->dest = dest;
    hdr->id = id;
    hdr->msg_size = msg_size;
    hdr->ack = ACK;
    hdr->hdr_chksum = 0;

    hdr->hdr_chksum = calculate_checksum((uint32_t *)hdr, MSG_HDR_WORDS);
}

} /* namespace DMCF */
