#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <malloc.h>

#define BUF_SZ 2056

void print_help();

int main(int argc, char** argv) {
    int cur_index;
    char *path, *path_to_split, *path_delim, *cur_folder, cur_file[BUF_SZ];
    struct stat cur_stat;

    // print a help section if the user only enters "which"
    if (argc == 1) {
        print_help();
        return 0;
    }

    // retreive the user's PATH variable to look in
    path = getenv("PATH");
    path_to_split = (char*)malloc(sizeof(char) * (strlen(path)+1));
    if (path == NULL) {
        printf("You don't seem to have a PATH variable defined.\n");
        return 0;
    }

    // for each argument provided to which, figure out if it exists
    // and in which folder in the user's PATH it does; if an argument
    // does not exist, don't print it
    for (cur_index = 1; cur_index < argc; cur_index++) {
        // don't worry about clearing the path_to_split buffer -- it is only
        // truncated after the next line
        strncpy(path_to_split, path, strlen(path)+1);
        // clear out contents of old file name
        memset(cur_file, 0, BUF_SZ);

        // if the user specifies an absolute or relative path,
        // search for the file and print it out if it exists.
        if (argv[cur_index][0] == '/') {
            if (stat(argv[cur_index], &cur_stat) == 0) {
                if (S_ISREG(cur_stat.st_mode)) {
                    printf("%s\n", argv[cur_index]);
                }
            }
        } else if (argv[cur_index][0] == '.') {
            // prepend the relative path and "/" before checking
            getcwd(cur_file, BUF_SZ);
            strncat(cur_file, "/", 1);
            strncat(cur_file, argv[cur_index], strlen(argv[cur_index]));
            if (stat(cur_file, &cur_stat) == 0) {
                printf("%s\n", cur_file);
            }
        }  else {
            // doesn't have a prepended path, so search each folder in the PATH
            // variable, one at a time; stop at the first instance you find
            cur_folder = strtok(path_to_split, ":");

            while (cur_folder != NULL) {
                memset(cur_file, 0, BUF_SZ);
                // cur_file will become:
                // [folder in PATH] + "/" + [argument passed to which]
                strncpy(cur_file, cur_folder, strlen(cur_folder)+1);
                strncat(cur_file, "/", 1);
                strncat(cur_file, argv[cur_index], strlen(argv[cur_index]));
                if (stat(cur_file, &cur_stat) == 0) {
                    printf("%s\n", cur_file);
                    break;
                }
                cur_folder = strtok(NULL, ":");
            }
        }
    }
    return 0;
}

void print_help() {
    printf("Help section goes here. No nifty flags can be used at the moment. \n");
}
