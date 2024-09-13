#include "exe_functions.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

void execute_who_function() {
    int error = 0;
    error = execlp("who", "who", (char *)NULL);
    perror("Execute who fails");
    if(error==-1){
        exit(EXIT_FAILURE);
    }
}

void execute_ls_function() {
    execlp("ls", "ls", "-l", (char *)NULL);
}

void execute_date_function() {
    int error = 0;
    error = execlp("date", "date", (char *)NULL);
    perror("Execute Date command fails");
    if(error==-1){
        exit(EXIT_FAILURE);
    }
}
void grep(const char *filename, const char *pattern) {
  int fd = open(filename, O_RDONLY);  // Open the file for reading
  if (fd == -1) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  char buffer[BUFFER_SIZE];
  ssize_t bytesRead;
  char *line;
  size_t len;

  // Reading file line by line
  while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) {
    line = strtok(buffer, "\n");
    while (line != NULL) {
      if (strstr(line, pattern) != NULL) {
        printf("%s\n", line);  // Print matching line
      }
      line = strtok(NULL, "\n");
    }
  }

  if (bytesRead == -1) {
    perror("Error reading file");
  }
  close(fd);  // Close the file
}


void execute_cd_function(const char *path){
  if (chdir(path) == 0) {
    printf("Changed directory to: %s\n", path);
  } else {
    perror("chdir failed");
  }
}
