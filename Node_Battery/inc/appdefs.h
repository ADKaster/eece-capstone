/* \file Application defines 
 *
 *
 *
 */

#ifndef APPDEFS_H
#define APPDEFS_H

#include <ti/display/Display.h>
#include <ti/drivers/I2C.h>
#include <pthread.h>
#include "DmcfLibSystem.h"

extern pthread_mutex_t gDisplayMuxtex;
extern Display_Handle  gTheDisplay;

#define APP_QUEUE_DEPTH (10)

extern I2C_Handle battpack_i2c;
extern dmcf_node_t currentApplication;

#endif
