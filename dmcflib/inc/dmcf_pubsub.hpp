/*
 * dmcf_pubsub.hpp
 *
 *  Created on: Feb 8, 2018
 *      Author: Andrew
 */

#ifndef INC_DMCF_PUBSUB_HPP_
#define INC_DMCF_PUBSUB_HPP_

#include "dmcf_msgdef.hpp"
#include <stdint.h>
#include <stdbool.h>
#include <ti/drivers/I2C.h>

namespace DMCF
{

#define MAX_QUEUE_DEPTH (20) /* Needs testing. 20 seems reasonable maximum though */

typedef enum pub_status_tag
{
    PUB_FAIL,
    PUB_SUCCESS,
    PUB_INVALID,
    PUB_FULL,
} pub_status_t;

typedef enum sub_status_tag
{
    SUB_ERR,
    SUB_SUCCESS,
    SUB_FAIL,
    SUB_INVALID,
    SUB_NONE
} sub_status_t;

typedef enum
{
    FREQ_UNLIMITED,
    FREQ_TEN_HZ,
    FREQ_TWENTY_HZ,
    FREQ_ONE_HUNDRED_HZ,
} pubsub_freq_t;

typedef enum pubsubstat_tag
{
    STATUS_IGNORE = 0,
    STATUS_SUBSCRIBED,
    STATUS_PUBLISHING
} pubsub_status_t;

typedef struct subinfo_tag
{
    msgdef_t               *pmsg_def;
    uint32_t                period_ms;
    mqd_t                   queue;
    uint32_t                q_width;
    uint32_t                q_depth;
    pubsub_status_t         status;
} subinfo_t, *subinfo_ptr;

typedef subinfo_t pubinfo_t;
typedef subinfo_ptr pubinfo_ptr;

typedef struct p2pitem_tag
{
    subinfo_ptr             psub_info;
    uint32_t                last_sent;
    volatile bool           completed;
    bool                    in_progress;
    volatile bool           success;
} p2p_subinfo_t;


} /* namespace DMCF */



#endif /* INC_DMCF_PUBSUB_HPP_ */
