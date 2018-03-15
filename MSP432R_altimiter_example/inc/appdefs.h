/* \file Application defines 
 *
 *
 *
 */

#ifndef APPDEFS_H
#define APPDEFS_H

#include <ti/display/Display.h>
#include <pthread.h>
#include "DmcfLibSystem.h"

extern pthread_mutex_t gDisplayMuxtex;
extern Display_Handle  gTheDisplay;

#define APP_QUEUE_DEPTH (10)

extern dmcf_node_t currentApplication;

#ifdef __cplusplus
extern "C" void *mainThread(void *arg0);
#else
extern void *mainThread(void *arg0);
#endif

#endif
