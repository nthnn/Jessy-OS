#include <jessy_agent.h>
#include <jessy_bios.h>
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

        if(arg == F("-r") || arg == F("--recursive"))
            isRecursive = true;
        else if(arg == F("-a") || arg == F("--all"))
            includeHiddenFiles = true;
    }

    String files[300];
    uint16_t count;

    if(isRecursive)
        count = JessyIO::listFilesRecursive(agent.getWorkingDirectory(), files);
    else count = JessyIO::listFiles(agent.getWorkingDirectory(), files);

    for(uint16_t i = 0; i < count; i++) {
        String fileName = files[i];
        bool isFile = JessyIO::isFile(fileName);

        JessyRTC lastWrite = JessyIO::getLastWrite(fileName);
        String actualFilename = fileName.substring(
            agent.getWorkingDirectory().length() + 1
        );

        if(!includeHiddenFiles && actualFilename.startsWith(F("."))) 
            continue;

        JessyIO::print(isFile ? "F " : "D ");
        JessyIO::print("[" + JessyUtility::rtcStructString(lastWrite) + "]\t");
        JessyIO::println(actualFilename);
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
    if(argc < 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    bool verbose = false;
    for(uint8_t i = 1; i < argc; i++)
        if(arguments[i] == F("-v") || arguments[i] == F("--verbose"))
            verbose = true;

    for(uint8_t i = 1; i < argc; i++)
        if(arguments[i] == F("-v") || arguments[i] == F("--verbose"))
            continue;
        else {
            String file = arguments[i],
                actualFile = agent.getWorkingDirectory() + "/" + file;

            if(verbose)
                JessyUtility::log(JSY_LOG_PLAIN, "Deleting " + file);

            if(!JessyIO::deleteFile(actualFile))
                JessyUtility::log(JSY_LOG_ERROR, "Cannot delete: " + file);
        }
}

void JessyTerminal::cp(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::mv(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::cat(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String file = agent.getWorkingDirectory() + "/" + arguments[1];
    if(!JessyIO::exists(file)) {
        printCommandError(arguments[0], F("File doesn't exist."));
        return;
    }
    else if(!JessyIO::isFile(file)) {
        printCommandError(arguments[0], F("Input path is directory."));
        return;
    }

    JessyIO::println(JessyIO::readFile(file));
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

void JessyTerminal::gpio(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::clock(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::cal(JessyAgent &agent, String arguments[], uint8_t argc) {
    JessyRTC rtc;// = JessyUtility::getRTC(JessyDS1307.now());
    rtc.year = 2023;
    rtc.month = 10;

    JessyIO::println(
        "Calendar for " + String(rtc.year) +
        "/" + String(rtc.month) + "\n"
    );

    const char* daysOfWeek[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    uint16_t year = rtc.year, month = rtc.month;

    if(month < 3) {
        month += 12;
        year--;
    }

    uint8_t K = year % 100, J = year / 100;
    uint8_t dayOfWeek = (rtc.day + 13 * (month + 1) / 5 + K + K / 4 + J / 4 - 2 * J) % 7;

    for(uint8_t i = 0; i < 7; i++)
        JessyIO::print(String(daysOfWeek[i]) + F("\t"));
    JessyIO::println();

    for(uint8_t i = 0; i < dayOfWeek; i++)
        JessyIO::print(F("\t"));

    uint8_t daysInMonth;
    if(month == 2)
        daysInMonth = (year % 4 == 0 && year % 100 != 0) ||
            (year % 400 == 0) ? 29 : 28;
    else if(month == 4 || month == 6 || month == 9 || month == 11)
        daysInMonth = 30;
    else daysInMonth = 31;

    for(uint8_t day = 1; day <= daysInMonth; day++) {
        JessyIO::print(String(day) + F("\t"));

        if((day + dayOfWeek) % 7 == 0)
            JessyIO::println();
    }

    JessyIO::println();
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
    else if(cmd == F("rm"))     JSY_EXEC(rm)
    else if(cmd == F("cat"))    JSY_EXEC(cat)
    else if(cmd == F("echo"))   JSY_EXEC(echo)
    else if(cmd == F("cal"))    JSY_EXEC(cal)
    else JessyUtility::log(
        JSY_LOG_ERROR,
        "Command not found: " + arguments[0]
    );

    #undef JSY_EXEC
}