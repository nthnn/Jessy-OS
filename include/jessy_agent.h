#ifndef JESSY_AGENT_H
#define JESSY_AGENT_H

#include <Arduino.h>

class JessyAgent {
public:
    void setName(String name);
    String getName();

    String shellString();

private:
    String name;
};

#endif