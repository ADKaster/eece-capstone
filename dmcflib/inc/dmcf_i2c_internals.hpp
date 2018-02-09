/*
 * dmcf_i2c_internals.hpp
 *
 *  Created on: Feb 8, 2018
 *      Author: Andrew
 */

#ifndef INC_DMCF_I2C_INTERNALS_HPP_
#define INC_DMCF_I2C_INTERNALS_HPP_


#include <ti/Drivers/I2C.h>

namespace DMCF
{

typedef I2C_Transaction i2c_trans_t;

/* This enum is needed by arch specific functions */
typedef enum i2c_status_tag
{
    I2C_SUCCESS,
    I2C_FAIL,
    I2C_ABORT,
} i2c_status_t;

typedef struct mas_q_tag
{
    i2c_trans_t         transaction;
    volatile bool      *completed;
    volatile bool      *success;
} master_action_t;

typedef enum i2c_tx_type_tag
{
    I2C_TX_SEND_TYPE,
    I2C_TX_GET_TYPE,
} i2c_tx_type_t;

typedef enum
{
    I2C_SLAVE_READ,
    I2C_SLAVE_WRITE
} i2c_slave_state_t;

} /* namespace DMCF */

#endif /* INC_DMCF_I2C_INTERNALS_HPP_ */
