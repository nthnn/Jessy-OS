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

#ifndef JESSY_IO_H
#define JESSY_IO_H

#include <Arduino.h>
#include <jessy_defs.h>

class JessyIO {
public:
    static void print(String text);
    static void println(String text = "");
    static void clearScreen();

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