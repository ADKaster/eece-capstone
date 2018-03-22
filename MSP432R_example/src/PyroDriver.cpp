/*
 * pyroDriver.cpp
 *
 *  Created on: Feb 14, 2018
 *      Author: Brian
 */

#include <PyroDriver.hpp>
#include <stddef.h>


volatile int pyroIdx[4];
const int pyroGpioValues[] = {GPIO_PYRO_FIRST, GPIO_PYRO_SECOND, GPIO_PYRO_THIRD, GPIO_PYRO_FOURTH};


/*
 * This callback is called when the timer runs out.
 * */
extern "C" void timerCallbackFunc(Timer_Handle myHandle)
{
    for(int i = 0; i < 4; i++)
    {
        if (pyroIdx[i] >= TIME_PYRO)
        {
            GPIO_toggle(pyroGpioValues[i]);
            pyroIdx[i] = 0;

        }
        else if(pyroIdx[i] > 0)
        {
            pyroIdx[i]++;
        }
    }

}

PyroDriver::PyroDriver(void)
{
    pyroIdx[0] = 0;
    pyroIdx[1] = 0;
    pyroIdx[2] = 0;
    pyroIdx[3] = 0;
    time = PYRO_TIMER_INTERVAL;

    /* Call driver init functions */
    GPIO_init();
    Timer_init();


    /* Configure the GPIO pin probably will change for the production portion*/
    GPIO_setConfig(GPIO_PYRO_FIRST, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_PYRO_SECOND, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_PYRO_THIRD, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(GPIO_PYRO_FOURTH, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Turn off user LED */
    GPIO_write(GPIO_PYRO_FIRST, Board_GPIO_LED_OFF);
    GPIO_write(GPIO_PYRO_SECOND, Board_GPIO_LED_OFF);
    GPIO_write(GPIO_PYRO_THIRD, Board_GPIO_LED_OFF);
    GPIO_write(GPIO_PYRO_FOURTH, Board_GPIO_LED_OFF);

    /* Setting up the timer in continuous callback mode that calls the callback
     * function every PYRO_TIMER_INTERVAL microseconds
     */
    Timer_Params_init(&params);
    params.period = time;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerCallbackFunc;

    timer0 = Timer_open(Board_TIMER0, &params);

    if (timer0 == NULL) {
        /* Failed to initialized timer */
        while (1);
    }

    Timer_start(timer0);
}

void PyroDriver::activate(int pyroGpio)
{
    if(pyroGpio < sizeof(pyroIdx)/sizeof(pyroIdx[0]))
    {
        pyroIdx[pyroGpio] = 1;

        GPIO_toggle(pyroGpioValues[pyroGpio]);

    }
}

PyroDriver::~PyroDriver(void)
{

}
