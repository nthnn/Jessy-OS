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

#include <Arduino.h>
#include <Wire.h>

#include "jessy_agent.h"
#include "jessy_bios.h"
#include "jessy_const.h"
#include "jessy_io.h"
#include "jessy_terminal.h"
#include "jessy_util.h"

void setup() {
    Serial.begin(115200);
    while(!Serial)
        yield();

    JessyBIOS::bootUp(currentAgent);
    JessyBIOS::fileSystemCheck();
    JessyIO::println(F("\n\033[91m██\033[92m██\033[93m██\033[94m██\033[95m██\033[96m██\033[97m██\033[0m\n"));

    JessyBIOS::autorun(currentAgent);
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