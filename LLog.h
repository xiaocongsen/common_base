#ifndef L_LOG_H
#define L_LOG_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

static void myLog(const char* file, int line, const char* fmt, ...)
{
    static char* buf = (char*)malloc(409600);
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, 409600, fmt, ap);
    va_end(ap);
    printf("---------------------------------------\n%s, %d\n%s\n", file, line, buf);
}
#define Log(fmt, args...) myLog(__FILE__, __LINE__, fmt, ##args);
#define TR Log
#define DEBUG Log

// #define DUMP printf

#endif
