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

#include "jessy_agent.h"
#include "jessy_io.h"
#include "jessy_util.h"

void JessyAgent::setName(String name) {
    this->name = name;
}

String JessyAgent::getName() {
    return this->name;
}

bool JessyAgent::setWorkingDirectory(String wd) {
    if(!JessyIO::exists(wd) || !JessyIO::isDirectory(wd))
        return false;

    this->wd = wd;
    return true;
}

String JessyAgent::getWorkingDirectory() {
    return this->wd;
}

String JessyAgent::shellString() {
    String path = "";

    if(this->getWorkingDirectory() == ("/root/" + this->name) ||
        (this->getName() == F("anonymous") &&
            this->getWorkingDirectory() == F("/root/shared")))
        path = F("root");
    else {
        int idx = this->getWorkingDirectory().lastIndexOf('/');
        if(idx == -1)
            idx = 0;

        path = this->getWorkingDirectory().substring(idx + 1);
    }

    return this->name + F("@") + path + F(" #~ ");
}

void JessyAgent::anonymous() {
    this->setName("anonymous");
    this->setWorkingDirectory("/root/shared");
}