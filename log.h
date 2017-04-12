#ifndef UDPFILESENT_LOG_H
#define UDPFILESENT_LOG_H

extern int logfile;

void Log(const char *mode, const char *file, int line, const char *func, const char *fmt, ...);

#ifdef NO_TRACE
#define TRACE( fmt, ... )
#else
#define TRACE(fmt, ...) Log( "TRC", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__ )
#endif

#ifdef NO_WARN
#define WARN( fmt, ... )
#else
#define WARN(fmt, ...) Log( "WRN", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__ )
#endif

#ifdef NO_INFO
#define INFO( fmt, ... )
#else
#define INFO(fmt, ...) Log( "INF", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__ )
#endif

#ifdef NO_ERROR
#define ERROR( fmt, ... )
#define FATAL( fmt, ... )
#else
#define ERROR(fmt, ...) Log( "ERR", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__ )
#define FATAL(fmt, ...) Log( "FTL", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__ )
#endif

#endif
