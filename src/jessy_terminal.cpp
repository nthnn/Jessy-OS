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

void JessyTerminal::cd(JessyAgent agent, String arguments[], uint8_t argc) {
    if(argc != 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    if(!agent.setWorkingDirectory(arguments[1]))
        printCommandError("cd", "Cannot change working directory.");
}

void JessyTerminal::pwd(JessyAgent agent, String arguments[], uint8_t argc) {
    if(argc != 1) {
        printIncorrectArity(arguments[0]);
        return;
    }

    JessyIO::println(agent.getWorkingDirectory());
}

void JessyTerminal::ls(JessyAgent agent, String arguments[], uint8_t argc) {
    bool isRecursive = false, includeHiddenFiles = false;
    for(uint8_t i = 1; i < argc; i++) {
        String arg = arguments[i];

        if(arg == "-r")
            isRecursive = true;
        else if(arg == "-a")
            includeHiddenFiles = true;
    }
}

void JessyTerminal::mkdir(JessyAgent agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::touch(JessyAgent agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::rm(JessyAgent agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::cp(JessyAgent agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::mv(JessyAgent agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::cat(JessyAgent agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::echo(JessyAgent agent, String arguments[], uint8_t argc) {
    if(argc < 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    for(uint16_t i = 1; i < argc; i++)
        JessyIO::println(arguments[i]);
}

void JessyTerminal::useradd(JessyAgent agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::userdel(JessyAgent agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::passwd(JessyAgent agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::sys(JessyAgent agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::su(JessyAgent agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::sd(JessyAgent agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::esp32cpu(JessyAgent agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::date(JessyAgent agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::time(JessyAgent agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::cal(JessyAgent agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::js(JessyAgent agent, String arguments[], uint8_t argc) {
}

void JessyExecCommand(JessyAgent agent, String arguments[], uint8_t argc) {
    String cmd = arguments[0];

    #define JSY_EXEC(cmd) JessyTerminal:: cmd (agent, arguments, argc);

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