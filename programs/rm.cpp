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
        IO::println(F("Usage:"));
        IO::println(F("  rm <filenames...>"));
        IO::println(F("  rm <folders...>"));

        return 1;
    }

    for(u8 i = 1; i < Args::count(); i++) {
        string path = Args::value(i);

        if(!FS::exists(path)) {
            IO::printf(
                F("Not {s}found{s}: {s}\r\n"),
                TERM_FG_HRED,
                TERM_FG_HWHITE,
                path
            );
            continue;
        }

        File file = File::open(path, F("n"));
        if(file.is_dir()) {
            if(FS::rmdir(path))
                IO::printf(
                    F("Folder deleted: {s}\r\n"),
                    file.name()
                );
            else IO::printf(
                F("Cannot {s}delete{s} folder: {s}\r\n"),
                TERM_FG_HRED,
                TERM_FG_HWHITE,
                file.name()
            );
        }
        else if(file.is_file()) {
            if(FS::remove(path))
                IO::printf(
                    F("File deleted: {s}\r\n"),
                    file.name()
                );
            else IO::printf(
                F("Cannot {s}delete{s} file: {s}\r\n"),
                TERM_FG_HRED,
                TERM_FG_HWHITE,
                file.name()
            );
        }

        file.close();
    }
}