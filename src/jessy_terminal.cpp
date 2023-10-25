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
    if(argc != 1) {
        printIncorrectArity(arguments[0]);
        return;
    }

    JessyIO::println(String(F("Chip cores:\t\t")) + ESP.getChipCores());
    JessyIO::println(String(F("Chip model:\t\t")) + ESP.getChipModel());
    JessyIO::println(String(F("Chip revision:\t\t")) + ESP.getChipRevision());
    JessyIO::println(String(F("CPU frequency:\t\t")) + ESP.getCpuFreqMHz());
    JessyIO::println(String(F("Cycle count:\t\t")) + ESP.getCycleCount());
    JessyIO::println(String(F("Chip ID:\t\t")) + ESP.getEfuseMac());
    JessyIO::println(String(F("Flash chip mode:\t")) + ESP.getFlashChipMode());
    JessyIO::println(String(F("Flash chip size:\t")) + ESP.getFlashChipSize());
    JessyIO::println(String(F("Flash chip speed:\t")) + ESP.getFlashChipSpeed());
    JessyIO::println(String(F("Free heap:\t\t")) + ESP.getFreeHeap());
    JessyIO::println(String(F("Free PSRAM:\t\t")) + ESP.getFreePsram());
    JessyIO::println(String(F("Flash sketch space:\t")) + ESP.getFreeSketchSpace());
    JessyIO::println(String(F("Heap size:\t\t")) + ESP.getHeapSize());
    JessyIO::println(String(F("Max alloc heap:\t\t")) + ESP.getMaxAllocHeap());
    JessyIO::println(String(F("Max alloc PSRAM:\t")) + ESP.getMaxAllocPsram());
    JessyIO::println(String(F("Min free heap:\t\t")) + ESP.getMinFreeHeap());
    JessyIO::println(String(F("Min free PSRAM:\t\t")) + ESP.getMinFreePsram());
    JessyIO::println(String(F("PSRAM Size:\t\t")) + ESP.getPsramSize());
    JessyIO::println(String(F("SDK version:\t\t")) + ESP.getSdkVersion());
    JessyIO::println(String(F("Sketch MD5:\t\t")) + ESP.getSketchMD5());
    JessyIO::println(String(F("Sketch size:\t\t")) + ESP.getSketchSize());
}

void JessyTerminal::gpio(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::clock(JessyAgent &agent, String arguments[], uint8_t argc) {
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
    else if(cmd == F("cd"))         JSY_EXEC(cd)
    else if(cmd == F("pwd"))        JSY_EXEC(pwd)
    else if(cmd == F("ls"))         JSY_EXEC(ls)
    else if(cmd == F("mkdir"))      JSY_EXEC(mkdir)
    else if(cmd == F("touch"))      JSY_EXEC(touch)
    else if(cmd == F("rm"))         JSY_EXEC(rm)
    else if(cmd == F("cat"))        JSY_EXEC(cat)
    else if(cmd == F("echo"))       JSY_EXEC(echo)
    else if(cmd == F("esp32cpu"))   JSY_EXEC(esp32cpu)
    else JessyUtility::log(
        JSY_LOG_ERROR,
        "Command not found: " + arguments[0]
    );

    #undef JSY_EXEC
}