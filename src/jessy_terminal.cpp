#include <jessy_agent.h>
#include <jessy_bios.h>
#include <jessy_io.h>
#include <jessy_terminal.h>
#include <jessy_util.h>
#include <SD.h>

static void printCommandError(String cmd, String message) {
    JessyUtility::log(JSY_LOG_ERROR, cmd + F(": ") + message);
}

static void printIncorrectArity(String cmd) {
    printCommandError(cmd,
        "Incorrect parameter arity."
        "\n    Type: help " + cmd
    );
}

void JessyTerminal::cd(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    if(!agent.setWorkingDirectory(JessyUtility::sanitizePath(agent, arguments[1])))
        printCommandError(F("cd"), F("Cannot change working directory."));
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

        JessyIO::print(isFile ? F("F ") : F("D "));
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
        String path = arguments[i],
            actualPath = JessyUtility::sanitizePath(agent, path);
        if(!JessyIO::mkdir(actualPath))
            printCommandError(arguments[0], "Cannot create directory: " + path);
    }
}

void JessyTerminal::touch(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc < 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    for(uint8_t i = 1; i < argc; i++) {
        String path = arguments[i],
            actualPath = JessyUtility::sanitizePath(agent, path);
        if(!JessyIO::writeFile(actualPath, "\r"))
            printCommandError(arguments[0], "Cannot create file: " + path);
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
                actualFile = JessyUtility::sanitizePath(agent, file);

            if(verbose)
                JessyUtility::log(JSY_LOG_PLAIN, "Deleting " + file);

            if(JessyIO::isFile(actualFile) && !JessyIO::deleteFile(actualFile))
                JessyUtility::log(JSY_LOG_ERROR, "Cannot delete file: " + file);
            else if(JessyIO::isDirectory(actualFile)) {
                String files[100];
                uint16_t count = JessyIO::listFilesRecursive(actualFile, files);

                for(uint16_t i = 0; i < count; i++)
                    if(JessyIO::isFile(files[i]) && !JessyIO::deleteFile(files[i])) {
                        JessyUtility::log(JSY_LOG_ERROR, "Cannot delete file: " + files[i]);
                        return;
                    }

                for(uint16_t i = 0; i < count; i++)
                    if(JessyIO::isDirectory(files[i]) && !JessyIO::rmdir(files[i])) {
                        JessyUtility::log(JSY_LOG_ERROR, "Cannot delete file: " + files[i]);
                        return;
                    }

                if(!JessyIO::rmdir(actualFile))
                    JessyUtility::log(JSY_LOG_ERROR, "Cannot delete directory: " + file);
            }
        }
}

void JessyTerminal::cp(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 3) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String source = JessyUtility::sanitizePath(agent, arguments[1]),
        dest = JessyUtility::sanitizePath(agent, arguments[2]);
    if(source == dest)
        return;

    if(!JessyIO::exists(source)) {
        printCommandError(arguments[0], "Source file doesn't exist.");
        return;
    }
    else if(JessyIO::exists(dest) && JessyIO::isFile(dest)) {
        printCommandError(arguments[0], "Destination file already exists.");
        return;
    }

    if(JessyIO::isFile(source) &&
        !JessyIO::writeFile(dest, JessyIO::readFile(source))) {
        printCommandError(arguments[0], F("Something went wrong while copying file."));
        return;
    }
    else {
        String files[100];
        for(uint16_t i = 0; i < JessyIO::listFiles(source, files); i++) {
            String output = files[i],
                original = output;
            output.replace(source, dest);

            if(JessyIO::isFile(original))
                JessyIO::writeFile(
                    output,
                    JessyIO::readFile(files[i])
                );
            else if(JessyIO::isDirectory(original)) {
                JessyIO::mkdir(output);

                String wd = agent.getWorkingDirectory();
                original.replace(wd, F(""));
                output.replace(wd, F(""));

                String args[] = {"cp", original, output};
                JessyTerminal::cp(agent, args, 3);
            }
        }
    }
}

void JessyTerminal::mv(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 3) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String source = arguments[1], dest = arguments[2];
    if(JessyUtility::sanitizePath(agent, source) ==
        JessyUtility::sanitizePath(agent, dest))
        return;

    String cpArgs[] = {"cp", source, dest};
    String rmArgs[] = {"rm", source};

    JessyTerminal::cp(agent, cpArgs, 3);
    JessyTerminal::rm(agent, rmArgs, 2);
}

void JessyTerminal::cat(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 2) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String file = JessyUtility::sanitizePath(agent, arguments[1]);
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

void JessyTerminal::clear(JessyAgent &agent, String arguments[], uint8_t argc) {
    JessyIO::clearScreen();
}

