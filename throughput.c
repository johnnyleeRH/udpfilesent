#include <sys/time.h>
#include <time.h>

/*
 * Calculate line rate interval in microseconds for the given
 * pkt_len (bytes) and linerate (bit)
 *
 * to send packets at line rate with assumption of link speed at X:
 * interval = ((packet length * bits per byte) / (X)) * 1000000
 * ex:
 * +---------------------------------------------------+
 * |            | 10Mbps      | 100Mbps    | 1000Mbps  |
 * +---------------------------------------------------+
 * |   14 bytes | 11 usecs.   | 1 usecs.   | 0 usecs.  |
 * | 1514 bytes | 1155 usecs. | 116 usecs. | 12 usecs. |
 * +---------------------------------------------------+
 */

#define ROUND(f) (f >= 0 ? (long)(f + 0.5) : (long)(f - 0.5))

int LinerateInterval(int pkt_len, int linerate) {
    return ROUND(((float)pkt_len * 8) / linerate * 1000000);
}

#ifndef TIMEVAL_TO_TIMESPEC
#define TIMEVAL_TO_TIMESPEC(tv, ts) {       \
    (ts)->tv_sec = (tv)->tv_sec;            \
    (ts)->tv_nsec = (tv)->tv_usec * 1000;   \
}
#endif

void sleeptest() {
    struct timeval sleep = {0,0};
    struct timespec nsleep;
    if (timerisset(&sleep)) {
        TIMEVAL_TO_TIMESPEC(&sleep, &nsleep);
        nanosleep(&nsleep, NULL);
    }
}

