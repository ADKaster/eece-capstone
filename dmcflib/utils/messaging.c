/*  \file Message helper functions 



*/

#include "dmcf_msgdef.h"
#include "messaging.h"

uint32_t dmcf_calculate_checksum(uint32_t *buf, uint32_t size)
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


bool dmcf_msg_verify_header(dmcf_pkthdr_t *hdr)
{
    bool retVal = false;
    dmcf_msgdef_t *msg_check;

    if ( (hdr->syncword == DMCF_HDR_SYNCWORD) &&
         (0 == dmcf_calculate_checksum((uint32_t *)hdr, DMCF_MSG_HDR_WORDS)) &&
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
bool dmcf_msg_verify_msg(uint32_t *buf, uint32_t len)
{
    bool retVal = false;

    if( 0 == dmcf_calculate_checksum(buf, len))
    {
        retVal = true;
    }

    return retVal;
}

void dmcf_msg_create_header(dmcf_pkthdr_t *hdr, dmcf_node_t src, dmcf_node_t dest, dmcf_msg_enum_t id, uint32_t msg_size)
{
    hdr->syncword = DMCF_HDR_SYNCWORD;
    hdr->src = src;
    hdr->dest = dest;
    hdr->id = id;
    hdr->msg_size = msg_size;
    hdr->ack = ACK;
    hdr->hdr_chksum = 0;

    hdr->hdr_chksum = dmcf_calculate_checksum((uint32_t *)hdr, DMCF_MSG_HDR_WORDS);
}

