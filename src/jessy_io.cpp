#include <Arduino.h>
#include <FS.h>
#include <jessy_io.h>
#include <jessy_util.h>
#include <SD.h>
#include <SPI.h>
#include <vfs_api.h>

void JessyIO::print(String text) {
    Serial.print(text);
}

void JessyIO::println(String text) {
    Serial.println(text);
}

void JessyIO::clearScreen() {
    JessyIO::print(F("<clear>"));
    delay(100);
}

String JessyIO::scan() {
    Serial.print(F("<~>"));
    return Serial.readStringUntil('\n');
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