void JessyTerminal::useradd(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 3) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String user = arguments[1],
        user64 = JessyUtility::toBase64(user),
        userFile = "/sys/users/" + user64,
        password = arguments[2];

    if(user.length() < 4 || password.length() < 4) {
        printCommandError(
            arguments[0],
            F("Username and password must be at least 4 characters.")
        );

        return;
    }

    if(user == "anonymous" || user == "shared") {
        printCommandError(arguments[0], F("Username is a special keyword."));
        return;
    }

    if(JessyIO::exists(userFile) && JessyIO::isFile(userFile)) {
        printCommandError(arguments[0], "Username already in use.");
        return;
    }

    JessyIO::writeFile(
        userFile,
        JessyUtility::aesEncrypt(user64, password)
    );

    user.toLowerCase();
    JessyIO::mkdir("/root/" + user);
}

void JessyTerminal::userdel(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 3) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String userdel = arguments[0];
    if(agent.getName() == F("anonymous")) {
        printCommandError(userdel, F("Anonymous user cannot delete another user."));
        return;
    }

    String user = arguments[1],
        user64 = JessyUtility::toBase64(user),
        password = arguments[2];
    String userFile = "/sys/users/" + user64;

    if(user.length() < 4 || password.length() < 4) {
        printCommandError(
            arguments[0],
            F("Username and password must be at least 4 characters.")
        );

        return;
    }

    if(JessyIO::exists(userFile) && 
        JessyIO::isFile(userFile) &&
            JessyIO::readFile(userFile).equals(
                JessyUtility::aesEncrypt(user64, password)
            )) {
        user.toLowerCase();

        String files[256];
        uint16_t count = JessyIO::listFilesRecursive("/root/" + user, files);

        for(uint16_t i = 0; i < count; i++)
            if(JessyIO::isFile(files[i])) {
                if(!JessyIO::deleteFile(files[i]))
                    printCommandError(
                        arguments[0],
                        "Cannot delete file: " + files[i]
                    );
            }

        for(uint16_t i = count; i > 0; i--)
            if(JessyIO::isDirectory(files[i])) {
                if(!JessyIO::rmdir(files[i]))
                    printCommandError(
                        arguments[0],
                        "Cannot delete directory: " + files[i]
                    );
            }

        if(!JessyIO::rmdir("/root/" + user)) {
            printCommandError(arguments[0], F("Cannot remove user root folder."));
            return;
        }

        if(!JessyIO::deleteFile("/sys/users/" + user64)) {
            printCommandError(arguments[0], F("Cannot remove user account."));
            return;
        }

        String args[] = {"su", "-o"};
        JessyTerminal::su(agent, args, 2);
        return;
    }

    printCommandError(userdel, F("Incorrect user credentials."));
}

void JessyTerminal::passwd(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 4) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String passwd = arguments[0];
    if(agent.getName() == F("anonymous")) {
        printCommandError(passwd, F("Anonymous user cannot edit another user."));
        return;
    }

    String user = arguments[1],
        user64 = JessyUtility::toBase64(user),
        password = arguments[2],
        newpass = arguments[3];
    String userFile = "/sys/users/" + user64;

    if(user.length() < 4 ||
        password.length() < 4 ||
        newpass.length() < 4) {
        printCommandError(
            arguments[0],
            F("Username and passwords must be at least 4 characters.")
        );

        return;
    }

    if(JessyIO::exists(userFile) && 
        JessyIO::isFile(userFile) &&
            JessyIO::readFile(userFile).equals(
                JessyUtility::aesEncrypt(user64, password)
            )) {
        JessyIO::writeFile(
            userFile,
            JessyUtility::aesEncrypt(user64, newpass)
        );
        return;
    }

    printCommandError(passwd, F("Incorrect user credentials."));
}

