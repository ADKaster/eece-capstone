/*
 * class.cpp
 *
 *  Created on: Feb 7, 2018
 *      Author: Andrew
 */


#include "class.hpp"
#include <stddef.h>

extern "C" {
#include <FreeRTOS.h>
#include <portmacro.h>
}

myClass::myClass(void)
{
    foo = 0;
    bar = (char *)pvPortMalloc(4*sizeof(char));
}

myClass::~myClass(void)
{
    vPortFree((void *)bar);
}
