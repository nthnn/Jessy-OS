#include <jessy_agent.h>
#include <jessy_io.h>
#include <jessy_terminal.h>
#include <jessy_util.h>

static void printCommandError(String cmd, String message) {
    JessyUtility::log(JSY_LOG_ERROR, cmd + ": " + message);
}

static void printIncorrectArity(String cmd) {
    printCommandError(cmd, "Incorrect parameter arity.");
}

void JessyTerminal::cd(String arguments[], uint8_t argc) {
    if(argc != 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    if(!currentAgent.setWorkingDirectory(arguments[1]))
        printCommandError("cd", "Cannot change working directory.");
}

void JessyTerminal::pwd(String arguments[], uint8_t argc) {
    if(argc != 1) {
        printIncorrectArity(arguments[0]);
        return;
    }

    JessyIO::println(currentAgent.getWorkingDirectory());
}

void JessyTerminal::ls(String arguments[], uint8_t argc) {
}

void JessyTerminal::mkdir(String arguments[], uint8_t argc) {
}

void JessyTerminal::touch(String arguments[], uint8_t argc) {
}

void JessyTerminal::rm(String arguments[], uint8_t argc) {
}

void JessyTerminal::cp(String arguments[], uint8_t argc) {
}

void JessyTerminal::mv(String arguments[], uint8_t argc) {
}

void JessyTerminal::cat(String arguments[], uint8_t argc) {
}

void JessyTerminal::echo(String arguments[], uint8_t argc) {
    if(argc < 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    for(uint16_t i = 1; i < argc; i++)
        JessyIO::println(arguments[i]);
}

void JessyTerminal::useradd(String arguments[], uint8_t argc) {
}

void JessyTerminal::userdel(String arguments[], uint8_t argc) {
}

void JessyTerminal::passwd(String arguments[], uint8_t argc) {
}

void JessyTerminal::sys(String arguments[], uint8_t argc) {
}

void JessyTerminal::su(String arguments[], uint8_t argc) {
}

void JessyTerminal::sd(String arguments[], uint8_t argc) {
}

void JessyTerminal::esp32cpu(String arguments[], uint8_t argc) {
}

void JessyTerminal::date(String arguments[], uint8_t argc) {
}

void JessyTerminal::time(String arguments[], uint8_t argc) {
}

void JessyTerminal::cal(String arguments[], uint8_t argc) {
}

void JessyTerminal::js(String arguments[], uint8_t argc) {
}

void JessyExecCommand(String arguments[], uint8_t argc) {
    String cmd = arguments[0];

    #define JSY_EXEC(cmd) JessyTerminal:: cmd (arguments, argc);

    if(cmd == "cd")
        JSY_EXEC(cd)
    else if(cmd == "pwd")
        JSY_EXEC(pwd)
    else if(cmd == "echo")
        JSY_EXEC(echo)
    else JessyUtility::log(
        JSY_LOG_ERROR,
        "Command not found: " + arguments[0]
    );

    #undef JSY_EXEC
}