/* \file Application defines 
 *
 *
 *
 */

#ifndef APPDEFS_H
#define APPDEFS_H

#include <ti/display/Display.h>
#include <pthread.h>

#define APPLICATION_WHOAMI (NODE_FOO)

extern pthread_mutex_t gDisplayMuxtex;
extern Display_Handle  gTheDisplay;

#define APP_QUEUE_DEPTH (10)

extern uint32_t currentApplication;

#endif
