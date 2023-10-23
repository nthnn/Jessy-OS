#include <FS.h>
#include <jessy_bios.h>
#include <jessy_const.h>
#include <jessy_io.h>
#include <jessy_util.h>
#include <RTClib.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>

bool JessyBIOS::checkSdCard() {
    return SD.begin(JESSY_SD_CARD_CS);
}

bool JessyBIOS::checkRTC() {
    return JessyDS1307.begin() && JessyDS1307.isrunning();
}

void JessyBIOS::bootUp() {
    JessyIO::println(F("---\n"));
    delay(500);

    JessyUtility::log(JSY_LOG_PLAIN, "Jessy OS " + String(JESSY_OS_VERSION) +
        " [" + String(getCpuFrequencyMhz()) + "Mhz]");
    JessyUtility::log(JSY_LOG_PLAIN, F("Booting up..."));
    JessyUtility::log(JSY_LOG_PLAIN, F("Checking up SD card..."));

    if(!JessyBIOS::checkSdCard()) {
        JessyUtility::log(JSY_LOG_ERROR, F("No SD card found."));
        JessyBIOS::halt();
    }

    JessyUtility::log(JSY_LOG_PLAIN, F("SD card initialized!"));
    JessyUtility::log(JSY_LOG_PLAIN, F("Check up DS1307 RTC..."));

    if(!JessyBIOS::checkRTC()) {
        JessyUtility::log(JSY_LOG_ERROR, F("No real-time clock found."));
        JessyBIOS::halt();
    }

    JessyUtility::log(JSY_LOG_PLAIN, F("Real-time clock found!"));

    DateTime now = JessyDS1307.now();
    JessyUtility::log(JSY_LOG_SUCCESS, "Boot up done at " + JessyUtility::getRTCString(now) + ".");
}

void JessyBIOS::halt() {
    while(true)
        yield();
}

void JessyBIOS::fileSystemCheck() {
    bool shouldCreateUser = false;

    String sysFolder = F("/sys"),
        rootFolder = F("/root"),
        userAccount = F("/sys/user.ini");

    JessyUtility::log(JSY_LOG_PLAIN, F("File system checking..."));
    if(!JessyIO::exists(sysFolder) ||
        (JessyIO::exists(sysFolder) &&
            !JessyIO::isDirectory(sysFolder))) {
        shouldCreateUser = true;

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

    JessyUtility::log(JSY_LOG_SUCCESS, F("Checking user accounts..."));
    if(!JessyIO::exists(userAccount) ||
        (JessyIO::exists(userAccount) &&
            !JessyIO::isFile(userAccount))) {
        shouldCreateUser = true;

        JessyUtility::log(JSY_LOG_WARNING, F("No user account found."));
        if(!JessyIO::writeFile(userAccount, F(""))) {
            JessyUtility::log(JSY_LOG_WARNING, F("Something went wrong while generating configuration."));
            JessyUtility::log(JSY_LOG_ERROR, F("Halting..."));
            JessyBIOS::halt();
        }

        JessyUtility::log(JSY_LOG_SUCCESS, F("Created configuration."));
    }

    JessyUtility::log(JSY_LOG_SUCCESS, F("File check done."));
}