#ifndef JESSY_BIOS_H
#define JESSY_BIOS_H

#include <Arduino.h>
#include <RTClib.h>

static RTC_DS1307 JessyDS1307;

class JessyBIOS {
public:
    static bool checkSdCard();
    static bool checkRTC();

    static void bootUp();
    static void halt();

    static void fileSystemCheck();
    static bool hasAccounts();

    static void createAccount();
};

#endif