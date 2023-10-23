#include <Arduino.h>
#include <jessy_agent.h>
#include <jessy_bios.h>
#include <jessy_const.h>
#include <jessy_io.h>
#include <Wire.h>

JessyAgent agent;

void setup() {
    Serial.begin(115200);
    while(!Serial)
        yield();

    JessyBIOS::bootUp();
    JessyBIOS::fileSystemCheck();
    JessyIO::println();

    agent.setName("<anon>");

    JessyIO::print(agent.shellString());
    delay(50);
}

void loop() {
    if(Serial.available() > 0) {
        String out = Serial.readStringUntil('\n');

        JessyIO::println(out);
        JessyIO::print(agent.shellString());
    }
}