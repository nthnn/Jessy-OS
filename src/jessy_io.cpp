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

#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <vfs_api.h>

#include "jessy_io.h"
#include "jessy_util.h"

void JessyIO::print(String text) {
    Serial.print(text);
    delay(3);
}

void JessyIO::println(String text) {
    Serial.println(text);
    delay(3);
}

void JessyIO::clearScreen() {
    JessyIO::println(F("<clear>"));
    Serial.flush();

    delay(100);
}

String JessyIO::scan() {
    Serial.print(F("<~>"));

    while(Serial.available() < 0);
    return Serial.readString();
}

bool JessyIO::mkdir(String path) {
    return SD.mkdir(path);
}

bool JessyIO::rmdir(String path) {
    return SD.rmdir(path);
}

String JessyIO::readFile(String fileName) {
    File file = SD.open(fileName);
    String result = "";

    while(file.available())
        result += (char) file.read();

    file.close();
    return result;
}

bool JessyIO::writeFile(String fileName, String content) {
    File file = SD.open(fileName, FILE_WRITE, true);
    if(!file)
        return false;

    bool success = file.print(content);
    file.close();

    return success;
}

bool JessyIO::appendFile(String fileName, String content) {
    File file = SD.open(fileName, FILE_APPEND);
    if(!file)
        return false;

    bool success = file.print(content);
    file.close();

    return success;
}

bool JessyIO::renameFile(String oldName, String newName) {
    return SD.rename(oldName, newName);
}

bool JessyIO::deleteFile(String fileName) {
    return SD.remove(fileName);
}

size_t JessyIO::getFileSize(String fileName) {
    File file = SD.open(fileName);
    size_t size = file.size();

    file.close();
    return size;
}

String JessyIO::getPath(String fileName) {
    File file = SD.open(fileName);
    String path = file.path();

    file.close();
    return path;
}

String JessyIO::getAbsoluteName(String fileName) {
    File file = SD.open(fileName);
    String name = file.name();

    file.close();
    return name;
}

bool JessyIO::exists(String path) {
    return SD.exists(path);
}

bool JessyIO::isFile(String path) {
    File file = SD.open(path);
    bool isfile = !file.isDirectory();

    file.close();
    return isfile;
}

bool JessyIO::isDirectory(String path) {
    File file = SD.open(path);
    bool isdir = file.isDirectory();

    file.close();
    return isdir;
}

JessyRTC JessyIO::getLastWrite(String path) {
    File file = SD.open(path);
    JessyRTC rtc = JessyUtility::fromTime(file.getLastWrite());

    file.close();
    return rtc;
}

uint16_t JessyIO::listFiles(String path, String files[]) {
    File directory = SD.open(path);
    uint16_t count = 0;

    if(directory) {
        while(true) {
            File entry = directory.openNextFile();
            if(!entry)
                break;

            files[count] = entry.path();
            entry.close();
            count++;
        }
    }

    directory.close();
    return count;
}

uint16_t JessyIO::listFilesRecursive(String path, String files[]) {
    File directory = SD.open(path);
    uint16_t count = 0;

    if(directory) {
        while(true) {
            File entry = directory.openNextFile();
            if(!entry)
                break;

            files[count] = entry.path();
            count ++;

            if(entry.isDirectory())
                count += JessyIO::listFilesRecursive(
                    entry.path(),
                    files + count
                );
            entry.close();
        }

        directory.close();
    }

    return count;
}