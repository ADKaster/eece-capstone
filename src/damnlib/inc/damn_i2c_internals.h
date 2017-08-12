/**
 * \file 
 *
 * \brief Private I2C implementations
 *
 */

#ifndef DAMN_I2C_INTERNALS_H
#define DAMN_I2C_INTERNALS_H

#include <mqueue.h>
#include "ringbuf.h"
#include "damn_msgdef.h"


/* This enum is needed by arch specific functions */
typedef enum i2c_status_tag
{
    I2C_SUCCESS,
    I2C_FAIL,
    I2C_ABORT,
} damn_i2c_status_t;

/* Architecture selection logic. Add new architectures here */
#ifdef __MSP432P401R__
    typedef I2C_Transaction                     damn_i2c_trans_t;
    #define damn_arch_i2c_transfer(request)     i2c_msp432_master_transfer(request);
    #define damn_arch_i2c_init                  i2c_msp432_init
    #include "i2c_msp432.h"
#else
    #error Unsupported processor/board. Please define a valid processor.
#endif

typedef enum i2c_tx_type_tag
{
    I2C_TX_SEND_TYPE,
    I2C_TX_GET_TYPE,
} i2c_tx_type_t;

#define I2C_SLAVE_RINGBUF_SIZE (0x100) /* 256 byte ring buffer should be plenty(tm) */

typedef enum
{
    I2C_SLAVE_WAIT_HDR,
    I2C_PARSE_HDR,
    I2C_SLAVE_WAIT_MSG,
    I2C_PROCESS_MSG,
    I2C_SLAVE_SEND_RESP,
    I2C_SLAVE_SEND_NACK
} damn_i2c_slave_state_t;

/* don't put a semicolon at the end of this! part of this macro is a function definition */
RINGBUF_DECL(i2c_slave, I2C_SLAVE_RINGBUF_SIZE)

void *i2cMasterThread(void *arg0);
void *i2cSlaveThread(void *arg0);

damn_i2c_status_t damn_i2c_init(void);

extern mqd_t gI2C_TXQueue;
extern mqd_t gI2C_RXQueue;
extern volatile i2c_slave_ringbuf_t gI2C_SlaveRingbuf;

extern pthread_t            i2cMasterPthread;
extern pthread_t            i2cSlavePthread;

#endif /* DAMN_I2C_INTERNALS_H */
