#ifndef JESSY_UTIL_H
#define JESSY__UTIL_H

#include <Arduino.h>
#include <jessy_defs.h>

class JessyUtility {
public:
    static void log(JessyLogType logType, String message);

    static JessyRTC fromTime(long time);
    static JessyRTC getRTC();
};

#endif