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

    // PyroDriver activate method interface, activates the corresponding pyro gpio
    void PyroDriver_activate(int pyroIndex);

#ifdef __cplusplus
}
#endif
#endif /* INC_PYRODRIVER_INTERFACE_H_ */
