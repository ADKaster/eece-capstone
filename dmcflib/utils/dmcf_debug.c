/*
 * dmcf_debug.c
 *
 *  Created on: Jan 25, 2018
 *      Author: Andrew
 */


#include "dmcf_debug.h"
#include <stdarg.h>
#include <pthread.h>
#include <ti/display/Display.h>

extern pthread_mutex_t gDisplayMuxtex;
extern Display_Handle gTheDisplay;

void dmcf_debugprintf(char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);

    pthread_mutex_lock(&gDisplayMuxtex);

    gTheDisplay->fxnTablePtr->vprintfFxn(gTheDisplay, 0, 0, fmt, va);

    pthread_mutex_unlock(&gDisplayMuxtex);
}
