#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <malloc.h>

#define BUF_SZ 2056

void printHelp();

int main(int argc, char** argv)
{
    int curIndex;    // loop counter to traverse arguments
    char *path, *pathToSplit, *pathDelim, *curFolder, curFile[BUF_SZ];
    struct stat curStat;

    // print a help section if the user only enters "which"
    if (argc == 1) {
        printHelp();
        return 0;
    }

    // retreive the user's PATH variable to look in
    path = getenv("PATH");
    pathToSplit = (char*)malloc(sizeof(char) * (strlen(path)+1));
    if (path == NULL) {
        printf("You don't seem to have a PATH variable defined.\n");
        return 0;
    }

    // for each argument provided to which, figure out if it exists
    // and in which folder in the user's PATH it does; if an argument
    // does not exist, don't print it
    for (curIndex = 1; curIndex < argc; curIndex++) {
        // don't worry about clearing the pathToSplit buffer -- it is only
        // truncated after the next line
        strncpy(pathToSplit, path, strlen(path)+1);
        // clear out contents of old file name
        memset(curFile, 0, BUF_SZ);

        // if the user specifies an absolute or relative path,
        // search for the file and print it out if it exists.
        if (argv[curIndex][0] == '/') {
            if (stat(argv[curIndex], &curStat) == 0) {
                if (S_ISREG(curStat.st_mode)) {
                    printf("%s\n", argv[curIndex]);
                }
            }
        }
        else if (argv[curIndex][0] == '.') {
            // prepend the relative path and "/" before checking
            getcwd(curFile, BUF_SZ);
            strncat(curFile, "/", 1);
            strncat(curFile, argv[curIndex], strlen(argv[curIndex]));
            if (stat(curFile, &curStat) == 0) {
                printf("%s\n", curFile);
            }
        }
        // doesn't have a prepended path, so search each folder in the PATH
        // variable, one at a time; stop at the first instance you find
        else {
            curFolder = strtok(pathToSplit, ":");

            while (curFolder != NULL) {
                memset(curFile, 0, BUF_SZ);
                // curFile will become:
                // [folder in PATH] + "/" + [argument passed to which]
                strncpy(curFile, curFolder, strlen(curFolder)+1);
                strncat(curFile, "/", 1);
                strncat(curFile, argv[curIndex], strlen(argv[curIndex]));
                if (stat(curFile, &curStat) == 0) {
                    printf("%s\n", curFile);
                    break;
                }

                curFolder = strtok(NULL, ":");
            }

        }
    }

    return 0;
}


void printHelp() {
    printf("Help section goes here.\n");
}
