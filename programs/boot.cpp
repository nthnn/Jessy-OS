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

i32 main() {
    IO::print(F("Booting up Jessy OS..."));

    if(FS::exists("~/bin")) {
        FS::mkdir(F("~/bin"));
        IO::print(F("Folder ~/bin not found, created one."));
    }

    if(FS::exists("~/etc")) {
        FS::mkdir(F("~/etc"));
        IO::print(F("Folder ~/etc not found, created one."));
    }

    if(FS::exists("~/home")) {
        FS::mkdir(F("~/home"));
        IO::print(F("Folder ~/home not found, created one."));
    }

    if(FS::exists("~/man")) {
        FS::mkdir(F("~/man"));
        IO::print(F("Folder ~/man not found, created one."));
    }

    if(FS::exists("~/tmp")) {
        FS::mkdir(F("~/tmp"));
        IO::print(F("Folder ~/tmp not found, created one."));
    }

    IO::print(F("Boot up done!"));
    Sys::delay(3000);

    return 0;
}