/*! \file Application Initialization

    Contains task initalization and library initialization
*/  


#include <ti/display/Display.h>
#include <pthread.h>
#include "Board.h"
#include "damn_pubsub.h"
#include "appdefs.h"

/* Stack size in bytes */
#define THREADSTACKSIZE   1024

extern void *mainThread(void *arg0);
void createMainThread(void);
void appDisplay_Init(void);

/* Global data */
pthread_mutex_t gDisplayMuxtex;
Display_Handle gTheDisplay;

/* CHANGE THIS FOR DIFFERENT APPLICATION */
damn_node_t currentApplication = NODE_FOO;

/* Initialize the entire application before the scheduler. Initialize damn library BEFORE application subscribes to anything!!! */
void ApplicationInit(void)
{
    damn_init();

    createMainThread();

    appDisplay_Init();

    if(NODE_FOO == currentApplication)
     {
        damn_subscribe_configure(STANDARD_PING_MSG,
                                 FREQ_TEN_HZ,
                                 APP_QUEUE_DEPTH);

        damn_publish_configure(BROADCAST_PING_MSG,
                               FREQ_UNLIMITED,
                               APP_QUEUE_DEPTH);
    }
    else
    {
        damn_subscribe_configure(BROADCAST_PING_MSG,
                                 FREQ_UNLIMITED,
                                 APP_QUEUE_DEPTH);

        damn_publish_configure(STANDARD_PING_MSG,
                               FREQ_TEN_HZ,
                               APP_QUEUE_DEPTH);
    }

    return;
}

/* This thread is made using TI's implementation of the POSIX API. */
/* Using POSIX ensures compatability with TI_RTOS */
void createMainThread(void)
{
    pthread_t           thread;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;
    int                 detachState;

    /* Set priority and stack size attributes */
    pthread_attr_init(&attrs);
    priParam.sched_priority = 2;

    detachState = PTHREAD_CREATE_DETACHED;
    retc = pthread_attr_setdetachstate(&attrs, detachState);
    if (retc != 0) {
        /* pthread_attr_setdetachstate() failed */
        while (1);
    }

    pthread_attr_setschedparam(&attrs, &priParam);

    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* pthread_attr_setstacksize() failed */
        while (1);
    }

    retc = pthread_create(&thread, &attrs, mainThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }

}

/* Intialize the display and create a mutex for it */
void appDisplay_Init(void)
{
    int retc = 0;

    Display_init();

    /* Open the HOST display for output */
    gTheDisplay = Display_open(Display_Type_UART, NULL);
    if (gTheDisplay == NULL) {
        while (1);
    }

    /* no pthread_mutexattr_t needed because we don't need a recursive mutex on the display */
    retc = pthread_mutex_init(&gDisplayMuxtex, NULL);
    if (retc != 0)
    {
        /* pthread_mutex_init() failed */
        while(1);
    }
}