void JessyTerminal::sys(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyTerminal::su(JessyAgent &agent, String arguments[], uint8_t argc) {
    String su = arguments[0];

    if(argc == 2 &&
        (arguments[1] == "-o" ||
            arguments[1] == "--out")) {
        if(agent.getName() == "anonymous")
            printCommandError(su, "Already a guess user.");
        else agent.anonymous();

        return;
    }
    else if(argc == 3) {
        String user = arguments[1],
            user64 = JessyUtility::toBase64(user),
            password = arguments[2];
        String userFile = "/sys/users/" + user64;

        if(JessyIO::exists(userFile) && 
            JessyIO::isFile(userFile) &&
                JessyIO::readFile(userFile).equals(
                    JessyUtility::aesEncrypt(user64, password)
                )) {
            user.toLowerCase();

            agent.setName(user);
            agent.setWorkingDirectory("/root/" + user);

            JessyIO::clearScreen();
            return;
        }

        if(JessyIO::exists(userFile))
            printCommandError(su, F("Incorrect user credentials."));
        else printCommandError(su, F("User not found."));
    }
    else printIncorrectArity(su);
}

void JessyTerminal::sd(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 1) {
        printIncorrectArity(arguments[0]);
        return;
    }

    sdcard_type_t type = SD.cardType();
    JessyIO::print("Card type:\t\t");

    switch(type) {
        case CARD_MMC:
            JessyIO::println("Multimedia Card");
            break;

        case CARD_SD:
            JessyIO::println("Secure Digital");
            break;

        case CARD_SDHC:
            JessyIO::println("SDHC");
            break;

        default:
            JessyIO::println("Unknown");
            break;
    }

    JessyIO::println("Card size:\t\t" + String(SD.cardSize() / 1048576) + "MB");
    JessyIO::println("Total bytes:\t\t" + String(SD.totalBytes() / 1048576) + "MB");
    JessyIO::println("Used bytes:\t\t" + String(SD.usedBytes() / 1048576) + "MB");
    JessyIO::println("Number of sectors:\t" + String(SD.numSectors()));
    JessyIO::println("Sector size:\t\t" + String(SD.sectorSize()));
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
    if(argc < 2 || argc > 4) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String act = arguments[1];
    uint8_t pin = (uint8_t) arguments[2].toInt();

    if(argc == 3) {
        if(act == "on") {
            pinMode(pin, OUTPUT);
            digitalWrite(pin, HIGH);
        }
        else if(act == "off") {
            pinMode(pin, OUTPUT);
            digitalWrite(pin, LOW);
        }
        else if(act == "read")
            JessyIO::println(pin + ": " + String(digitalRead(pin)));
        else printCommandError(arguments[0], F("Invalid action for GPIO."));
    }
    else if(argc == 4 && act == "write")
        digitalWrite(pin, (byte) arguments[2].toInt());
}

void JessyTerminal::date(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 1) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String dateTime = JessyUtility::getRTCString(
        JessyUtility::createClock().now()
    );
    dateTime = dateTime.substring(0, dateTime.indexOf(' '));

    JessyIO::println(dateTime);
}

void JessyTerminal::time(JessyAgent &agent, String arguments[], uint8_t argc) {
    if(argc != 1) {
        printIncorrectArity(arguments[0]);
        return;
    }

    String dateTime = JessyUtility::getRTCString(
        JessyUtility::createClock().now()
    );
    dateTime = dateTime.substring(dateTime.indexOf(' ') + 1);

    JessyIO::println(dateTime);
}

void JessyTerminal::js(JessyAgent &agent, String arguments[], uint8_t argc) {
}

void JessyExecCommand(JessyAgent &agent, String arguments[], uint8_t argc) {
    String cmd = arguments[0];

    #define JSY_EXEC(cmd) JessyTerminal:: cmd (agent, arguments, argc);

    if(argc == 0)                   return;
    else if(cmd == F("cd"))         JSY_EXEC(cd)
    else if(cmd == F("pwd"))        JSY_EXEC(pwd)
    else if(cmd == F("ls"))         JSY_EXEC(ls)
    else if(cmd == F("mkdir"))      JSY_EXEC(mkdir)
    else if(cmd == F("touch"))      JSY_EXEC(touch)
    else if(cmd == F("rm"))         JSY_EXEC(rm)
    else if(cmd == F("cp"))         JSY_EXEC(cp)
    else if(cmd == F("mv"))         JSY_EXEC(mv)
    else if(cmd == F("cat"))        JSY_EXEC(cat)
    else if(cmd == F("echo"))       JSY_EXEC(echo)
    else if(cmd == F("clear"))      JSY_EXEC(clear)
    else if(cmd == F("useradd"))    JSY_EXEC(useradd)
    else if(cmd == F("userdel"))    JSY_EXEC(userdel)
    else if(cmd == F("passwd"))     JSY_EXEC(passwd)
    else if(cmd == F("su"))         JSY_EXEC(su)
    else if(cmd == F("sd"))         JSY_EXEC(sd)
    else if(cmd == F("esp32cpu"))   JSY_EXEC(esp32cpu)
    else if(cmd == F("gpio"))       JSY_EXEC(gpio)
    else if(cmd == F("date"))       JSY_EXEC(date)
    else if(cmd == F("time"))       JSY_EXEC(time)
    else JessyUtility::log(
        JSY_LOG_ERROR,
        "Command not found: " + arguments[0]
    );

    #undef JSY_EXEC
}