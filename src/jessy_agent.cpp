#include <jessy_agent.h>

void JessyAgent::setName(String name) {
    this->name = name;
}

String JessyAgent::getName() {
    return this->name;
}

void JessyAgent::setWorkingDirectory(String wd) {
    this->wd = "/root/" + wd;
}

String JessyAgent::getWorkingDirectory() {
    return this->wd;
}

String JessyAgent::shellString() {
    String path = "";

    if(this->getWorkingDirectory() == ("/root/" + this->name) ||
        (this->getName() == "anonymous" &&
            this->getWorkingDirectory() == ("/root/shared")))
        path = F("root");
    else {
        int idx = this->wd.lastIndexOf(F('/'));
        if(idx == -1)
            idx = 0;

        path = this->wd.substring(idx);
    }

    return this->name + "@" + path + " #~ ";
}

void JessyAgent::anonymous() {
    this->setName("anonymous");
    this->setWorkingDirectory("shared");
}