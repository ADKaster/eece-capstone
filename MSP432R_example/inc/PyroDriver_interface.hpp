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
    // Handler to work with cpp class in c and use the PyroDrivers methods methods
    typedef struct PyroDriver* PyroDriverHandle;

    // PyroDriver constructor interface
    PyroDriverHandle newPyroDriver();

    // PyroDriver activate method, which is used to activate the PyroDriver
    int PyroDriver_activate(PyroDriverHandle v);

    // PyroDriver setTime method interface, sets the time for the PyroDriver to remain activated before shutting off
    void PyroDriver_setTime(PyroDriverHandle v, int timeMS);

    // PyroDriver deconstructor interface
    void deletePyroDriver(PyroDriverHandle v);

#ifdef __cplusplus
}
#endif
#endif /* INC_PYRODRIVER_INTERFACE_H_ */
