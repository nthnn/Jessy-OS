#ifndef JESSY_AGENT_H
#define JESSY_AGENT_H

#include <Arduino.h>

class JessyAgent {
public:
    void setName(String name);
    String getName();

    void setWorkingDirectory(String wd);
    String getWorkingDirectory();

    String shellString();

private:
    String name;
    String wd;
};

static JessyAgent currentAgent;

#endif