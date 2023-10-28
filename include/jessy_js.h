#ifndef JESSY_JS_H
#define JESSY_JS_H

#include <jessy_agent.h>

class JessyDuktape {
public:
    static void init();
    static void free();

    static void eval(String fileName);

private:
    static void initIOObject();
};

#endif