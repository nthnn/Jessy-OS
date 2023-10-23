#include <jessy_bios.h>
#include <jessy_io.h>
#include <jessy_util.h>
#include <RTClib.h>
#include <TimeLib.h>

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

JessyRTC JessyUtility::getRTC(DateTime dateTime) {
    JessyRTC rtc;
    rtc.year = dateTime.year();
    rtc.month = dateTime.month();
    rtc.day = dateTime.day();
    rtc.hour = dateTime.hour();
    rtc.minute = dateTime.minute();
    rtc.second = dateTime.second();

    return rtc;
}

void JessyUtility::log(JessyLogType logType, String message) {
    switch(logType) {
        case JSY_LOG_ERROR:
            JessyIO::print(F("[-] "));
            break;
        
        case JSY_LOG_PLAIN:
            JessyIO::print(F("[+] "));
            break;

        case JSY_LOG_WARNING:
            JessyIO::print(F("[!] "));
            break;

        case JSY_LOG_SUCCESS:
            JessyIO::print(F("[*] "));
            break;
    }

    JessyIO::println(message);
}

String JessyUtility::getRTCString(DateTime dateTime) {
    JessyRTC rtc = JessyUtility::getRTC(dateTime);

    return String(rtc.month) + "/" + String(rtc.day) + "/" +
        String(rtc.year) + " " + String(rtc.hour) + ":" +
        String(rtc.minute) + ":" + String(rtc.second);
}

void JessyUtility::shellTokenizer(String &input, String *output, uint8_t &count) {
    int startIndex = 0;
    count = 0;

    for(uint16_t i = 0; i < input.length(); i++) {
        if(input[i] == '"') {
            int endIndex = input.indexOf('"', i + 1);

            if(endIndex != -1) {
                output[count] = input.substring(i + 1, endIndex);
                i = endIndex;
            }
        }
        else if(input[i] == ' ') {
            output[count] = input.substring(startIndex, i);
            count++;

            startIndex = i + 1;
        }
    }

    if(startIndex < input.length()) {
        output[count] = input.substring(startIndex);
        count++;
    }

    for(uint8_t i = 0; i < count; i++) {
        String out = output[i];

        if(out.startsWith(F("\"")) && out.endsWith(F("\"")))
            output[i] = out.substring(1, out.length() - 1);
    }
}