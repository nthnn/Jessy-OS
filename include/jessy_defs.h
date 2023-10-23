#ifndef JESSY_DEFS_H
#define JESSY_DEFS_H

#include <Arduino.h>

typedef enum _JessyLogType {
    JSY_LOG_PLAIN,
    JSY_LOG_WARNING,
    JSY_LOG_ERROR,
    JSY_LOG_SUCCESS
} JessyLogType;

typedef struct _JessyRTC {
    uint16_t hour, minute, second;
    uint16_t day, month, year;
} JessyRTC;

#endif