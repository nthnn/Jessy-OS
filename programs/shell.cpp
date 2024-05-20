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

void shell_print() {
    IO::print(F("["), TERM_FG_GREEN, TERM_BG_BLACK, TERM_STYLE_BOLD);
    IO::print(Sys::working_dir());
    IO::print(F("]~ "), TERM_FG_GREEN, TERM_BG_BLACK, TERM_STYLE_BOLD);
}

i32 main() {
    while(true) {
        shell_print();

        string cmd = IO::readline();
        if(cmd == "")
            continue;
        else if(cmd == "exit")
            Sys::exit(0);

        Sys::shellexec(cmd);
    }

    return 0;
}