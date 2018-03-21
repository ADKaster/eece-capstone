/*
 * pyroDriver_interface.h
 *
 *  Created on: Feb 27, 2018
 *      Author: Brian
 */

#ifndef INC_PYRODRIVER_INTERFACE_H_
#define INC_PYRODRIVER_INTERFACE_H_

#include "PyroDriver.hpp"

#ifdef __cplusplus
extern "C" {
#endif

    // PyroDriver setTime method interface, sets the time for the PyroDriver to remain activated before shutting off
    void PyroDriver_setTime(int timeMS);

#ifdef __cplusplus
}
#endif
#endif /* INC_PYRODRIVER_INTERFACE_H_ */
