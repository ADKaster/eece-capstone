/*
 * pyroDriver.cpp
 *
 *  Created on: Feb 14, 2018
 *      Author: Brian
 */

#include <PyroDriver.hpp>
#include <stddef.h>


/*
 * This callback is called when the timer runs out.
 * */
extern "C" void timerCallbackFunc(Timer_Handle myHandle)
{
    if (GPIO_read(Board_GPIO_LED0)) {
        GPIO_toggle(Board_GPIO_LED0);
        Timer_stop(myHandle);
    }
}

PyroDriver::PyroDriver(void)
{
    activated = false;
    time = 5000000;
}

int PyroDriver::activate(void)
{
    if (!activated) {
        /* Period and duty in microseconds */
        Timer_Handle timer0;
        Timer_Params params;

        /* Call driver init functions */
        GPIO_init();
        Timer_init();

        /* Configure the GPIO pin probably will change for the production portion*/
        GPIO_setConfig(Board_GPIO_LED0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

        /* Turn off user LED */
        GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_OFF);

        /* Setting up the timer in continuous callback mode that calls the callback
         * function every 1,000,000 microseconds, or 1 second.
         */
        Timer_Params_init(&params);
        params.period = time;
        params.periodUnits = Timer_PERIOD_US;
        params.timerMode = Timer_ONESHOT_CALLBACK;
        params.timerCallback = timerCallbackFunc;

        GPIO_toggle(Board_GPIO_LED0);

        timer0 = Timer_open(Board_TIMER0, &params);

        if (timer0 == NULL) {
            /* Failed to initialized timer */
            while (1);
        }

        if (Timer_start(timer0) == Timer_STATUS_ERROR) {
            /* Failed to start timer */
            while (1);
        }
        activated = true;
    }

    return 0;
}

PyroDriver::~PyroDriver(void)
{

}
