#include <jessy_agent.h>

JessyAgent::JessyAgent(String _name):
    name(_name) { }

String JessyAgent::getName() {
    return this->name;
}

String JessyAgent::shellString() {
    return this->name + "#~ ";
}