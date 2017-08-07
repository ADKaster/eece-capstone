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
    #define damn_i2c_trans_t                    I2C_Transaction
    #define damn_arch_i2c_tx_send(request)      i2c_msp432_master_send(request);
    #define damn_arch_i2c_tx_get(request)       i2c_msp432_master_recieve(request);
    #define damn_arch_i2c_init                  i2c_msp432_init
#else
    #error Unsupported processor/board. Please define a valid processor.
#endif

typedef enum i2c_tx_type_tag
{
    I2C_TX_SEND_TYPE,
    I2C_TX_GET_TYPE,
} i2c_tx_type_t;

#define I2C_MASTER_BUFSIZE 0x40 /* max transfer size is 16 words */

/* NOTE: The typedef for this is forward declared at the top of the file */
typedef struct damn_i2c_master_request_s
{
    damn_i2c_trans_t transaction;
    uint8_t txbuf[I2C_MASTER_BUFSIZE];
    uint8_t rxbuf[I2C_MASTER_BUFSIZE];
    i2c_tx_type_t type; 
} damn_i2c_request_t;

#define I2C_SLAVE_RINGBUF_SIZE (0x100) /* 256 byte ring buffer should be plenty(tm) */

typedef enum
{
    I2C_SLAVE_WAIT_HDR,
    I2C_SLAVE_WAIT_MSG,
    I2C_PARSE_HDR,
    I2C_SLAVE_SEND_RESP,
} damn_i2c_slave_state_t;

/* don't put a semicolon at the end of this! part of this macro is a function definition */
RINGBUF_DECL(i2c_slave, I2C_SLAVE_RINGBUF_SIZE)

void *i2cMasterThread(void *arg0);
void *i2cSlaveThread(void *arg0);

void damn_i2cSlaveHandle(damn_pkthdr_t *hdr, volatile ringbuf_t *rbuf);

damn_i2c_status_t damn_i2c_init(void);

extern mqd_t gI2C_TXQueue;
extern mqd_t gI2C_RXQueue;
extern volatile i2c_slave_ringbuf_t gI2C_SlaveRingbuf;

extern pthread_t            i2cMasterPthread;
extern pthread_t            i2cSlavePthread;


#ifdef __MSP432P401R__
    #include "i2c_msp432.h"
#else
    #error Unsupported processor/board. Please define a valid processor.
#endif

#endif /* DAMN_I2C_INTERNALS_H */
