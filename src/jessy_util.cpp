#include <jessy_obj.h>
#include <jessy_utils.h>
#include <RTClib.h>
#include <TimeLib.h>

void JessyUtility::log(JessyLogType logType, String message) {
}

JessyRTC JessyUtility::fromTime(long time) {
    tmElements_t tm;
    JessyRTC rtc;

    breakTime(time, tm);
    rtc.year = tm.Year;
    rtc.month = tm.Month;
    rtc.day = tm.Day;
    rtc.hour = tm.Hour;
    rtc.minute = tm.Minute;
    rtc.second = tm.Second;

    return rtc;
}

JessyRTC JessyUtility::getRTC() {
    JessyRTC rtc;
    DateTime dateTime = JessyDS1307.now();

    rtc.year = dateTime.year();
    rtc.month = dateTime.month();
    rtc.day = dateTime.day();
    rtc.hour = dateTime.hour();
    rtc.minute = dateTime.minute();
    rtc.second = dateTime.second();

    return rtc;
}