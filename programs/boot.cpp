/* 
 * This file is part of the Jessy-OS distribution (https://github.com/nthnn/Jessy-OS).
 * Copyright (c) 2024 Nathanne Isip.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <librishka.h>

typedef enum {
    INFO,
    ERROR,
    WARNING
} log_t;

void log(log_t type, string message) {
    IO::print(F("["), TERM_FG_GREEN, TERM_BG_BLACK, TERM_STYLE_BOLD);

    switch(type) {
        case INFO:
            IO::print(F("+"));
            break;

        case ERROR:
            IO::print(F("-"));
            break;

        case WARNING:
            IO::print(F("!"));
            break;

        default: break;
    }

    IO::print(F("] "), TERM_FG_GREEN, TERM_BG_BLACK, TERM_STYLE_BOLD);
    IO::println(message);
}

void check_sys_folders() {
   if(!FS::exists("~/bin")) {
        FS::mkdir(F("~/bin"));
        log(ERROR, F("Folder ~/bin not found, created one."));
    }

    if(!FS::exists("~/etc")) {
        FS::mkdir(F("~/etc"));
        log(ERROR, F("Folder ~/etc not found, created one."));
    }

    if(!FS::exists("~/home")) {
        FS::mkdir(F("~/home"));
        log(ERROR, F("Folder ~/home not found, created one."));
    }

    if(!FS::exists("~/man")) {
        FS::mkdir(F("~/man"));
        log(ERROR, F("Folder ~/man not found, created one."));
    }

    if(!FS::exists("~/tmp")) {
        FS::mkdir(F("~/tmp"));
        log(ERROR, F("Folder ~/tmp not found, created one."));
    }

    log(INFO, F("Checking system folders done!"));
}

void u8_to_hexstring(u8 value, string str) {
    const char hexdigits[] = "0123456789abcdef";

    str[0] = hexdigits[(value >> 4) & 0x0f];
    str[1] = hexdigits[value & 0x0f];
}

void scan_i2c_devs() {
    u8 err, ndevices = 0;
    for(u8 address = 0; address < 0xff; address++) {
        I2C::begin_transmission(address);

        if(I2C::end_transmission(false) == 0) {
            rune addr[3];
            u8_to_hexstring(address, addr);

            IO::print(F("  I2C device found at address 0x"));
            IO::print(addr);
            IO::println(F("."));

            ndevices++;
        }
    }

    if(ndevices == 0)
        log(ERROR, F("No I2C devices found."));
    log(INFO, F("Scanning I2C devices done!"));
}

i32 main() {
    log(INFO, F("Booting up Jessy OS..."));

    check_sys_folders();
    scan_i2c_devs();

    log(INFO, F("Boot up done!"));
    Sys::delay(3500);
    Sys::shellexec(F("clear"), 0,  (char**) nil);

    return 0;
}