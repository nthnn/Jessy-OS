#ifndef JESSY_UTIL_H
#define JESSY__UTIL_H

#include <Arduino.h>
#include <jessy_agent.h>
#include <jessy_defs.h>
#include <RTCLib.h>

class JessyUtility {
public:
    static JessyRTC fromTime(long time);
    static JessyRTC getRTC(DateTime dateTime);

    static void log(JessyLogType logType, String message);
    static String getRTCString(DateTime dateTime);
    static String rtcStructString(JessyRTC rtc);

    static void shellTokenizer(String &input, String *output, uint8_t &count);
    static RTC_DS1307 createClock();

    static String sanitizePath(JessyAgent &agent, String path);
    static String aesEncrypt(String keyString, String str);
    static String toBase64(String str);
};

#endif