#include <assert.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exe_functions.h"
#include <unistd.h>
#include <pthread.h>

#include "parse.h"

static void print_cmd(Command *cmd);
static void print_pgm(Pgm *p);
static void run_command(Command *cmd_list);
void stripwhite(char *);

static void run_pgm(Pgm *pC);

void* monitor_ctrl_d(void* arg) {
  pthread_t* execution_thread = (pthread_t*)arg;
  printf("Monitor thread started. Press Ctrl+D (EOF) to terminate the sleep thread.\n");

  while (1) {
    int c = getchar(); // Read from stdin
    if (c == EOF) {
      printf("\nCtrl+D (EOF) detected. Terminating the sleep thread...\n");
      // Cancel the sleep thread
      pthread_cancel(*execution_thread);
      pthread_exit(NULL);
    }
  }
  return NULL;
}

void* sleep_thread(void* arg) {
  int sleep_time = *(int*)arg;
  printf("Sleeping for %d seconds...\n", sleep_time);
  sleep(sleep_time);
  printf("Finished sleeping.\n");
  return NULL;
}


int main(void)
{
  for (;;)
  {
    char *line;
    line = readline("> ");

    // Remove leading and trailing whitespace from the line
    stripwhite(line);

    // If stripped line not blank
    if (*line)
    {
      add_history(line);

      Command cmd;
      if (parse(line, &cmd) == 1){
          run_command(&cmd);
      }
      else{
        printf("Parse ERROR\n");
      }
    }
    // Clear memory
    free(line);
  }

  return 0;
}

// Execute the command in program list
static void run_command(Command *cmd_list){
  run_pgm(cmd_list->pgm);
  print_cmd(cmd_list);
}

static void run_pgm(Pgm *p) {
  if (p == NULL){
    return;
  }
  else{
    char **pl = p->pgmlist;
    int sleep_time;
    run_pgm(p->next);
    pid_t pid = fork();
      if (pid < 0) {
        perror("fork");
        exit(0);
      } else if (pid == 0) {
        switch (*pl[0]) {
          case 'l':
            if (strcmp(*pl, "ls") == 0) {
              execute_ls_function(*(pl+1));
              exit(0);
            }
            break;
          case 'w':
            if (strcmp(*pl, "who") == 0) {
              execute_who_function();
              exit(0);
            }
            break;
          case 'c':
            if (strcmp(*pl, "cd") == 0) {
              execute_cd_function(*pl + 3);
              exit(0);
            }
            break;
          case 'd':
            if (strcmp(*pl, "date") == 0) {
              execute_date_function();
              exit(0);
            }
            break;
          case 'e':
            if (strcmp(*pl, "exit") == 0) {
              exit(1);
            }
            break;
          case 's':
            // Get the sleep number after 6 position of 's'
            sleep_time = atoi(*pl + 6);
            pthread_t monitor_thread, execution_thread;
            pthread_create(&monitor_thread, NULL, monitor_ctrl_d, (void*)&execution_thread);
            pthread_create(&execution_thread, NULL, sleep_thread, &sleep_time);
            // Wait for the sleep thread to finish
            pthread_join(execution_thread, NULL);
          default:printf("unknown command");

        }
      } else {
        // Parent process: wait for the child to complete
        int status;
        waitpid(pid, &status, 0);
      }
    }
  }

static void print_cmd(Command *cmd_list){
  printf("------------------------------\n");
  printf("Parse OK\n");
  printf("stdin:      %s\n", cmd_list->rstdin ? cmd_list->rstdin : "<none>");
  printf("stdout:     %s\n", cmd_list->rstdout ? cmd_list->rstdout : "<none>");
  printf("background: %s\n", cmd_list->background ? "true" : "false");
  printf("Pgms:\n");
  print_pgm(cmd_list->pgm);
  printf("------------------------------\n");
}

static void print_pgm(Pgm *p){
  if (p == NULL){
    return;
  }else{
    char **pl = p->pgmlist;
    /* The list is in reversed order so print
     * it reversed to get right
     */
    print_pgm(p->next);
    printf("            * [ ");
    while (*pl){
      printf("%s ", *pl++);
    }
    printf("]\n");
  }
}

void stripwhite(char *string){
  size_t i = 0;
  while (isspace(string[i])){
    i++;
  }
  if (i){
    memmove(string, string + i, strlen(string + i) + 1);
  }
  i = strlen(string) - 1;
  while (i > 0 && isspace(string[i])){
    i--;
  }
  string[++i] = '\0';
}
