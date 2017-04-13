#include <sys/time.h>
#include <time.h>
#include <stdio.h>

#include "filesent.h"
#include "iniopr.h"

extern int IsAppToBeStopped();
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

int LinerateInterval(const int pktlen, const int linerate) {
    return ROUND(((float)pktlen * 8) / linerate * 1000000);
}

void SendFilePackets() {
    static char msgbuf[4096];
    int currentfile = 0;
    while (!IsAppToBeStopped()) {
        FILE* inistream = fopen(file, "r+");
    }
}

#if 0
void sleeptest() {
    struct timeval sleep = {0,0};
    struct timespec nsleep;
    if (timerisset(&sleep)) {
        TIMEVAL_TO_TIMESPEC(&sleep, &nsleep);
        nanosleep(&nsleep, NULL);
    }
}
#endif

