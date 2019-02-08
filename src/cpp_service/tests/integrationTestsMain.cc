#include <nlohmann/json.hpp>
#include <fstream> 
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TESTS_DIR "./tests"

using namespace std;
using json = nlohmann::json;

static bool fileExists(const string& fname) {
    if (FILE *f = fopen(fname.c_str(), "r")) {
        fclose(f);
        return true;
    } else {
        return false;
    }
}

void exec(string &result, const string &cmd) {
    array<char, 512> buffer;
    shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        return;
    }
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
        result += buffer.data();
    }
}

static int runTests(char *pInTestFolderName)
{
    pid_t serverPID = fork();
    int returnValue = 0;

    if (serverPID == 0) {
        int fd = open("/dev/null", O_WRONLY | O_CREAT, 0666);
        //dup2(fd, 1); // replace stdout
        execl("./bin/server.out", "./bin/server.out", (char *) 0);
    } else {
        sleep(2); // wait for server to be ready
        printf("Runing tests for %s...\n", pInTestFolderName);
        string fname = TESTS_DIR;
        fname += "/";
        fname += pInTestFolderName;
        fname += "/testCases.txt";

        if (! fileExists(fname)) {
            return -1;
        }

        ifstream i(fname);
        json jHash;
        i >> jHash;

        string cmdLine;
        string result;

        auto testCases = jHash["test-cases"];
        for (auto tcase : testCases) {
            cmdLine = "./bin/client.out ";
            cmdLine += tcase["input"].get<string>();
            printf("\nCMD: %s\n", cmdLine.c_str());
            result = "";
            exec(result, cmdLine);

            string baseline = TESTS_DIR;
            baseline += "/";
            baseline += pInTestFolderName;
            baseline += "/baseline/";
            baseline += tcase["output"].get<string>();
            ifstream t(baseline);
            string baselineText((std::istreambuf_iterator<char>(t)), istreambuf_iterator<char>());

            if (result != baselineText) {
                printf("Test case failed.\n");
                printf("%s\n", cmdLine.c_str());
                printf("Expecting output: %s\n", baselineText.c_str());
                printf("Got: %s\n", result.c_str());
                returnValue = 1;
            }
        }

        kill(serverPID, SIGTERM);
    }

    return returnValue;
}

int main(int argc, char *argv[])
{
    int returnCode = 0;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(TESTS_DIR)) != NULL) {
        // iterates through all directories
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type & DT_DIR) {
                if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..")) {
                    int errorCode = runTests(ent->d_name);
                    if (errorCode == -1) {
                        closedir(dir);
                        printf("Could execute tests for %s\n", ent->d_name);
                        returnCode = 1;
                        break;
                    } else if (errorCode > 0) {
                        returnCode = 1;
                    }
                }
            }
        }
        //closedir(dir);
    } else {
        printf("Could not open %s\n", TESTS_DIR);
        returnCode = 1;
    }

    return returnCode;
}