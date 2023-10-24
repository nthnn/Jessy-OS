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
    String target = "";
    
    if(wd.startsWith("/"))
        target = "/root/" + (this->name == "anonymous" ?
            "shared" : this->name) + wd;
    else if(wd.startsWith("./"))
        target = this->wd + wd.substring(1);
    else if(wd == "~")
        target = "/root/" + (this->name == "anonymous" ?
            "shared" : this->name);
    else return false;

    this->wd = target;
    return !JessyIO::exists(target) ||
        (JessyIO::exists(target) && JessyIO::isFile(target));
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
    this->setWorkingDirectory("~");
}