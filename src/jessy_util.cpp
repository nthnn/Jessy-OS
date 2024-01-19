/*
 * This file is part of the Jessy OS.
 * Copyright (c) 2023 Nathanne Isip
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <base64.h>
#include <mbedtls/aes.h>
#include <RTClib.h>
#include <TimeLib.h>

#include "jessy_bios.h"
#include "jessy_io.h"
#include "jessy_util.h"

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
            JessyIO::print(F("[\e[90m-\e[0m] "));
            break;

        case JSY_LOG_WARNING:
            JessyIO::print(F("[\e[94m!\e[0m] "));
            break;

        case JSY_LOG_PLAIN:
        case JSY_LOG_SUCCESS:
            JessyIO::print(F("[\e[92m+\e[0m] "));
            break;
    }

    JessyIO::println(message);
}

String JessyUtility::rtcStructString(JessyRTC rtc) {
    return String(rtc.month) + "/" + String(rtc.day) + "/" +
        String(rtc.year >= 2020 ? (rtc.year >= 2100 ? rtc.year - 100 : rtc.year) : rtc.year + 1970) + " " +
        String(rtc.hour) + ":" + String(rtc.minute) + ":" +
        String(rtc.second);
}

String JessyUtility::getRTCString(DateTime dateTime) {
    return JessyUtility::rtcStructString(
        JessyUtility::getRTC(dateTime)
    );
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

RTC_DS1307 JessyUtility::createClock() {
    RTC_DS1307 rtc;
    rtc.begin();

    return rtc;
}

String JessyUtility::sanitizePath(JessyAgent &agent, String path) {
    String agentName = agent.getName(),
        wd = agent.getWorkingDirectory(),
        strictPath = F("/root/");

    strictPath += (agentName == F("anonymous")) ?
        F("shared") : agentName;

    if(path == F("~"))
        wd = strictPath;
    else if(path == F(".") || path == F("./"));
    else if(path.startsWith(F(".."))) {
        int idx = wd.lastIndexOf(F("/"));

        if(wd.substring(0, idx).startsWith(strictPath))
            wd = wd.substring(0, idx) + path.substring(2);
    }
    else if(path.startsWith(F("./")) && !path.endsWith(F("./")))
        wd += path.substring(1);
    else wd += (path.startsWith(F("/")) ? "" : "/") + path;

    if(wd.endsWith("/"))
        wd = wd.substring(0, wd.length() - 1);
    return wd;
}

String JessyUtility::aesEncrypt(String keyString, String string) {
    char key[16];
    unsigned char output[16];

    String keyStr(keyString.c_str());
    keyStr = keyStr + keyStr + keyStr + keyStr;
    strcpy(key, (char*) keyStr.substring(0, 16).c_str());

    String str = string + string + string + string;
    str = str.substring(0, 16);

    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    mbedtls_aes_setkey_enc(&aes, (const unsigned char*) key, strlen(key) * 8);
    mbedtls_aes_crypt_ecb(
        &aes,
        MBEDTLS_AES_ENCRYPT,
        (const unsigned char*) str.c_str(),
        output
    );
    mbedtls_aes_free(&aes);

    String encrypted = "";
    for(uint8_t i = 0; i < 16; i++) {
        char s[3];

        sprintf(s, "%02x", output[i]);
        encrypted += s;
    }
    return encrypted;
}

String JessyUtility::toBase64(String str) {
    uint8_t enclen = Base64.encodedLength(str.length());
    char encstr[enclen + 1];

    Base64.encode(encstr, (char*) str.c_str(), enclen);
    return String(encstr);
}