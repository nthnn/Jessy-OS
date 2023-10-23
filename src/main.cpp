#include <Arduino.h>
#include <jessy_agent.h>
#include <jessy_bios.h>
#include <jessy_const.h>
#include <jessy_io.h>
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

    JessyIO::print(currentAgent.shellString());
    delay(50);
}

void loop() {
    if(Serial.available() > 0) {
        String out = Serial.readStringUntil('\n');

        JessyIO::println(out);
        JessyIO::print(currentAgent.shellString());
    }
}