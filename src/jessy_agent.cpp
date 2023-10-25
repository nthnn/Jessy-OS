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
    String target = "", shared("shared");
    
    if(wd.startsWith("/") && wd.length() != 1)
        target = "/root/" + (this->getName().equals("anonymous") ?
            shared : this->name) + wd;
    else if(wd.startsWith("./") && !wd.endsWith("./"))
        target = this->getWorkingDirectory() + wd.substring(1);
    else if(wd.equals("~"))
        target = "/root/" + (this->getName().equals("anonymous") ?
            shared : this->name);
    else return false;

    if(!JessyIO::exists(target) || !JessyIO::isDirectory(target))
        return false;

    this->wd = target;
    return true;
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
        int idx = this->getWorkingDirectory().lastIndexOf('/');
        if(idx == -1)
            idx = 0;

        path = this->getWorkingDirectory().substring(idx + 1);
    }

    return this->name + "@" + path + " #~ ";
}

void JessyAgent::anonymous() {
    this->setName("anonymous");
    this->setWorkingDirectory("~");
}