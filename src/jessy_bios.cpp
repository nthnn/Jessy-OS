#include <FS.h>
#include <jessy_bios.h>
#include <jessy_const.h>
#include <jessy_obj.h>
#include <RTClib.h>
#include <SD.h>
#include <SPI.h>

bool JessyBIOS::checkSdCard() {
    return SD.begin(JESSY_SD_CARD_CS);
}

bool JessyBIOS::checkRTC() {
    return JessyDS1307.begin();
}

void JessyBIOS::bootUp() {
    Serial.println(F("---\n"));
    delay(500);

    Serial.println("[+] Jessy OS " + String(JESSY_OS_VERSION) + " [" + String(getCpuFrequencyMhz()) + "Mhz]");
    Serial.println(F("[+] Booting up..."));
    Serial.println(F("[+] Checking up SD card..."));

    if(!JessyBIOS::checkSdCard()) {
        Serial.println(F("[-] No SD card found."));
        return;
    }

    Serial.println(F("[+] SD card initialized!"));
    Serial.println(F("[+] Check up DS1307 RTC..."));

    if(!JessyBIOS::checkRTC()) {
        Serial.println(F("[-] No real-time clock found."));
        return;
    }
    else if(!JessyDS1307.isrunning()) {
        Serial.println(F("[-] RTC is not running."));
        return;
    }

    Serial.println(F("[+] Real-time clock found!"));
    Serial.println(F("[*] Boot up done."));
    Serial.println();
}