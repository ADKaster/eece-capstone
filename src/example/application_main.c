/* \file Example main thread



 */

#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include <ti/display/Display.h>

extern pthread_mutex_t gDisplayMuxtex;
extern Display_Handle gTheDisplay;

void *mainThread(void *arg0)
{
    struct timespec currtime;
    for(;;)
    {
        clock_gettime(CLOCK_REALTIME, &currtime);

        pthread_mutex_lock(&gDisplayMuxtex);

        Display_printf(gTheDisplay, 0, 0, "Hello from main: Time: %dsec, %d ns", currtime.tv_sec, currtime.tv_nsec);

        pthread_mutex_unlock(&gDisplayMuxtex);


        sleep(5);
    }
    return NULL;
 }
