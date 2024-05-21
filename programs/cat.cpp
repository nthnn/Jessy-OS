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
    if(Args::count() < 2) {
        IO::println(F("Usage:\r\n  cat <files...>"));
        return 1;
    }

    for(u8 i = 1; i < Args::count(); i++) {
        File file = File::open(Args::value(i), F("r"));
        string contents = (string) Memory::alloc(file.size() + 1);
        i32 data, idx = 0;

        while((data = file.read()) != -1)
            contents[idx++] = (rune) data;

        contents[idx] = '\0';
        file.close();

        IO::println(contents);
    }

    return 0;
}