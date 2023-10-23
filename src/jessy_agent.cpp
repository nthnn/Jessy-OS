#include <jessy_agent.h>

void JessyAgent::setName(String name) {
    this->name = name;
}

String JessyAgent::getName() {
    return this->name;
}

void JessyAgent::setWorkingDirectory(String wd){
    this->wd = wd;
}

String JessyAgent::getWorkingDirectory() {
    return this->wd;
}

String JessyAgent::shellString() {
    int idx = this->wd.lastIndexOf('/');
    if(idx == -1)
        idx = 0;

    String path = this->wd.substring(idx);
    if(path == "/")
        path = "root";

    return this->name + "@" + path + " #~ ";
}