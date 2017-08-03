/**
 * \file 
 *
 * \brief PubSub API definition
 *
 */

#ifndef DAMN_PUBSUB_H
#define DAMN_PUBSUB_H

#include "damn_msgdef.h"

typedef enum pub_status_tag
{
    PUB_SUCCESS,
    PUB_FAIL,
    PUB_ABORT,
} damn_pub_status_t;

typedef enum sub_status_tag
{
    SUB_SUCCESS,
    SUB_FAIL,
    SUB_ABORT,
} damn_sub_status_t;

typedef enum
{
    FREQ_UNLIMITED,
    FREQ_TEN_HZ,
    FREQ_TWENTY_HZ,
    FREQ_ONE_HUNDRED_HZ.
} damn_pubsub_freq_t;

//Basic idea: Broadcast -- Publisher controlled timing
//            P2P       -- Subscriber controlled timing

// If a subscriber tries to poll at a frequency faster than the publisher is
// publishing, the publisher will notify the subscriber that it's going too fast.
damn_pub_status_t damn_publish_configure(damn_msgdef_t *msg,
                                         damn_pubsub_freq_t frequency,
                                         void *send_buff);

damn_pub_status_t damn_publish_send(damn_msgdef_t *msg);

// When frequency is set to unlimited, the message will be populated in the
// subscriber's recieve buffer when published by the publisher.
// This can only happen for broadcast messages. For point to point messages,
// the subscriber must configure a frequency to poll the publisher at.
damn_sub_status_t damn_subscribe_configure(damn_msgdef *msg,
                                           damn_pubsub_freq_t frequency);

damn_sub_status_t damn_subscribe_receive(damn_msgdef_t *msg,
                                         uint32_t timeout,
                                         void *recvbuff,
                                         uint32_t num_msgs);

#endif /* DAMN_PUBSUB_H */
