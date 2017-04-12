#include "log.h"
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int logfile;

static const char *monthList[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
static pthread_mutex_t logMutex =  PTHREAD_MUTEX_INITIALIZER;

void Log( const char* mode, const char* file, int line, const char* func, const char* fmt, ... ) {
    static char tbuf[32];
    struct tm now;
    static char cbuffer[8196];
    int usedlen = 0;
    pthread_mutex_lock( &logMutex );
    time_t t = time(NULL);
    localtime_r( &t, &now );
    snprintf( tbuf, 32, "[%s %02d %02d:%02d:%02d] %s: ",
    monthList[now.tm_mon], now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec, mode );
    va_list argList;
    memcpy(cbuffer, tbuf, 32);
    va_start( argList, fmt );
    usedlen = vsnprintf( cbuffer + 32, 8196 - 32, fmt, argList );
    va_end( argList );
    if (usedlen == -1) {
        pthread_mutex_unlock( &logMutex );
        return;
    } else {
        if (usedlen < 8196 - 33) {
            cbuffer[usedlen + 32] = '\n';
            cbuffer[usedlen + 33] = 0;
        }
        write(logfile, cbuffer, usedlen + 34);
        pthread_mutex_unlock( &logMutex );
    }
}