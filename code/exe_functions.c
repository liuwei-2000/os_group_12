#include "exe_functions.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void execute_pwd_function() {
    int error = 0;
    error = execlp("pwd", "pwd", (char *)NULL);
    perror("Execute pwd fails");
    if(error==-1){
        exit(EXIT_FAILURE);
    }
}

void execute_who_function() {
    int error = 0;
    error = execlp("who", "who", (char *)NULL);
    perror("Execute who fails");
    if(error==-1){
        exit(EXIT_FAILURE);
    }
}

void execute_ls_function() {
    int error = 0;
    error = execlp("ls", "ls", "-l", (char *)NULL);
    perror("Execute lp fails");
    if(error==-1){
        exit(EXIT_FAILURE);
    }
}

void execute_date_function() {
    int error = 0;
    error = execlp("date", "date", (char *)NULL);
    perror("Execute Date command fails");
    if(error==-1){
        exit(EXIT_FAILURE);
    }
}
/*
    The split Command function took in string, and a command number counter
    Further use in calling the function
    char input[] = "ls | pwd | date";
    int command_count = 0;

    char** commands = splitString(input, &command_count);

    for (int i = 0; i < command_count; i++) {
        -- execute commands [i];
        free(commands[i]);
    }
 */
char** splitCommand(char* str, int* count) {
    int commands_count = 0;
    char* tempStr = strdup(str); // Make a copy of the string for manipulation
    char* token = strtok(tempStr, "|");
    
    // Count the number of commands
    while (token != NULL) {
        commands_count++;
        token = strtok(NULL, "|");
    }

    // Allocate memory for the commands array
    char** commands = malloc(commands_count * sizeof(char*));
    strcpy(tempStr, str); // Reset the string copy
    token = strtok(tempStr, "|");

    // Extract commands
    for (int i = 0; i < commands_count; i++) {
        commands[i] = strdup(token); // Duplicate the token to the array
        token = strtok(NULL, "|");
    }

    // Clean up
    free(tempStr);

    *count = commands_count;
    return commands;

}