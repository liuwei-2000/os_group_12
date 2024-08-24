#include "exe_functions.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h> d
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