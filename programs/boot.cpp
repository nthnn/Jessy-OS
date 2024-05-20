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

            IO::printf(
                F("  I2C device found at address 0x{u}.\r\n"),
                addr
            );
            ndevices++;
        }
    }

    if(ndevices == 0)
        log(ERROR, F("No I2C devices found."));
    log(INFO, F("Scanning I2C devices done!"));
}

void check_ps2_keyboard() {
    if(Keyboard::device_type() == PS2_DEV_UNKNOWN) {
        log(ERROR, F("Invalid or no PS/2 keyboard found."));
        Sys::exit(-1);
    }

    log(INFO, F("PS/2 keyboard detected!"));

    IO::printf(
        F("  Layout name: {s}\r\n"),
        Keyboard::layout_name()
    );
    IO::printf(
        F("  Layout desc: {s}\r\n"),
        Keyboard::layout_desc()
    );
}

void display_info() {
    log(INFO, F("Logging display info..."));

    IO::printf(
        F("  Screen size:\t{i}x{i}\r\n"),
        Display::screen_width(),
        Display::screen_height()
    );

    IO::printf(
        F("  Viewport:\t{i}x{i}\r\n"),
        Display::viewport_width(),
        Display::viewport_height()
    );

    IO::printf(
        F("  Colors:\t{i}\r\n"),
        Display::supported_colors()
    );
}

i32 main() {
    log(INFO, F("Booting up Jessy OS..."));

    display_info();
    check_sys_folders();
    scan_i2c_devs();
    check_ps2_keyboard();

    log(INFO, F("Boot up done!"));
 
    Sys::delay(3500);
    Sys::shellexec(F("clear"));
    Sys::shellexec(F("shell"));

    return 0;
}