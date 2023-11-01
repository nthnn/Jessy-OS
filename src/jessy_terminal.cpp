/*
 * This file is part of the Jessy OS.
 * Copyright (c) 2023 Nathanne Isip
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <ESP32Ping.h>
#include <HTTPClient.h>
#include <SD.h>
#include <WiFi.h>

#include "jessy_agent.h"
#include "jessy_bios.h"
#include "jessy_io.h"
#include "jessy_js.h"
#include "jessy_terminal.h"
#include "jessy_util.h"

#ifdef __cplusplus
    extern "C" {
#endif

uint8_t temprature_sens_read();

#ifdef __cplusplus
    }
#endif

uint8_t temprature_sens_read();

static void printCommandError(String cmd, String message) {
    JessyUtility::log(JSY_LOG_ERROR, cmd + F(": ") + message);
}

static void printIncorrectArity(String cmd) {
    printCommandError(cmd,
        "Incorrect parameter arity."
        "\n    Type: help " + cmd
    );
}

static String encryptionString(wifi_auth_mode_t enctype) {
    switch(enctype) {
        case WIFI_AUTH_OPEN:
            return F("Open            ");

        case WIFI_AUTH_WEP:
            return F("WEP             ");

        case WIFI_AUTH_WPA_PSK:
            return F("WPA-PSK         ");

        case WIFI_AUTH_WPA2_PSK:
            return F("WPA2-PSK        ");

        case WIFI_AUTH_WPA_WPA2_PSK:
            return F("WPA/WPA2-PSK    ");

        case WIFI_AUTH_WPA2_ENTERPRISE:
            return F("WPA2-ENTERPRISE ");

        default:
            return F("Unknown         ");
    }
}

void JessyTerminal::cd(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    if(!agent.setWorkingDirectory(JessyUtility::sanitizePath(agent, arguments[1])))
        printCommandError(F("cd"), F("Cannot change working directory."));
}

void JessyTerminal::pwd(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 1) {
        printIncorrectArity(arguments[0]);
        return;
    }

    JessyIO::println(agent.getWorkingDirectory());
}

void JessyTerminal::ls(JessyAgent &agent, String arguments[], uint8_t argc) {
    bool isRecursive = false, includeHiddenFiles = false;
    for(uint8_t i = 1; i < argc; i++) {
        String arg = arguments[i];

        if(arg == F("-r") || arg == F("--recursive"))
            isRecursive = true;
        else if(arg == F("-a") || arg == F("--all"))
            includeHiddenFiles = true;
    }

    String files[300];
    uint16_t count;

    if(isRecursive)
        count = JessyIO::listFilesRecursive(agent.getWorkingDirectory(), files);
    else count = JessyIO::listFiles(agent.getWorkingDirectory(), files);

    for(uint16_t i = 0; i < count; i++) {
        String fileName = files[i];
        bool isFile = JessyIO::isFile(fileName);

        JessyRTC lastWrite = JessyIO::getLastWrite(fileName);
        String actualFilename = fileName.substring(
            agent.getWorkingDirectory().length() + 1
        );

        if(!includeHiddenFiles && actualFilename.startsWith(F("."))) 
            continue;

        JessyIO::print(isFile ? F("F ") : F("D "));
        JessyIO::print("[" + JessyUtility::rtcStructString(lastWrite) + "]\t");
        JessyIO::println(actualFilename);
    }
}

void JessyTerminal::mkdir(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc < 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    for(uint8_t i = 1; i < argc; i++) {
        String path = arguments[i],
            actualPath = JessyUtility::sanitizePath(agent, path);
        if(!JessyIO::mkdir(actualPath))
            printCommandError(arguments[0], "Cannot create directory: " + path);
    }
}

void JessyTerminal::touch(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc < 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    for(uint8_t i = 1; i < argc; i++) {
        String path = arguments[i],
            actualPath = JessyUtility::sanitizePath(agent, path);
        if(!JessyIO::writeFile(actualPath, "\r"))
            printCommandError(arguments[0], "Cannot create file: " + path);
    }
}

void JessyTerminal::rm(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc < 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    bool verbose = false;
    for(uint8_t i = 1; i < argc; i++)
        if(arguments[i] == F("-v") || arguments[i] == F("--verbose"))
            verbose = true;

    for(uint8_t i = 1; i < argc; i++)
        if(arguments[i] == F("-v") || arguments[i] == F("--verbose"))
            continue;
        else {
            String file = arguments[i],
                actualFile = JessyUtility::sanitizePath(agent, file);

            if(verbose)
                JessyUtility::log(JSY_LOG_PLAIN, "Deleting " + file);

            if(JessyIO::isFile(actualFile) && !JessyIO::deleteFile(actualFile))
                JessyUtility::log(JSY_LOG_ERROR, "Cannot delete file: " + file);
            else if(JessyIO::isDirectory(actualFile)) {
                String files[100];
                uint16_t count = JessyIO::listFilesRecursive(actualFile, files);

                for(uint16_t i = 0; i < count; i++)
                    if(JessyIO::isFile(files[i]) && !JessyIO::deleteFile(files[i])) {
                        JessyUtility::log(JSY_LOG_ERROR, "Cannot delete file: " + files[i]);
                        return;
                    }

                for(uint16_t i = 0; i < count; i++)
                    if(JessyIO::isDirectory(files[i]) && !JessyIO::rmdir(files[i])) {
                        JessyUtility::log(JSY_LOG_ERROR, "Cannot delete file: " + files[i]);
                        return;
                    }

                if(!JessyIO::rmdir(actualFile))
                    JessyUtility::log(JSY_LOG_ERROR, "Cannot delete directory: " + file);
            }
        }
}

void JessyTerminal::cp(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 3) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String source = JessyUtility::sanitizePath(agent, arguments[1]),
        dest = JessyUtility::sanitizePath(agent, arguments[2]);
    if(source == dest)
        return;

    if(!JessyIO::exists(source)) {
        printCommandError(arguments[0], "Source file doesn't exist.");
        return;
    }
    else if(JessyIO::exists(dest) && JessyIO::isFile(dest)) {
        printCommandError(arguments[0], "Destination file already exists.");
        return;
    }

    if(JessyIO::isFile(source) &&
        !JessyIO::writeFile(dest, JessyIO::readFile(source))) {
        printCommandError(arguments[0], F("Something went wrong while copying file."));
        return;
    }
    else {
        String files[100];
        for(uint16_t i = 0; i < JessyIO::listFiles(source, files); i++) {
            String output = files[i],
                original = output;
            output.replace(source, dest);

            if(JessyIO::isFile(original))
                JessyIO::writeFile(
                    output,
                    JessyIO::readFile(files[i])
                );
            else if(JessyIO::isDirectory(original)) {
                JessyIO::mkdir(output);

                String wd = agent.getWorkingDirectory();
                original.replace(wd, F(""));
                output.replace(wd, F(""));

                String args[] = {"cp", original, output};
                JessyTerminal::cp(agent, args, 3);
            }
        }
    }
}

void JessyTerminal::mv(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 3) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String source = arguments[1], dest = arguments[2];
    if(JessyUtility::sanitizePath(agent, source) ==
        JessyUtility::sanitizePath(agent, dest))
        return;

    String cpArgs[] = {"cp", source, dest};
    String rmArgs[] = {"rm", source};

    JessyTerminal::cp(agent, cpArgs, 3);
    JessyTerminal::rm(agent, rmArgs, 2);
}

void JessyTerminal::cat(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String file = JessyUtility::sanitizePath(agent, arguments[1]);
    if(!JessyIO::exists(file)) {
        printCommandError(arguments[0], F("File doesn't exist."));
        return;
    }
    else if(!JessyIO::isFile(file)) {
        printCommandError(arguments[0], F("Input path is directory."));
        return;
    }

    JessyIO::println(JessyIO::readFile(file));
}

void JessyTerminal::echo(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc < 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    for(uint16_t i = 1; i < argc; i++)
        JessyIO::println(arguments[i]);
}

void JessyTerminal::clear(JessyAgent &agent, String arguments[], uint8_t argc) {
    JessyIO::clearScreen();
}

void JessyTerminal::useradd(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 3) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String user = arguments[1],
        user64 = JessyUtility::toBase64(user),
        userFile = "/sys/users/" + user64,
        password = arguments[2];

    if(user.length() < 4 || password.length() < 4) {
        printCommandError(
            arguments[0],
            F("Username and password must be at least 4 characters.")
        );

        return;
    }

    if(user == "anonymous" || user == "shared") {
        printCommandError(arguments[0], F("Username is a special keyword."));
        return;
    }

    if(JessyIO::exists(userFile) && JessyIO::isFile(userFile)) {
        printCommandError(arguments[0], "Username already in use.");
        return;
    }

    JessyIO::writeFile(
        userFile,
        JessyUtility::aesEncrypt(user64, password)
    );

    user.toLowerCase();
    JessyIO::mkdir("/root/" + user);
}

void JessyTerminal::userdel(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 3) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String userdel = arguments[0];
    if(agent.getName() == F("anonymous")) {
        printCommandError(userdel, F("Anonymous user cannot delete another user."));
        return;
    }

    String user = arguments[1],
        user64 = JessyUtility::toBase64(user),
        password = arguments[2];
    String userFile = "/sys/users/" + user64;

    if(user.length() < 4 || password.length() < 4) {
        printCommandError(
            arguments[0],
            F("Username and password must be at least 4 characters.")
        );

        return;
    }

    if(JessyIO::exists(userFile) && 
        JessyIO::isFile(userFile) &&
            JessyIO::readFile(userFile).equals(
                JessyUtility::aesEncrypt(user64, password)
            )) {
        user.toLowerCase();

        String files[256];
        uint16_t count = JessyIO::listFilesRecursive("/root/" + user, files);

        for(uint16_t i = 0; i < count; i++)
            if(JessyIO::isFile(files[i])) {
                if(!JessyIO::deleteFile(files[i]))
                    printCommandError(
                        arguments[0],
                        "Cannot delete file: " + files[i]
                    );
            }

        for(uint16_t i = count; i > 0; i--)
            if(JessyIO::isDirectory(files[i])) {
                if(!JessyIO::rmdir(files[i]))
                    printCommandError(
                        arguments[0],
                        "Cannot delete directory: " + files[i]
                    );
            }

        if(!JessyIO::rmdir("/root/" + user)) {
            printCommandError(arguments[0], F("Cannot remove user root folder."));
            return;
        }

        if(!JessyIO::deleteFile("/sys/users/" + user64)) {
            printCommandError(arguments[0], F("Cannot remove user account."));
            return;
        }
        user.toLowerCase();

        if(user == agent.getName()) {
            String args[] = {"su", "-o"};
            JessyTerminal::su(agent, args, 2);
        }
        return;
    }

    printCommandError(userdel, F("Incorrect user credentials."));
}

void JessyTerminal::passwd(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 4) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String passwd = arguments[0];
    if(agent.getName() == F("anonymous")) {
        printCommandError(passwd, F("Anonymous user cannot edit another user."));
        return;
    }

    String user = arguments[1],
        user64 = JessyUtility::toBase64(user),
        password = arguments[2],
        newpass = arguments[3];
    String userFile = "/sys/users/" + user64;

    if(user.length() < 4 ||
        password.length() < 4 ||
        newpass.length() < 4) {
        printCommandError(
            arguments[0],
            F("Username and passwords must be at least 4 characters.")
        );

        return;
    }

    if(JessyIO::exists(userFile) && 
        JessyIO::isFile(userFile) &&
            JessyIO::readFile(userFile).equals(
                JessyUtility::aesEncrypt(user64, password)
            )) {
        JessyIO::writeFile(
            userFile,
            JessyUtility::aesEncrypt(user64, newpass)
        );
        return;
    }

    printCommandError(passwd, F("Incorrect user credentials."));
}

void JessyTerminal::su(JessyAgent &agent, String arguments[], uint8_t argc) {
    String su = arguments[0];

    if(argc == 2 &&
        (arguments[1] == "-o" ||
            arguments[1] == "--out")) {
        if(agent.getName() == "anonymous")
            printCommandError(su, "Already a guess user.");
        else agent.anonymous();

        return;
    }
    else if(argc == 3) {
        String user = arguments[1],
            user64 = JessyUtility::toBase64(user),
            password = arguments[2];
        String userFile = "/sys/users/" + user64;

        if(JessyIO::exists(userFile) && 
            JessyIO::isFile(userFile) &&
                JessyIO::readFile(userFile).equals(
                    JessyUtility::aesEncrypt(user64, password)
                )) {
            if(WiFi.status() == WL_CONNECTED)
                WiFi.disconnect();
            user.toLowerCase();

            agent.setName(user);
            agent.setWorkingDirectory("/root/" + user);

            JessyIO::clearScreen();
            return;
        }

        if(JessyIO::exists(userFile))
            printCommandError(su, F("Incorrect user credentials."));
        else printCommandError(su, F("User not found."));
    }
    else printIncorrectArity(su);
}

void JessyTerminal::sd(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 1) {
        printIncorrectArity(arguments[0]);
        return;
    }

    sdcard_type_t type = SD.cardType();
    JessyIO::print("Card type:\t\t");

    switch(type) {
        case CARD_MMC:
            JessyIO::println("Multimedia Card");
            break;

        case CARD_SD:
            JessyIO::println("Secure Digital");
            break;

        case CARD_SDHC:
            JessyIO::println("SDHC");
            break;

        default:
            JessyIO::println("Unknown");
            break;
    }

    JessyIO::println("Card size:\t\t" + String(SD.cardSize() / 1048576) + "MB");
    JessyIO::println("Total bytes:\t\t" + String(SD.totalBytes() / 1048576) + "MB");
    JessyIO::println("Used bytes:\t\t" + String(SD.usedBytes() / 1048576) + "MB");
    JessyIO::println("Number of sectors:\t" + String(SD.numSectors()));
    JessyIO::println("Sector size:\t\t" + String(SD.sectorSize()));
}

void JessyTerminal::esp32cpu(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 1) {
        printIncorrectArity(arguments[0]);
        return;
    }

    JessyIO::println(String(F("Chip cores:\t\t")) + ESP.getChipCores());
    JessyIO::println(String(F("Chip model:\t\t")) + ESP.getChipModel());
    JessyIO::println(String(F("Chip revision:\t\t")) + ESP.getChipRevision());
    JessyIO::println(String(F("CPU frequency:\t\t")) + ESP.getCpuFreqMHz());
    JessyIO::println(String(F("Cycle count:\t\t")) + ESP.getCycleCount());
    JessyIO::println(String(F("Chip ID:\t\t")) + ESP.getEfuseMac());
    JessyIO::println(String(F("Flash chip mode:\t")) + ESP.getFlashChipMode());
    JessyIO::println(String(F("Flash chip size:\t")) + ESP.getFlashChipSize());
    JessyIO::println(String(F("Flash chip speed:\t")) + ESP.getFlashChipSpeed());
    JessyIO::println(String(F("Free heap:\t\t")) + ESP.getFreeHeap());
    JessyIO::println(String(F("Free PSRAM:\t\t")) + ESP.getFreePsram());
    JessyIO::println(String(F("Flash sketch space:\t")) + ESP.getFreeSketchSpace());
    JessyIO::println(String(F("Heap size:\t\t")) + ESP.getHeapSize());
    JessyIO::println(String(F("Max alloc heap:\t\t")) + ESP.getMaxAllocHeap());
    JessyIO::println(String(F("Max alloc PSRAM:\t")) + ESP.getMaxAllocPsram());
    JessyIO::println(String(F("Min free heap:\t\t")) + ESP.getMinFreeHeap());
    JessyIO::println(String(F("Min free PSRAM:\t\t")) + ESP.getMinFreePsram());
    JessyIO::println(String(F("PSRAM Size:\t\t")) + ESP.getPsramSize());
    JessyIO::println(String(F("SDK version:\t\t")) + ESP.getSdkVersion());
    JessyIO::println(String(F("Sketch MD5:\t\t")) + ESP.getSketchMD5());
    JessyIO::println(String(F("Sketch size:\t\t")) + ESP.getSketchSize());
    JessyIO::println(String(F("Internal temperature:\t")) +
        String((temprature_sens_read() - 32) / 1.8) + " C"
    );
}

void JessyTerminal::reboot(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 1) {
        printIncorrectArity(arguments[0]);
        return;
    }

    ESP.restart();
}

void JessyTerminal::gpio(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc < 2 || argc > 4) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String act = arguments[1];
    uint8_t pin = (uint8_t) arguments[2].toInt();

    if(argc == 3) {
        if(act == "on") {
            pinMode(pin, OUTPUT);
            digitalWrite(pin, HIGH);
        }
        else if(act == "off") {
            pinMode(pin, OUTPUT);
            digitalWrite(pin, LOW);
        }
        else if(act == "read")
            JessyIO::println(pin + ": " + String(digitalRead(pin)));
        else printCommandError(arguments[0], F("Invalid action for GPIO."));
    }
    else if(argc == 4 && act == "write")
        digitalWrite(pin, (byte) arguments[3].toInt());
}

void JessyTerminal::date(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 1) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String dateTime = JessyUtility::getRTCString(
        JessyUtility::createClock().now()
    );
    dateTime = dateTime.substring(0, dateTime.indexOf(' '));

    JessyIO::println(dateTime);
}

void JessyTerminal::time(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 1) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String dateTime = JessyUtility::getRTCString(
        JessyUtility::createClock().now()
    );
    dateTime = dateTime.substring(dateTime.indexOf(' ') + 1);

    JessyIO::println(dateTime);
}

void JessyTerminal::ping(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(WiFi.status() != WL_CONNECTED) {
        printCommandError(arguments[0], F("Not connect to any network."));
        return;
    }

    #define sendPing(count) for(uint8_t i = 0; i < count ; i++) {   \
        if(Ping.ping(host.c_str()))                                 \
            JessyIO::println(                                       \
                "[" + String(i + 1) +                               \
                "] Packet sent to " + host                          \
            );                                                      \
        else JessyIO::println(                                      \
            "[" + String(i + 1) +                                   \
            "] Packet not sent to " + host                          \
        );                                                          \
    }

    if(argc == 2) {
        String host = arguments[1];

        sendPing(3);
        return;
    }
    else if(argc == 3) {
        String host = arguments[1];
        uint8_t count = (uint8_t) arguments[2].toInt();

        sendPing(count);
        return;
    }

    printIncorrectArity(arguments[0]);
    #undef sendPing
}

void JessyTerminal::wlan(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc == 2) {
        String command = arguments[1];
        if(command == F("disconnect")) {
            WiFi.disconnect();
            JessyUtility::log(JSY_LOG_SUCCESS, F("Disconnected to WLAN!"));

            return;
        }
        else if(command == F("status")) {
            JessyIO::print(F("WLAN Status:\t"));
            switch(WiFi.status()) {
                case WL_CONNECT_FAILED:
                    JessyIO::println(F("Connect failed."));
                    break;
                
                case WL_CONNECTED:
                    JessyIO::println(F("Connected."));
                    break;

                case WL_CONNECTION_LOST:
                    JessyIO::println(F("Connection lost."));
                    break;

                case WL_DISCONNECTED:
                    JessyIO::println(F("Disconnected."));
                    break;

                case WL_IDLE_STATUS:
                    JessyIO::println(F("Idle status."));
                    break;

                case WL_NO_SHIELD:
                    JessyIO::println(F("No shield."));
                    break;

                case WL_NO_SSID_AVAIL:
                    JessyIO::println(F("SSID available"));
                    break;

                case WL_SCAN_COMPLETED:
                    JessyIO::println(F("Scan completed."));
                    break;

                default:
                    JessyIO::println(F("Unknown."));
                    break;
            }

            JessyIO::println("RSSI:\t\t" + String(WiFi.RSSI()) + "dBm");
            return;
        }
        else if(command == "info") {
            if(WiFi.status() != WL_CONNECTED) {
                printCommandError(arguments[0], F("Not connected to any network."));
                return;
            }

            JessyIO::println("Broadcast IP:\t" + WiFi.broadcastIP().toString());
            JessyIO::println("BSSID:\t\t" + WiFi.BSSIDstr());
            JessyIO::println("DNS IP:\t\t" + WiFi.dnsIP().toString());
            JessyIO::println("Gateway IP:\t" + WiFi.gatewayIP().toString());
            JessyIO::println("Local IP:\t" + WiFi.localIP().toString());
            JessyIO::println("Local IPv6:\t" + WiFi.localIPv6().toString());
            JessyIO::println("Mac Address:\t" + WiFi.macAddress());
            JessyIO::println("Subnet mask:\t" + WiFi.subnetMask().toString());

            return;
        }
        else if(command == "scan") {
            int16_t n = WiFi.scanNetworks();
            if(n == 0) {
                JessyUtility::log(JSY_LOG_WARNING, "No networks found.");
                return;
            }

            for(int16_t i = 0; i < n; i++) {
                JessyIO::print("[   " + String(i + 1) + "]: ");
                JessyIO::print(encryptionString(WiFi.encryptionType(i)) + "\t");
                JessyIO::print(String(WiFi.RSSI(i)) + "dBm\t");
                JessyIO::println(WiFi.SSID(i));

                delay(10);
            }

            return;
        }
    }
    else if(argc == 3 && arguments[1] == "mode") {
        String mode = arguments[2];

        if(mode == "station") {
            WiFi.mode(WIFI_STA);
            return;
        }
        else if(mode == "ap") {
            WiFi.mode(WIFI_AP);
            return;
        }
        else if(mode == "ap-station") {
            WiFi.mode(WIFI_AP_STA);
            return;
        }

        printCommandError(arguments[0], F("Unknown WiFi mode."));
        return;
    }
    else if(argc == 4 && arguments[1] == "config") {
        String config = arguments[2];

        IPAddress localIP = WiFi.localIP();
        IPAddress gateway = WiFi.gatewayIP();
        IPAddress subnet = WiFi.subnetMask();

        if(config == "ip") {
            if(!localIP.fromString(arguments[3])){
                JessyUtility::log(JSY_LOG_ERROR, F("Invalid IP address string."));
                return;
            }

            if(!WiFi.config(localIP, gateway, subnet))
                printCommandError(arguments[0], F("Cannot configure local IP."));
            return;
        }
        else if(config == "gateway") {
            if(!gateway.fromString(arguments[3])){
                JessyUtility::log(JSY_LOG_ERROR, F("Invalid gateway address string."));
                return;
            }

            if(!WiFi.config(localIP, gateway, subnet))
                printCommandError(arguments[0], F("Cannot configure gateway."));
            return;
        }
        else if(config == "subnet") {
            if(!subnet.fromString(arguments[3])){
                JessyUtility::log(JSY_LOG_ERROR, F("Invalid subnet address string."));
                return;
            }

            if(!WiFi.config(localIP, gateway, subnet))
                printCommandError(arguments[0], F("Cannot configure subnet."));
            return;
        }

        printCommandError(arguments[0], F("Invalid config command."));
    }
    else if(argc == 5 && arguments[1] == "config" && arguments[2] == "dns") {
        IPAddress localIP = WiFi.localIP();
        IPAddress gateway = WiFi.gatewayIP();
        IPAddress subnet = WiFi.subnetMask();
        IPAddress primary, secondary;

        if(!primary.fromString(arguments[3])){
            JessyUtility::log(JSY_LOG_ERROR, F("Invalid primary DNS address string."));
            return;
        }

        if(!secondary.fromString(arguments[4])){
            JessyUtility::log(JSY_LOG_ERROR, F("Invalid secondary DNS address string."));
            return;
        }

        if(!WiFi.config(localIP, gateway, subnet, primary, secondary))
            printCommandError(arguments[0], F("Cannot configure DNS addresses."));
        return;
    }
    else if(argc == 4 && arguments[1] == "connect") {
        String ssid = arguments[2], password = arguments[3];

        WiFi.setHostname((agent.getName() + "@jsy").c_str());
        WiFi.begin(ssid, password);
        JessyUtility::log(JSY_LOG_PLAIN, "Connecting to SSID: " + ssid);

        long marker = millis();
        while(WiFi.status() != WL_CONNECTED) {
            delay(100);

            if(millis() - marker > 20000) {
                JessyUtility::log(JSY_LOG_ERROR, F("Cannot connect to WLAN."));
                return;
            }
        }

        JessyUtility::log(JSY_LOG_SUCCESS, "Connected!");
        return;
    }

    printIncorrectArity(arguments[0]);
}

void JessyTerminal::js(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc == 3 && arguments[1] == "eval") {
        JessyDuktape::init();
        JessyDuktape::evalString(arguments[2]);
        JessyDuktape::free();

        JessyIO::println();
        return;
    }
    else if(argc == 2) {
        String inputFile = JessyUtility::sanitizePath(agent, arguments[1]);
        if(!JessyIO::exists(inputFile) || !JessyIO::isFile(inputFile)) {
            printCommandError(arguments[0], "Input script doesn't exist.");
            return;
        }

        JessyDuktape::init();
        JessyDuktape::eval(inputFile);
        JessyDuktape::free();

        JessyIO::println();
        return;
    }

    printIncorrectArity(arguments[0]);
}

void JessyExecCommand(JessyAgent &agent, String arguments[], uint8_t argc) {
    String cmd = arguments[0];

    #define JSY_EXEC(cmd) JessyTerminal:: cmd (agent, arguments, argc);

    if(argc == 0)                   return;
    else if(cmd == F("cd"))         JSY_EXEC(cd)
    else if(cmd == F("pwd"))        JSY_EXEC(pwd)
    else if(cmd == F("ls"))         JSY_EXEC(ls)
    else if(cmd == F("mkdir"))      JSY_EXEC(mkdir)
    else if(cmd == F("touch"))      JSY_EXEC(touch)
    else if(cmd == F("rm"))         JSY_EXEC(rm)
    else if(cmd == F("cp"))         JSY_EXEC(cp)
    else if(cmd == F("mv"))         JSY_EXEC(mv)
    else if(cmd == F("cat"))        JSY_EXEC(cat)
    else if(cmd == F("echo"))       JSY_EXEC(echo)
    else if(cmd == F("clear"))      JSY_EXEC(clear)
    else if(cmd == F("useradd"))    JSY_EXEC(useradd)
    else if(cmd == F("userdel"))    JSY_EXEC(userdel)
    else if(cmd == F("passwd"))     JSY_EXEC(passwd)
    else if(cmd == F("su"))         JSY_EXEC(su)
    else if(cmd == F("sd"))         JSY_EXEC(sd)
    else if(cmd == F("esp32cpu"))   JSY_EXEC(esp32cpu)
    else if(cmd == F("reboot"))     JSY_EXEC(reboot)
    else if(cmd == F("gpio"))       JSY_EXEC(gpio)
    else if(cmd == F("date"))       JSY_EXEC(date)
    else if(cmd == F("time"))       JSY_EXEC(time)
    else if(cmd == F("wlan"))       JSY_EXEC(wlan)
    else if(cmd == F("ping"))       JSY_EXEC(ping)
    else if(cmd == F("js"))         JSY_EXEC(js)
    else JessyUtility::log(
        JSY_LOG_ERROR,
        "Command not found: " + arguments[0]
    );

    #undef JSY_EXEC
}