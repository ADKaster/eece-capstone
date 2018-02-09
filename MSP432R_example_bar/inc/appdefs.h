/* \file Application defines 
 *
 *
 *
 */

#ifndef APPDEFS_H
#define APPDEFS_H

#ifdef __cplusplus
#include "dmcf_msp432.hpp"
extern DMCF::dmcf_msp432 dmcf_obj;

extern "C" {
#endif

#include <ti/display/Display.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>

extern pthread_mutex_t gDisplayMuxtex;
extern Display_Handle  gTheDisplay;

#define APP_QUEUE_DEPTH (10)

extern void *mainThread(void *arg0);
extern void ApplicationInit(void);

#ifdef __cplusplus
}
#endif

#endif
