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

#include <FS.h>
#include <RTClib.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>

#include "jessy_bios.h"
#include "jessy_const.h"
#include "jessy_io.h"
#include "jessy_terminal.h"
#include "jessy_util.h"

bool JessyBIOS::checkSdCard() {
    return SD.begin(JESSY_SD_CARD_CS);
}

bool JessyBIOS::checkRTC() {
    return JessyDS1307.begin() && JessyDS1307.isrunning();
}

void JessyBIOS::bootUp() {
    JessyIO::println(F("---\n"));
    delay(500);

    JessyIO::println("\033[44m\033[97m        Jessy OS " +
        String(JESSY_OS_VERSION) + F(" [") +
        String(getCpuFrequencyMhz()) + F("Mhz]       \033[0m\n"));

    JessyUtility::log(JSY_LOG_WARNING, F("Booting up..."));
    JessyUtility::log(JSY_LOG_PLAIN, F("Check up DS1307 RTC..."));

    if(!JessyBIOS::checkRTC()) {
        JessyUtility::log(JSY_LOG_ERROR, F("No real-time clock found."));
        JessyBIOS::halt();
    }

    JessyUtility::log(JSY_LOG_SUCCESS, F("Real-time clock found!"));
    JessyUtility::log(JSY_LOG_PLAIN, F("Checking up SD card..."));

    if(!JessyBIOS::checkSdCard()) {
        JessyUtility::log(JSY_LOG_ERROR, F("No SD card found."));
        JessyBIOS::halt();
    }
    JessyUtility::log(JSY_LOG_SUCCESS, F("SD card initialized!"));

    DateTime now = JessyDS1307.now();
    JessyUtility::log(
        JSY_LOG_SUCCESS,
        "Boot up done at " + JessyUtility::getRTCString(now) + "."
    );
}

void JessyBIOS::autorun(JessyAgent &agent) {
    String autorun = JessyUtility::sanitizePath(agent, F("autorun.js"));
    if(!JessyIO::exists(autorun) || !JessyIO::isFile(autorun))
        return;

    String args[] = {F("js"), F("autorun.js")};
    JessyTerminal::js(agent, args, 2);
}

void JessyBIOS::halt() {
    while(true)
        yield();
}

void JessyBIOS::fileSystemCheck() {
    String sysFolder = F("/sys"),
        rootFolder = F("/root"),
        usersFolder = F("/sys/users"),
        sharedFolder = F("/root/shared");

    JessyUtility::log(JSY_LOG_PLAIN, F("File system checking..."));
    if(!JessyIO::exists(sysFolder) ||
        (JessyIO::exists(sysFolder) &&
            !JessyIO::isDirectory(sysFolder))) {
        if(JessyIO::mkdir(sysFolder))
            JessyUtility::log(JSY_LOG_PLAIN, F("Created system folder."));
        else {
            JessyUtility::log(JSY_LOG_WARNING, F("Something went wrong creating system folder..."));
            JessyUtility::log(JSY_LOG_ERROR, F("Halting..."));

            JessyBIOS::halt();
        }
    }

    if(!JessyIO::exists(rootFolder) ||
        (JessyIO::exists(rootFolder) &&
            !JessyIO::isDirectory(rootFolder))) {
        if(JessyIO::mkdir(rootFolder))
            JessyUtility::log(JSY_LOG_PLAIN, F("Created root folder."));
        else {
            JessyUtility::log(JSY_LOG_WARNING, F("Something went wrong creating root folder..."));
            JessyUtility::log(JSY_LOG_ERROR, F("Halting..."));

            JessyBIOS::halt();
        }
    }

    if(!JessyIO::exists(usersFolder) ||
        (JessyIO::exists(usersFolder) &&
            !JessyIO::isDirectory(usersFolder))) {
        if(JessyIO::mkdir(usersFolder))
            JessyUtility::log(JSY_LOG_PLAIN, F("Created users folder on /sys."));
        else {
            JessyUtility::log(JSY_LOG_WARNING, F("Something went wrong creating users folder on /sys..."));
            JessyUtility::log(JSY_LOG_ERROR, F("Halting..."));

            JessyBIOS::halt();
        }
    }

    if(!JessyIO::exists(sharedFolder) ||
        (JessyIO::exists(sharedFolder) &&
            !JessyIO::isDirectory(sharedFolder))) {
        if(JessyIO::mkdir(sharedFolder))
            JessyUtility::log(JSY_LOG_PLAIN, F("Created shared folder on /root."));
        else {
            JessyUtility::log(JSY_LOG_WARNING, F("Something went wrong creating shared folder on /root..."));
            JessyUtility::log(JSY_LOG_ERROR, F("Halting..."));

            JessyBIOS::halt();
        }
    }

    JessyUtility::log(JSY_LOG_SUCCESS, F("File check done."));
}

bool JessyBIOS::hasAccounts() {
    String accounts[10];
    return JessyIO::listFiles("/sys/users", accounts) > 0;
}