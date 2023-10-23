#include <jessy_agent.h>

void JessyAgent::setName(String name) {
    this->name = name;
}

String JessyAgent::getName() {
    return this->name;
}

String JessyAgent::shellString() {
    return this->name + "#~ ";
}