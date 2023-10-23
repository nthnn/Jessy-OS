#ifndef JESSY_UTIL_H
#define JESSY__UTIL_H

#include <Arduino.h>
#include <jessy_defs.h>
#include <RTCLib.h>

class JessyUtility {
public:
    static JessyRTC fromTime(long time);
    static JessyRTC getRTC(DateTime dateTime);

    static void log(JessyLogType logType, String message);
    static String getRTCString(DateTime dateTime);
};

#endif