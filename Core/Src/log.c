
#include "log.h"

#define MAX_CALLBACKS 32

extern FATFS SDFatFS; /* File system object for SD logical drive */


FIL sdFile; /* File object for SD */



char *log_level(const uint8_t level) {
    switch (level) {
        case LOG_TRACE:
            return "TRACE";
        case LOG_DEBUG:
            return "DEBUG";
        case LOG_INFO:
            return "INFO";
        case LOG_WARN:
            return "WARN";
        case LOG_ERROR:
            return "ERROR";
        default:
            return "INFO";
    }
}

void log_init() {
#ifdef ENABLE_LOG
    RTC_DateTypeDef rDate;
    HAL_RTC_GetDate(&hrtc, &rDate, RTC_FORMAT_BIN);
    char filename[15];
    sprintf(filename, "%04d-%02d-%02d.log", 2000 + rDate.Year, rDate.Month, rDate.Date);
    retSD = f_open(&sdFile, filename, FA_OPEN_APPEND | FA_WRITE | FA_READ);

    if (retSD) {
        printf("open log file error : %d\n", retSD);
    } else {
        printf("open log file success:%s \n", filename);
    }

#endif
}


void log_log(const uint8_t level, const char *file, const uint8_t line, const char *fmt, ...) {
#ifdef ENABLE_LOG
    if (level >= LOG_LEVEL) {
        va_list arg;
        va_start(arg, fmt);
        char buf[50] = {0};
        vsnprintf(buf, sizeof(buf), fmt, arg);
        get_fattime();
        va_end(arg);

        RTC_TimeTypeDef rTime;
        HAL_RTC_GetTime(&hrtc, &rTime, RTC_FORMAT_BIN);

        //printf("%02d:%02d:%02d [%-5s] [%10s%4d] %s \n", rTime.Hours, rTime.Minutes, rTime.Seconds,
        //               log_level(level), file, line, buf);
        char bufs[100];
        sprintf(bufs, "%02d:%02d:%02d [%-5s] [%10s%4d] %s \n", rTime.Hours, rTime.Minutes, rTime.Seconds,
                log_level(level), file, line, buf);
        //写入文件
        uint32_t bw = 0;
        retSD = f_write(&sdFile, bufs, sizeof(bufs), (void *) &bw);

        /*
        if (retSD) {
            printf("write log file error: %d\n", retSD);
        } else {
            printf("write log file success,data:%lu\n", bw);
        }
         */
        //同步比较耗时，每次同步大概需要800ms，所以需定时同步
        //在同步情况下，每秒可写入1200-1500行，同步情况下只能写入100多行
        if (rTime.Seconds % 5 == 0) {
            retSD = f_sync(&sdFile);
        }
        /*
        if (retSD) {
            printf("sync log file error: %d\n", retSD);
        } else {
            printf("sync log file success,data\n");
        }
         */

    }
#endif
}


void close() {
    retSD = f_close(&sdFile);
    if (retSD) {
        printf("write log close error: %d\n", retSD);
    } else {
        printf("write close success\n");
    }
}







