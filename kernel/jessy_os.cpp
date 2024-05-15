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
#include "jessy_os_configs.h"

#include <fabgl.h>
#include <SD.h>

fabgl::ILI9341Controller DisplayController;
fabgl::PS2Controller     PS2Controller;
fabgl::Terminal          Terminal;

SPIClass sdSpi(HSPI);

void JessyOS::initDevices() {
    PS2Controller.begin(PS2Preset::KeyboardPort0);

    DisplayController.begin(TFT_SCK, TFT_MOSI, TFT_DC, TFT_RESET, TFT_CS, TFT_SPIBUS);
    DisplayController.setResolution("\"TFT_320x240\" 320 240");
}

void JessyOS::initTerminal() {
    Terminal.begin(&DisplayController);
    Terminal.loadFont(&fabgl::FONT_8x14);
    Terminal.setBackgroundColor(Color::Black);
    Terminal.setForegroundColor(Color::White);
    Terminal.connectLocally();
    Terminal.clear();
    Terminal.enableCursor(true);
}

void JessyOS::initSDCard() {
    sdSpi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

    if(!SD.begin(SD_CS, sdSpi, 80000000)) {
        Terminal.println("Card \e[94mMount\e[97m Failed");

        do sdSpi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
        while(!SD.begin(SD_CS, sdSpi, 80000000));

        Terminal.clear();
    }
}

void JessyOS::initPSRAM() {
    if(!psramInit()) {
        Terminal.println("\e[94mCannot\e[97m initialize PSRAM.");
        while(true);
    }
}

void JessyOS::startVM(RishkaVM* vm) {
    vm = new RishkaVM();
    vm->initialize(&Terminal);

    Terminal.onVirtualKeyItem = [&](VirtualKeyItem * vkItem) {
        if(vkItem->CTRL && vkItem->vk == VirtualKey::VK_c && vm->isRunning()) {
            vm->stopVM();
            vm->reset();

            Terminal.println("^C");
        }
    };
}

void JessyOS::loadBoot(RishkaVM* vm) {
    if(!vm->loadFile("/bin/boot.bin")) {
        Terminal.println("Failed to \e[94mload\e[97m boot binary.");
        while(true);
    }

    vm->run(0, NULL);
    if(vm->getExitCode() != 0)
        while(true);
}

void JessyOS::print(String text) {
    Terminal.print(text);
}

void JessyOS::println(String text) {
    Terminal.println(text);
}

String JessyOS::readLine() {
    fabgl::LineEditor line(&Terminal);
    line.edit();

    return String(line.get());
}