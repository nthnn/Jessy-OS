#ifndef JESSY_BIOS_H
#define JESSY_BIOS_H

#include <Arduino.h>

class JessyBIOS {
public:
    static bool checkSdCard();
    static bool checkRTC();
    static void bootUp();
};

#endif