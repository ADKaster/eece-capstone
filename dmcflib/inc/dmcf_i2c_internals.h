/**
 * \file 
 *
 * \brief Private I2C implementations
 *
 */

#ifndef DMCF_I2C_INTERNALS_H
#define DMCF_I2C_INTERNALS_H

#include <mqueue.h>
#include "dmcf_msgdef.h"


/* This enum is needed by arch specific functions */
typedef enum i2c_status_tag
{
    I2C_SUCCESS,
    I2C_FAIL,
    I2C_ABORT,
} dmcf_i2c_status_t;

/* Architecture selection logic. Add new architectures here */
#ifdef __MSP432P401R__
    #include <ti/drivers/I2C.h>
    typedef I2C_Transaction                     dmcf_i2c_trans_t;
    #define dmcf_arch_i2c_transfer(request)     i2c_msp432_master_transfer(request)
    #define dmcf_arch_i2c_slave_send(buf, size) i2c_msp432_slave_send(buf, size)
    #define dmcf_arch_i2c_slave_read(buf, size) i2c_msp432_slave_read(buf, size)
    #define dmcf_arch_i2c_init                  i2c_msp432_init
    #include "i2c_msp432.h"
#else
    #error Unsupported processor/board. Please define a valid processor.
#endif

typedef struct mas_q_tag
{
    dmcf_i2c_trans_t    transaction;
    volatile bool      *completed;
    volatile bool      *success;
} dmcf_master_action_t;

typedef enum i2c_tx_type_tag
{
    I2C_TX_SEND_TYPE,
    I2C_TX_GET_TYPE,
} i2c_tx_type_t;

typedef enum
{
    I2C_SLAVE_READ,
    I2C_SLAVE_WRITE
} dmcf_i2c_slave_state_t;

void *i2cMasterThread(void *arg0);
void *i2cSlaveThread(void *arg0);

dmcf_i2c_status_t dmcf_i2c_init(void);

extern mqd_t gI2C_MasterActionQueue;

extern pthread_t            i2cMasterPthread;
extern pthread_t            i2cSlavePthread;

#endif /* DMCF_I2C_INTERNALS_H */
