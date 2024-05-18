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

void listFiles(File root) {
    while(true) {
        File file = root.next();
        if(!file.is_ok())
            break;

        IO::printf(
            F("{s}{s}\r\n"),
            file.is_dir() ? F("/") : F(""),
            file.name()
        );
        file.close();
    }
}

i32 main() {
    File origin = File::open(Sys::working_dir(), F("n"));
    if(Args::count() == 2)
        origin = File::open(Args::value(1), F("n"));

    if(!origin.is_ok()) {
        IO::print(F("Cannot open specified path.\r\n"));
        return 0;
    }

    if(!origin.is_dir()) {
        IO::print(F("Specified path is not a directory.\r\n"));
        return 0;
    }

    listFiles(origin);
    return 0;
}