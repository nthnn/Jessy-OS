#ifndef JESSY_IO_H
#define JESSY_IO_H

#include <Arduino.h>
#include <jessy_defs.h>

class JessyIO {
public:
    static void print(String text);
    static void println(String text = "");
    static void clearScreen();
    static String scan();

    static bool mkdir(String path);
    static bool rmdir(String path);

    static String readFile(String fileName);
    static bool writeFile(String fileName, String content);
    static bool appendFile(String fileName, String content);
    static bool renameFile(String oldName, String newName);
    static bool deleteFile(String fileName);
    static size_t getFileSize(String fileName);
    static String getPath(String fileName);
    static String getAbsoluteName(String fileName);

    static bool exists(String path);
    static bool isFile(String path);
    static bool isDirectory(String path);

    static JessyRTC getLastWrite(String path);

    static uint16_t listFiles(String path, String files[]);
    static uint16_t listFilesRecursive(String path, String files[]);
};

#endif