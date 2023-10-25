#include <Arduino.h>
#include <jessy_agent.h>
#include <jessy_bios.h>
#include <jessy_const.h>
#include <jessy_io.h>
#include <jessy_terminal.h>
#include <jessy_util.h>
#include <Wire.h>

void setup() {
    Serial.begin(115200);
    while(!Serial)
        yield();

    JessyBIOS::bootUp();
    JessyBIOS::fileSystemCheck();
    JessyIO::println();

    currentAgent.anonymous();
    delay(100);
    JessyIO::print(currentAgent.shellString());
}

void loop() {
    if(Serial.available() > 0) {
        String out = Serial.readStringUntil('\n');

        String tokens[16];
        uint8_t count;

        JessyUtility::shellTokenizer(out, tokens, count);
        JessyExecCommand(currentAgent, tokens, count);

        delay(100);
        JessyIO::print(currentAgent.shellString());
    }
}