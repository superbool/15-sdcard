#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>
//#include <stdbool.h>
//#include <time.h>
#include "fatfs.h"


#define  ENABLE_LOG 1
#define  LOG_LEVEL LOG_INFO

typedef enum {
    LOG_TRACE = 0,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
} LOG_LEVEL_ENUM;

#define log_trace(...) log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void log_init(void);

void log_log(const uint8_t level, const char *file, const uint8_t line, const char *fmt, ...);

#endif
