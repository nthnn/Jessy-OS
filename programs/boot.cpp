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

void check_sys_folders() {
   if(!FS::exists("~/bin")) {
        FS::mkdir(F("~/bin"));
        IO::print(F("Folder ~/bin not found, created one.\r\n"));
    }

    if(!FS::exists("~/etc")) {
        FS::mkdir(F("~/etc"));
        IO::print(F("Folder ~/etc not found, created one.\r\n"));
    }

    if(!FS::exists("~/home")) {
        FS::mkdir(F("~/home"));
        IO::print(F("Folder ~/home not found, created one.\r\n"));
    }

    if(!FS::exists("~/man")) {
        FS::mkdir(F("~/man"));
        IO::print(F("Folder ~/man not found, created one.\r\n"));
    }

    if(!FS::exists("~/tmp")) {
        FS::mkdir(F("~/tmp"));
        IO::print(F("Folder ~/tmp not found, created one.\r\n"));
    }

    IO::print(F("Checking system folders done!\r\n"));
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

            IO::print(F("I2C device found with address 0x"));
            IO::print(addr);
            IO::print(F(".\r\n"));

            ndevices++;
        }
    }

    if(ndevices == 0)
        IO::print(F("No I2C devices found.\r\n"));
    IO::print(F("Scanning I2C devices done!\r\n"));
}

i32 main() {
    IO::print(F("Booting up Jessy OS...\r\n"));

    check_sys_folders();
    scan_i2c_devs();

    IO::print(F("Boot up done!\r\n"));
    Sys::delay(3000);
    Sys::shellexec(F("clear"), 0,  (char**) nil);

    return 0;
}