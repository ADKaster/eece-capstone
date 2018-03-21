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
    #define GPIO_PYRO_FIRST 7
    #define GPIO_PYRO_SECOND 8
    #define GPIO_PYRO_THIRD 9
    #define GPIO_PYRO_FOURTH 10

    #define TIME_PYRO 30 /* Length of the time the pyro runs multiplied by the PYRO_TIMER_INTERVAL below*/
    #define PYRO_TIMER_INTERVAL 500000
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

        /**
         * This activates a GPIO by setting it to high and creating a callback to turn it off.
         */
        void activate(int idx);

    private:
        bool activated[4];
        int time;
        Timer_Handle timer0;
        Timer_Params params;
    };
#endif
#endif /* INC_PYRODRIVER_HPP_ */

