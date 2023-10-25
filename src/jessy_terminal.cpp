#include <jessy_agent.h>
#include <jessy_io.h>
#include <jessy_terminal.h>
#include <jessy_util.h>

static void printCommandError(String cmd, String message) {
    JessyUtility::log(JSY_LOG_ERROR, cmd + F(": ") + message);
}

static void printIncorrectArity(String cmd) {
    printCommandError(cmd, F("Incorrect parameter arity."));
}

void JessyTerminal::cd(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    if(!agent.setWorkingDirectory(arguments[1]))
        printCommandError("cd", F("Cannot change working directory."));
}

void JessyTerminal::pwd(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 1) {
        printIncorrectArity(arguments[0]);
        return;
    }

    JessyIO::println(agent.getWorkingDirectory());
}

void JessyTerminal::ls(JessyAgent &agent, String arguments[], uint8_t argc) {
    bool isRecursive = false, includeHiddenFiles = false;
    for(uint8_t i = 1; i < argc; i++) {
        String arg = arguments[i];

        if(arg == F("-r"))
            isRecursive = true;
        else if(arg == F("-a"))
            includeHiddenFiles = true;
    }

    String files[300];
    uint16_t count;

    if(isRecursive)
        count = JessyIO::listFilesRecursive(agent.getWorkingDirectory(), files);
    else count = JessyIO::listFiles(agent.getWorkingDirectory(), files);

    for(uint16_t i = 0; i < count; i++) {
        String fileName = files[i];
        if(!includeHiddenFiles && fileName.startsWith(F("."))) 
            continue;

        String actualFilePath = agent.getWorkingDirectory() + F("/") + fileName;
        bool isFile = JessyIO::isFile(actualFilePath);
        JessyRTC lastWrite = JessyIO::getLastWrite(actualFilePath);

        JessyIO::print(isFile ? "F " : "D ");
        JessyIO::print("[" + JessyUtility::rtcStructString(lastWrite) + "]\t");
        JessyIO::println(fileName);
    }
}

void JessyTerminal::mkdir(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc < 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    for(uint8_t i = 1; i < argc; i++) {
        String dir = arguments[i];
        if(!JessyIO::mkdir(agent.getWorkingDirectory() + F("/") + dir))
            printCommandError(arguments[0], "Cannot create directory: " + dir);
    }
}

void JessyTerminal::touch(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc < 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    for(uint8_t i = 1; i < argc; i++) {
        String file = arguments[i];
        if(!JessyIO::writeFile(agent.getWorkingDirectory() + F("/") + file, "\r"))
            printCommandError(arguments[0], "Cannot create file: " + file);
    }
}

void JessyTerminal::rm(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::cp(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::mv(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::cat(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::echo(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc < 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    for(uint16_t i = 1; i < argc; i++)
        JessyIO::println(arguments[i]);
}

void JessyTerminal::useradd(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::userdel(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::passwd(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::sys(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::su(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::sd(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::esp32cpu(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::date(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::time(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::cal(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::js(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyExecCommand(JessyAgent &agent, String arguments[], uint8_t argc) {
    String cmd = arguments[0];

    #define JSY_EXEC(cmd) JessyTerminal:: cmd (agent, arguments, argc);

    if(argc == 0)
        return;
    else if(cmd == F("cd"))     JSY_EXEC(cd)
    else if(cmd == F("pwd"))    JSY_EXEC(pwd)
    else if(cmd == F("ls"))     JSY_EXEC(ls)
    else if(cmd == F("mkdir"))  JSY_EXEC(mkdir)
    else if(cmd == F("touch"))  JSY_EXEC(touch)
    else if(cmd == F("echo"))   JSY_EXEC(echo)
    else JessyUtility::log(
        JSY_LOG_ERROR,
        "Command not found: " + arguments[0]
    );

    #undef JSY_EXEC
}