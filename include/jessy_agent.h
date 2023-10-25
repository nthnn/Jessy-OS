#ifndef JESSY_AGENT_H
#define JESSY_AGENT_H

#include <Arduino.h>

class JessyAgent {
public:
    void setName(String name);
    String getName();

    bool setWorkingDirectory(String wd);
    String getWorkingDirectory();

    String shellString();
    void anonymous();

private:
    String name, wd;
};

static JessyAgent currentAgent;

#endif