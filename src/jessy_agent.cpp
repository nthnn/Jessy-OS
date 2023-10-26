#include <jessy_agent.h>
#include <jessy_io.h>
#include <jessy_util.h>

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