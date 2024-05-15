/* 
 * This file is part of the Jessy OS distribution (https://github.com/nthnn/Jessy-OS).
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

#ifndef JESSY_OS_CONFIGS_H
#define JESSY_OS_CONFIGS_H

#include <SPI.h>

#define TFT_CS     5            // TFT SPI select pin
#define TFT_SCK    18           // TFT SPI clock pin
#define TFT_MOSI   23           // TFT SPI MOSI pin
#define TFT_DC     15           // TFT data/command pin
#define TFT_RESET  4            // TFT reset pin
#define TFT_SPIBUS VSPI_HOST    // TFT SPI bus

#define SD_CS      2            // SD card chip select pin
#define SD_SCK     14           // SD card SPI clock pin
#define SD_MOSI    13           // SD card SPI MOSI pin
#define SD_MISO    12           // SD card SPI MISO pin

#endif