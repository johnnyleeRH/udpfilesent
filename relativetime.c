#include <errno.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
/*#include <stdio.h>
#include <unistd.h>*/
#include "log.h"

static timer_t timerid = 0;
/*void testfunc(sigval_t sigval) {
    int* p = (int*)sigval.sival_ptr;
    printf("haha\n");
}*/

void SetRelativeTimer(void* func) {
    struct sigevent evp;
    struct itimerspec tmspec;
    const time_t kdelay = 1;

    memset(&evp, 0, sizeof(evp));
    evp.sigev_value.sival_ptr = NULL;
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = func;
    if (timer_create(CLOCK_REALTIME, &evp, &timerid) < 0) {
        ERROR("fail to create login timer %d!\n", errno);
        return;
    }

    memset(&tmspec, 0, sizeof(tmspec));
    tmspec.it_value.tv_sec =  kdelay;
    tmspec.it_value.tv_nsec = 0;
    if (timer_settime(timerid, 0, &tmspec, 0) < 0) {
        ERROR("fail to set login timer failed %d!\n", errno);
        return;
    }
}

/*int main() {
    SetRelativeTimer(testfunc);
    sleep(10);
}*/

