/**
 * \file 
 *
 * \brief PubSub API definition
 *
 */

#ifndef DAMN_PUBSUB_H
#define DAMN_PUBSUB_H

#include "damn_msgdef.h"
#include <stdint.h>
#include <stdbool.h>

#define MAX_QUEUE_DEPTH (20) /* Needs testing. 20 seems reasonable maximum though */

typedef enum pub_status_tag
{
    PUB_SUCCESS,
    PUB_FAIL,
    PUB_INVALID,
    PUB_FULL,
} damn_pub_status_t;

typedef enum sub_status_tag
{
    SUB_SUCCESS,
    SUB_FAIL,
    SUB_INVALID,
    SUB_NONE,
    SUB_ERR
} damn_sub_status_t;

typedef enum
{
    FREQ_UNLIMITED,
    FREQ_TEN_HZ,
    FREQ_TWENTY_HZ,
    FREQ_ONE_HUNDRED_HZ,
} damn_pubsub_freq_t;

typedef enum pubsubstat_tag
{
    STATUS_IGNORE = 0,
    STATUS_SUBSCRIBED,
    STATUS_PUBLISHING
} damn_pubsub_status_t;

typedef struct subinfo_tag
{
    damn_msgdef_t          *pmsg_def;
    uint32_t                period_ms;
    mqd_t                   queue;
    uint32_t                q_width;
    uint32_t                q_depth;
    damn_pubsub_status_t    status;
} damn_subinfo_t, *damn_subinfo_ptr;

typedef damn_subinfo_t damn_pubinfo_t;
typedef damn_subinfo_ptr damn_pubinfo_ptr;

typedef struct p2pitem_tag
{
    damn_subinfo_ptr        psub_info;
    uint32_t                last_sent;
    volatile bool           completed;
    bool                    in_progress;
    volatile bool           success;
} damn_p2p_subinfo_t;

//Basic idea: Broadcast -- Publisher controlled timing
//            P2P       -- Subscriber controlled timing

// If a subscriber tries to poll at a frequency faster than the publisher is
// publishing, the publisher will notify the subscriber that it's going too fast.
damn_pub_status_t damn_publish_configure(damn_msg_enum_t id,
                                         damn_pubsub_freq_t frequency,
                                         uint32_t queue_depth);

damn_pub_status_t damn_pub_put(damn_msg_enum_t id,
                               void *send_buff);

// When frequency is set to unlimited, the message will be populated in the
// subscriber's recieve buffer when published by the publisher.
// This can only happen for broadcast messages. For point to point messages,
// the subscriber must configure a frequency to poll the publisher at.
damn_sub_status_t damn_subscribe_configure(damn_msg_enum_t id,
                                           damn_pubsub_freq_t frequency,
                                           uint32_t queue_depth);

damn_sub_status_t damn_sub_get(damn_msg_enum_t id,
                               void *recvbuff,
                               damn_nack_t *ack_chk);

void damn_init(void);

extern damn_subinfo_t gTheSubscriptions[NUM_MSG_DEFINITONS];
extern damn_pubinfo_t gThePublications[NUM_MSG_DEFINITONS];


#endif /* DAMN_PUBSUB_H */
