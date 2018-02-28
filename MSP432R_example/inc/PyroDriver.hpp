/*
 * pyroDriver.hpp
 *
 *  Created on: Feb 14, 2018
 *      Author: Brian
 */

#ifndef INC_PYRODRIVER_HPP_
#define INC_PYRODRIVER_HPP_

#ifdef __cplusplus
extern "C" {
#endif
    /* Driver Header files */
    #include <ti/drivers/GPIO.h>
    #include <ti/drivers/Timer.h>

    /* Board Header file */
    #include "Board.h"
    #include <FreeRTOS.h>
    #include <portmacro.h>
    void timerCallbackFunc(Timer_Handle myHandle);
#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
    class PyroDriver
    {
    public:
        PyroDriver();
        ~PyroDriver();


        void setTime(int timeMS) {time = timeMS; }

        /**
         * This activates a GPIO by setting it to high and creating a callback to turn it off.
         */
        int activate(void);
        int isActivated() { return activated; }

    private:
        bool activated;
        int time;
    };
#endif
#endif /* INC_PYRODRIVER_HPP_ */

