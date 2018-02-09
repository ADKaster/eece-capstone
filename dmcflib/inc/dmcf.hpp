/*
 * dmcf.hpp
 *
 *  Created on: Feb 8, 2018
 *      Author: Andrew Kaster
 */

#ifndef INC_DMCF_HPP_
#define INC_DMCF_HPP_

#ifdef FREERTOS
    #include <FreeRTOS.h>
    #include <portmacro.h>
    #include <task.h>
    typedef TaskHandle_t kernel_task_handle_t;
#else
    #error "Non-FreeRTOS not supported yet!"
#endif

#include <stdint.h>
#include <stddef.h>
#include <pthread.h>
#include <ti/drivers/I2C.h>

#ifndef size_t
typedef unsigned int size_t;
#endif

#include "dmcf_msgdef.hpp"
#include "DmcfSystem.h"
#include "dmcf_pubsub.hpp"
#include "dmcf_i2c_internals.hpp"

namespace DMCF
{

/******* P2P Defines *******/
#define P2P_SCHED_STACK_SIZE (1024) /* 1kiB of stack */
#define P2P_BUFSIZE (0x100) /* 256 words per message */
#define SUB_BUFSIZE (0x100) /* Hold 256 words */
#define PUB_BUFSIZE SUB_BUFSIZE

/******* I2C Defines *******/
#define I2C_STACK_DEPTH (2048) /* 1kiB of stack */
#define I2C_RX_BUFFER_SIZE (0x100) /* Hold 256  words of data at once */
#define I2C_PUB_BUFFER_SIZE (0x80) /* Hold 128 words of data at once */
#define I2C_TXQUEUE_NAME ("I2C TX QUEUE")
#define I2C_TXQUEUE_DEPTH (0x14)  /* Hold 20 i2c transactions at once */
#define I2C_TXQUEUE_WIDTH (sizeof(master_action_t))

/* Thread wrappers to get around pthread limitations. Pass object as arg0,
 * call member function there */
void *p2p_threadWrapper(void *arg0);
void *i2cM_threadWrapper(void *arg0);
void *i2cS_threadWrapper(void *arg0);

class dmcf
{
    friend void *p2p_threadWrapper(void *arg0);
    friend void *i2cM_threadWrapper(void *arg0);
    friend void *i2cS_threadWrapper(void *arg0);


public:

    /* initalization function */
    virtual bool init(void) = 0;

    /* pubsub API functions */
    //Basic idea: Broadcast -- Publisher controlled timing
    //            P2P       -- Subscriber controlled timing

    // If a subscriber tries to poll at a frequency faster than the publisher is
    // publishing, the publisher will notify the subscriber that it's going too fast.
    pub_status_t publish_configure(msg_enum_t id,
                                   pubsub_freq_t frequency,
                                   uint32_t queue_depth);

    pub_status_t pub_put(msg_enum_t id, void *send_buff);

    // When frequency is set to unlimited, the message will be populated in the
    // subscriber's recieve buffer when published by the publisher.
    // This can only happen for broadcast messages. For point to point messages,
    // the subscriber must configure a frequency to poll the publisher at.
    sub_status_t subscribe_configure(msg_enum_t id,
                                     pubsub_freq_t frequency,
                                     uint32_t queue_depth);

    sub_status_t sub_get(msg_enum_t id,
                         void *recvbuff,
                         nack_t *ack_chk);


protected:
    dmcf(node_t appName, bus_type_t busType);
    ~dmcf() { /* TODO do we need to do anything here or what */ }

   virtual kernel_task_handle_t getSlaveTaskHandle(void) = 0;
   virtual i2c_status_t arch_i2c_master_transfer(i2c_trans_t *request) { while(1); /* master transfer unimplemented in base class */ }
   virtual i2c_status_t arch_i2c_slave_send(void *buf, size_t size) { while(1); /* slave send unimplemented in base class */ }
   virtual i2c_status_t arch_i2c_slave_read(void *buf, size_t size) { while(1); /* slave read unimplemented in base class */ }
   virtual i2c_status_t i2c_init(void);

private:
    /* Messaging functions */
    uint32_t calculate_checksum(uint32_t *buf, uint32_t size);
    bool msg_create_header(pkthdr_t *hdr);
    bool msg_verify_header(pkthdr_t *hdr);
    bool msg_verify_msg(uint32_t *buf, uint32_t len);

    void msg_create_header(pkthdr_t *hdr,
                           node_t src,
                           node_t dest,
                           msg_enum_t id,
                           uint32_t msg_size);

    /* pubsub functions */
    void *p2p_sub_scheduler_task(void *arg0);
    int32_t check_frequency(pubsub_freq_t freq, tx_type_t type);
    void p2p_sub_sched_init(void);

    /* I2C Functions */
    void *i2cMasterThread(void *arg0);
    void *i2cSlaveThread(void *arg0);
    bool Slave_Hdr_Parse(pkthdr_t *hdr, uint32_t *buf);
    void Slave_Reverse_Hdr(pkthdr_t *dest, pkthdr_t *src);
    i2c_slave_state_t Slave_Process_Bytes(void);
    void i2c_slave_send_resp(void);
    void i2c_slave_nack(nack_t nack_byte);

    /********** Private variables ********/
    node_t appName;
    bus_type_t busType;

    /****** I2C Variables ******/
    mqd_t I2C_MasterActionQueue;
    i2c_slave_state_t I2C_SlaveState = I2C_SLAVE_READ;
    uint32_t slaveRxHoldingBuf[I2C_RX_BUFFER_SIZE];
    uint32_t slavePubHoldingBuf[I2C_PUB_BUFFER_SIZE];

    /****** Thread Handles ******/
    pthread_t    i2cMasterPthread;
    pthread_t    i2cSlavePthread;
    pthread_t    p2pSchedPthread;

    /******* Pubsub variables *****/
    p2p_subinfo_t p2p_sublist[NUM_MSG_DEFINITONS + 1];
    uint32_t p2p_sub_rxbuf[NUM_MSG_DEFINITONS][P2P_BUFSIZE];
    pkthdr_t p2p_hdrs[NUM_MSG_DEFINITONS];
    uint32_t sub_holding_buf[SUB_BUFSIZE];
    uint32_t pub_holding_buf[NUM_MSG_DEFINITONS][PUB_BUFSIZE];
    volatile bool throwaway_bcast_bool;
    subinfo_t theSubscriptions[NUM_MSG_DEFINITONS];
    pubinfo_t thePublications[NUM_MSG_DEFINITONS];

}; /* base class dmcf */

} /* namespace dmcf */

#endif /* INC_DMCF_HPP_ */
