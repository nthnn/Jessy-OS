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

#ifndef JESSY_TERMINAL_H
#define JESSY_TERMINAL_H

class JessyTerminal {
public:
    static void cd(JessyAgent &agent, String arguments[], uint8_t argc);
    static void pwd(JessyAgent &agent, String arguments[], uint8_t argc);
    static void ls(JessyAgent &agent, String arguments[], uint8_t argc);
    static void mkdir(JessyAgent &agent, String arguments[], uint8_t argc);
    static void touch(JessyAgent &agent, String arguments[], uint8_t argc);
    static void rm(JessyAgent &agent, String arguments[], uint8_t argc);
    static void cp(JessyAgent &agent, String arguments[], uint8_t argc);
    static void mv(JessyAgent &agent, String arguments[], uint8_t argc);

    static void cat(JessyAgent &agent, String arguments[], uint8_t argc);
    static void echo(JessyAgent &agent, String arguments[], uint8_t argc);
    static void clear(JessyAgent &agent, String arguments[], uint8_t argc);

    static void useradd(JessyAgent &agent, String arguments[], uint8_t argc);
    static void userdel(JessyAgent &agent, String arguments[], uint8_t argc);
    static void passwd(JessyAgent &agent, String arguments[], uint8_t argc);
    static void su(JessyAgent &agent, String arguments[], uint8_t argc);

    static void sd(JessyAgent &agent, String arguments[], uint8_t argc);
    static void esp32cpu(JessyAgent &agent, String arguments[], uint8_t argc);
    static void reboot(JessyAgent &agent, String arguments[], uint8_t argc);
    static void gpio(JessyAgent &agent, String arguments[], uint8_t argc);

    static void date(JessyAgent &agent, String arguments[], uint8_t argc);
    static void time(JessyAgent &agent, String arguments[], uint8_t argc);

    static void ping(JessyAgent &agent, String arguments[], uint8_t argc);
    static void wlan(JessyAgent &agent, String arguments[], uint8_t argc);
    static void bt(JessyAgent &agent, String arguments[], uint8_t argc);
    static void wget(JessyAgent &agent, String arguments[], uint8_t argc);

    static void js(JessyAgent &agent, String arguments[], uint8_t argc);
};

void JessyExecCommand(JessyAgent &agent, String arguments[], uint8_t argc);

#endif