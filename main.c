#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>

extern int errno;

void handle_cd(char* new_path) {
    int error = chdir(new_path);
    if (error == -1) {
        if (errno == ENOENT) {
            printf("Directory '%s' does not exist\n", new_path);
        } else {
            printf("Error occured opening '%s'\n", new_path);
        }
    }
}

void handle_ls() {
    DIR* current = opendir(".");

    struct dirent* entry;

    // Get rid of the '.' and '..' entries
    readdir(current);
    readdir(current);

    while ((entry = readdir(current)) != NULL) {
        printf("%s\n", entry->d_name);
    }
}

bool handle_other(char* command) {
    char copied[128] = {};
    strcpy(copied, "/bin/");

    char just_command[128] = {};
    int end_of_command = (int) (strchr(command, ' ') - command);
    if (end_of_command < 0) {
        end_of_command = strlen(command);
    }

    strncpy(just_command, command, end_of_command);
    strcat(copied, just_command);
    FILE* file = fopen(copied, "r");
    if (file != NULL) {
        char copied_full[128];
        strcpy(copied_full, "/bin/");
        strcat(copied_full, command);

        system(copied_full);
        return true;
    }
    return false;
}

bool handle_command(char* command) {
    // Have some things be treated as internal?

    //if (strcmp(command, "exit") == 0) {
    //    return true;
    //}

    //if (strncmp(command, "cd", 1) == 0) {
    //    handle_cd(command + 3);
    //    return false;
    //}

    //if (strncmp(command, "ls", 1) == 0) {
    //    handle_ls();
    //    return false;
    //}

    if (!handle_other(command)) {
        printf("Unregonized command '%s'\n", command);
    } 

    return false;
}

int main() {
    char cwd[64];
    while (true) {
        getcwd(cwd, 64);
        printf("%s $ ", cwd);
        char buffer[64];
        char random;
        scanf("%[^\n]%c", buffer, &random);
        if (handle_command(buffer)) {
            break;
        }
    }
}
