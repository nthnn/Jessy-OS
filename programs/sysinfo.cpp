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

static void print_info_num(string msg, sysinfon_t key);
static void print_info_str(string msg, sysinfos_t key);

void print_sd_info();
void print_chip_info();

i32 main() {
    print_sd_info();
    print_chip_info();

    return 0;
}

static void print_info_num(string msg, sysinfon_t key) {
    IO::print(msg);
    IO::print(Sys::info_num(key));
    IO::print(F("\r\n"));
}

static void print_info_str(string msg, sysinfos_t key) {
    IO::print(msg);
    IO::print(Sys::info_str(key));
    IO::print(F("\r\n"));
}

void print_sd_info() {
    IO::print(F("SD Card Type: "));
    switch(Sys::info_num(SYSINFO_CARD_TYPE)) {
        case SD_CARD_MMC:
            IO::print(F("SD MMC"));
            break;

        case SD_CARD_SD:
            IO::print(F("SD Card"));
            break;

        case SD_CARD_SDHC:
            IO::print(F("SDHC"));
            break;
        
        case SD_CARD_UNKNOWN:
            IO::print(F("Unknown"));
            break;
        
        default:
            IO::print(F("None"));
            break;
    }
    IO::print(F("\r\n"));

    print_info_num(F("Card Size: "), SYSINFO_CARD_SIZE);
    print_info_num(F("No. of Sectors: "), SYSINFO_NUM_SECTORS);
    print_info_num(F("Sector Size: "), SYSINFO_SECTOR_SIZE);
    print_info_num(F("Total Storage: "), SYSINFO_TOTAL_STORAGE);
    print_info_num(F("Used Storage: "), SYSINFO_USED_STORAGE);
}

void print_chip_info() {
    print_info_str(F("Chip Model: "), SYSINFO_CHIPMODEL);
    print_info_str(F("SDK Version: "), SYSINFO_SDK_VERSION);
    print_info_str(F("Sketch MD5: "), SYSINFO_SKETCH_MD5);
    print_info_num(F("PSRAM Size: "), SYSINFO_PSRAM_SIZE);
    print_info_num(F("CPU Frequency (Mhz): "), SYSINFO_CPU_FREQ);
}