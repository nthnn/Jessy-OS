#include <Arduino.h>
#include <jessy_agent.h>
#include <jessy_bios.h>
#include <jessy_const.h>
#include <jessy_io.h>
#include <jessy_util.h>
#include <Wire.h>

void setup() {
    Serial.begin(115200);
    while(!Serial)
        yield();

    JessyBIOS::bootUp();
    JessyBIOS::fileSystemCheck();
    JessyIO::println();

    currentAgent.setName("<anon>");
    currentAgent.setWorkingDirectory("/");

    delay(100);
    JessyIO::print(currentAgent.shellString());
}

void loop() {
    if(Serial.available() > 0) {
        String out = Serial.readStringUntil('\n');

        String tokens[16];
        uint8_t count;

        JessyUtility::shellTokenizer(out, tokens, count);
        for(uint8_t i = 0; i < count; i++)
            Serial.println("-> " + tokens[i]);

        JessyIO::print(currentAgent.shellString());
    }
}