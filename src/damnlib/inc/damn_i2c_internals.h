/**
 * \file 
 *
 * \brief Private I2C implementations
 *
 */

#ifndef DAMN_I2C_INTERNALS_H
#define DAMN_I2C_INTERNALS_H

#include <mqueue.h>

/* Forward declare request structure. transaction is arch specific. */
typedef struct damn_i2c_request_s damn_i2c_request_t;

/* This enum is needed by arch specific functions */
typedef enum i2c_status_tag
{
	I2C_SUCCESS,
	I2C_FAIL,
	I2C_ABORT,
} damn_i2c_status_t;

/* Architecture selection logic. Add new architectures here */
#ifdef __MSP432P401R__
    #include "i2c_msp432.h"
	#define damn_i2c_trans_t I2C_Transaction
#else
    #error Unsupported processor/board. Please define a valid processor.
#endif

typedef enum i2c_txrx_type_tag
{
	I2C_TX_TYPE,
	I2C_RX_TYPE,
} i2c_txrx_t;

#define I2C_BUFSIZE 0x40 /* max transfer size is 16 words */

struct damn_i2c_request_s
{
	damn_i2c_trans_t transaction;
	uint8_t txbuf[I2C_BUFSIZE];
	uint8_t rxbuf[I2C_BUFSIZE];
	i2c_txrx_t type; 
};


damn_i2c_status_t damn_arch_i2c_tx_send(damn_i2c_request_t *request);

damn_i2c_status_t damn_arch_i2c_tx_get(damn_i2c_request_t *request);

damn_i2c_status_t damn_i2c_init(void);

damn_i2c_status_t damn_arch_i2c_init(void);

void *i2cThread(void *arg0);

extern mqd_t gI2C_TXQueue;
extern mqd_t gI2C_RXQueue;

#endif /* DAMN_I2C_INTERNALS_H */
