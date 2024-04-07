#include "log.h"

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

// no use
void ff_log(char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void ff_log_line(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    struct timespec tspec;
    clock_gettime(CLOCK_REALTIME, &tspec);
    struct tm t;
    localtime_r(&tspec.tv_sec, &t);
    int ms = tspec.tv_nsec / 1000000;
    int us = tspec.tv_nsec / 1000 - ms * 1000;
    printf("%4d-%02d-%02d %02d:%02d:%02d.%03d%03d ", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, ms, us);

    vprintf(format, args);
    printf("\n");
    va_end(args);

    fflush(stdout);
}
