#ifndef JESSY_TERMINAL_H
#define JESSY_TERMINAL_H

class JessyTerminal {
public:
    static void cd(String arguments[], uint8_t argc);
    static void pwd(String arguments[], uint8_t argc);
    static void ls(String arguments[], uint8_t argc);
    static void mkdir(String arguments[], uint8_t argc);
    static void touch(String arguments[], uint8_t argc);
    static void rm(String arguments[], uint8_t argc);
    static void cp(String arguments[], uint8_t argc);
    static void mv(String arguments[], uint8_t argc);

    static void cat(String arguments[], uint8_t argc);
    static void echo(String arguments[], uint8_t argc);

    static void useradd(String arguments[], uint8_t argc);
    static void userdel(String arguments[], uint8_t argc);
    static void passwd(String arguments[], uint8_t argc);
    static void sys(String arguments[], uint8_t argc);
    static void su(String arguments[], uint8_t argc);

    static void sd(String arguments[], uint8_t argc);
    static void esp32cpu(String arguments[], uint8_t argc);

    static void date(String arguments[], uint8_t argc);
    static void time(String arguments[], uint8_t argc);
    static void cal(String arguments[], uint8_t argc);

    static void js(String arguments[], uint8_t argc);
};

void JessyExecCommand(String arguments[], uint8_t argc);

#endif