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
        IO::println(F("Usage:\r\n  touch <filename>"));
        return 1;
    }

    for(u8 i = 1; i < Args::count(); i++) {
        string filename = Args::value(i);
        File output = File::open(filename, F("w"));

        output.write(F(""));
        IO::printf(
            F("File created: {s}{s}{s}\r\n"),
            TERM_FG_CYAN,
            filename,
            TERM_FG_HWHITE
        );
    }

    return 0;
}