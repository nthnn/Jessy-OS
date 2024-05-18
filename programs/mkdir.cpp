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
    if(Args::count() == 1) {
        IO::print(F("Usage:\r\n  mkdir <path>\r\n"));
        return -1;
    }

    for(i32 i = 1; i < Args::count(); i++) {
        string path = Args::value(i);

        if(!FS::mkdir(path)) {
            IO::printf(
                F("Cannot create folder: {s}\r\n"),
                path
            );

            break;
        }
    }

    return 0;
}