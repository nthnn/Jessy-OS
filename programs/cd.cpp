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
    if(Args::count() != 2) {
        IO::print(F("Usage:\r\n\tcd <path>\r\n"));
        return -1;
    }

    string path = Args::value(1);
    if(!Sys::change_dir(path)) {
        IO::print(F("Cannot change directory.\r\n"));
        return -1;
    }

    return 0;
}