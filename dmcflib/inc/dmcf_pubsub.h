/**
 * \file 
 *
 * \brief PubSub API definition
 *
 */

#ifndef DMCF_PUBSUB_H
#define DMCF_PUBSUB_H

#include "dmcf_msgdef.h"
#include <stdint.h>
#include <stdbool.h>

#define MAX_QUEUE_DEPTH (20) /* Needs testing. 20 seems reasonable maximum though */

typedef enum pub_status_tag
{
    PUB_SUCCESS,
    PUB_FAIL,
    PUB_INVALID,
    PUB_FULL,
} dmcf_pub_status_t;

typedef enum sub_status_tag
{
    SUB_SUCCESS,
    SUB_FAIL,
    SUB_INVALID,
    SUB_NONE,
    SUB_ERR
} dmcf_sub_status_t;

typedef enum
{
    FREQ_UNLIMITED,
    FREQ_TEN_HZ,
    FREQ_TWENTY_HZ,
    FREQ_ONE_HUNDRED_HZ,
} dmcf_pubsub_freq_t;

typedef enum pubsubstat_tag
{
    STATUS_IGNORE = 0,
    STATUS_SUBSCRIBED,
    STATUS_PUBLISHING
} dmcf_pubsub_status_t;

typedef struct subinfo_tag
{
    dmcf_msgdef_t          *pmsg_def;
    uint32_t                period_ms;
    mqd_t                   queue;
    uint32_t                q_width;
    uint32_t                q_depth;
    dmcf_pubsub_status_t    status;
} dmcf_subinfo_t, *dmcf_subinfo_ptr;

typedef dmcf_subinfo_t dmcf_pubinfo_t;
typedef dmcf_subinfo_ptr dmcf_pubinfo_ptr;

typedef struct p2pitem_tag
{
    dmcf_subinfo_ptr        psub_info;
    uint32_t                last_sent;
    volatile bool           completed;
    bool                    in_progress;
    volatile bool           success;
} dmcf_p2p_subinfo_t;

//Basic idea: Broadcast -- Publisher controlled timing
//            P2P       -- Subscriber controlled timing

// If a subscriber tries to poll at a frequency faster than the publisher is
// publishing, the publisher will notify the subscriber that it's going too fast.
dmcf_pub_status_t dmcf_publish_configure(dmcf_msg_enum_t id,
                                         dmcf_pubsub_freq_t frequency,
                                         uint32_t queue_depth);

dmcf_pub_status_t dmcf_pub_put(dmcf_msg_enum_t id,
                               void *send_buff);

// When frequency is set to unlimited, the message will be populated in the
// subscriber's recieve buffer when published by the publisher.
// This can only happen for broadcast messages. For point to point messages,
// the subscriber must configure a frequency to poll the publisher at.
dmcf_sub_status_t dmcf_subscribe_configure(dmcf_msg_enum_t id,
                                           dmcf_pubsub_freq_t frequency,
                                           uint32_t queue_depth);

dmcf_sub_status_t dmcf_sub_get(dmcf_msg_enum_t id,
                               void *recvbuff,
                               dmcf_nack_t *ack_chk);

void dmcf_init(void);

extern dmcf_subinfo_t gTheSubscriptions[NUM_MSG_DEFINITONS];
extern dmcf_pubinfo_t gThePublications[NUM_MSG_DEFINITONS];


#endif /* DMCF_PUBSUB_H */
