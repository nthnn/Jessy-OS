#ifndef JESSY_TERMINAL_H
#define JESSY_TERMINAL_H

class JessyTerminal {
public:
    static void cd(JessyAgent &agent, String arguments[], uint8_t argc);
    static void pwd(JessyAgent &agent, String arguments[], uint8_t argc);
    static void ls(JessyAgent &agent, String arguments[], uint8_t argc);
    static void mkdir(JessyAgent &agent, String arguments[], uint8_t argc);
    static void touch(JessyAgent &agent, String arguments[], uint8_t argc);
    static void rm(JessyAgent &agent, String arguments[], uint8_t argc);
    static void cp(JessyAgent &agent, String arguments[], uint8_t argc);
    static void mv(JessyAgent &agent, String arguments[], uint8_t argc);

    static void cat(JessyAgent &agent, String arguments[], uint8_t argc);
    static void echo(JessyAgent &agent, String arguments[], uint8_t argc);

    static void useradd(JessyAgent &agent, String arguments[], uint8_t argc);
    static void userdel(JessyAgent &agent, String arguments[], uint8_t argc);
    static void passwd(JessyAgent &agent, String arguments[], uint8_t argc);
    static void sys(JessyAgent &agent, String arguments[], uint8_t argc);
    static void su(JessyAgent &agent, String arguments[], uint8_t argc);

    static void sd(JessyAgent &agent, String arguments[], uint8_t argc);
    static void esp32cpu(JessyAgent &agent, String arguments[], uint8_t argc);
    static void gpio(JessyAgent &agent, String arguments[], uint8_t argc);

    static void date(JessyAgent &agent, String arguments[], uint8_t argc);
    static void time(JessyAgent &agent, String arguments[], uint8_t argc);

    static void js(JessyAgent &agent, String arguments[], uint8_t argc);
};

void JessyExecCommand(JessyAgent &agent, String arguments[], uint8_t argc);

#endif