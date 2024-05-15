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

void splitString(const String& input, char** tokens, int maxTokens, int &count) {
    int tokenCount = 0, tokenStart = 0;
    bool inQuotes = false;

    for(int i = 0; i < input.length(); i++) {
        if(input[i] == '"')
            inQuotes = !inQuotes;
        else if(input[i] == ' ' && !inQuotes) {
            if(tokenCount < maxTokens) {
                input.substring(tokenStart, i)
                    .toCharArray(tokens[tokenCount], i - tokenStart + 1);

                tokens[tokenCount++][i - tokenStart] = '\0';
                tokenStart = i + 1;
                count++;
            }
            else break;
        }
    }

    if(tokenCount < maxTokens && tokenStart < input.length()) {
        input.substring(tokenStart).toCharArray(
            tokens[tokenCount],
            input.length() - tokenStart + 1
        );

        tokens[tokenCount++][input.length() - tokenStart] = '\0';
        count++;
    }
}

void setup() {
    JessyOS::initDevices();
    JessyOS::initTerminal();
    JessyOS::initSDCard();
    JessyOS::initPSRAM();

    JessyOS::startVM(vm);
    JessyOS::loadBoot(vm);

    JessyOS::print("\e[32m[\e[97m" +
        vm->getWorkingDirectory() +
        "\e[97m\e[32m]~\e[97m "
    );
}

void loop() {
    String input = JessyOS::readLine();
    if(input == "") {
        JessyOS::print(String("\e[32m[\e[97m") +
            vm->getWorkingDirectory() +
            "\e[97m\e[32m]~\e[97m "
        );

        return;
    }

    char* tokens[10];
    int count = 0;

    for(int i = 0; i < 10; i++)
        tokens[i] = (char*) malloc(50 * sizeof(char));
    splitString(input, tokens, 10, count);

    if(!vm->loadFile(tokens[0])) {
        JessyOS::println("Failed to \e[94mload\e[97m specified file: " +
          String(tokens[0]));

        for(int i = 0; i < 10; i++)
            free(tokens[i]);

        vm->reset();

        JessyOS::print(String("\e[32m[\e[97m") +
            vm->getWorkingDirectory() + "\e[97m\e[32m]~\e[97m ");
        return;
    }

    vm->run(count, tokens);
    vm->reset();

    for(int i = 0; i < 10; i++)
        free(tokens[i]);

    JessyOS::print(String("\e[32m[\e[97m") +
        vm->getWorkingDirectory() + "\e[97m\e[32m]~\e[97m ");
}
