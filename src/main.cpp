#include <Arduino.h>
#include <jessy_agent.h>
#include <jessy_bios.h>
#include <jessy_const.h>

JessyAgent agent("<anon>");

void setup() {
    Serial.begin(115200);
    while(!Serial)
        yield();

    JessyBIOS::bootUp();
    Serial.print(agent.shellString());
}

void loop() {
    if(Serial.available() > 0) {
        String out = Serial.readStringUntil('\n');

        Serial.println(out);
        Serial.print(agent.shellString());
    }
}