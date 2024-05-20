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

#include "jessy_os.h"
#include <rishka.h>

RishkaVM* vm;

void setup() {
    JessyOS::initDevices();
    JessyOS::initTerminal();
    JessyOS::initSDCard();
    JessyOS::initPSRAM();

    vm = new RishkaVM();
    JessyOS::startVM(vm);
    JessyOS::loadBoot(vm);
}

void loop() {
    yield();
    vTaskDelay(10);
}
