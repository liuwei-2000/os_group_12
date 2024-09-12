/*
 * Main source code file for lsh shell program
 *
 * You are free to add functions to this file.
 * If you want to add functions in a separate file(s)
 * you will need to modify the CMakeLists.txt to compile
 * your additional file(s).
 *
 * Add appropriate comments in your code to make it
 * easier for us while grading your assignment.
 *
 * Using assert statements in your code is a great way to catch errors early and make debugging easier.
 * Think of them as mini self-checks that ensure your program behaves as expected.
 * By setting up these guardrails, you're creating a more robust and maintainable solution.
 * So go ahead, sprinkle some asserts in your code; they're your friends in disguise!
 *
 * All the best!
 */
#include <assert.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exe_functions.h"
// The <unistd.h> header is your gateway to the OS's process management facilities.
#include <unistd.h>
#include <pthread.h>

#include "parse.h"

static void print_cmd(Command *cmd);
static void print_pgm(Pgm *p);
static void run_command(Command *cmd_list);
void stripwhite(char *);

static void run_pgm(Pgm *pC);

void* monitor_ctrl_d(pthread_t execution_thread) {
  printf("Monitor thread started. Press Ctrl+D (EOF) to terminate the sleep thread.\n");

  // Monitor stdin for Ctrl+D (EOF)
  while (1) {
    int c = getchar(); // Read from stdin
    if (c == EOF) {
      printf("\nCtrl+D (EOF) detected. Terminating the execution thread...\n");

      // Cancel the sleep thread
      pthread_cancel(execution_thread);
      break;
    }
  }

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
  if (p == NULL)
  {
    return;
  }
  else
  {
    char **pl = p->pgmlist;
    int sleep_time;
    run_pgm(p->next);
    while (*pl)
    {
      pid_t pid = fork();
      if (pid < 0) {
        perror("fork");
        exit(0);
      } else if (pid == 0) {
        switch (*pl[0]) {
          case 'l':
            if (strcmp(*pl, "ls") == 0) {
              execute_ls_function();
            }
            break;
          case 'w':
            if (strcmp(*pl, "who") == 0) {
              execute_who_function();
            }
            break;
          case 'c':
            if (strcmp(*pl, "cd") == 0) {
              execute_ls_function();
            }
            break;
          case 'd':
            if (strcmp(*pl, "date") == 0) {
              execute_date_function();
            }
            break;
          case 's':
            // Get the sleep number after 6 position of 's'
            sleep_time = atoi(*pl + 6);
            sleep(sleep_time);
          default:printf("unknown command");
        }
      } else {
        // Parent process: wait for the child to complete
        int status;
        waitpid(pid, &status, 0);
      }
      pl++;
    }
  }
}

static void print_cmd(Command *cmd_list)
{
  printf("------------------------------\n");
  printf("Parse OK\n");
  printf("stdin:      %s\n", cmd_list->rstdin ? cmd_list->rstdin : "<none>");
  printf("stdout:     %s\n", cmd_list->rstdout ? cmd_list->rstdout : "<none>");
  printf("background: %s\n", cmd_list->background ? "true" : "false");
  printf("Pgms:\n");
  print_pgm(cmd_list->pgm);
  printf("------------------------------\n");
}

/* Print a (linked) list of Pgm:s.
 *
 * Helper function, no need to change. Might be useful to study as inpsiration.
 */
static void print_pgm(Pgm *p)
{
  if (p == NULL)
  {
    return;
  }
  else
  {
    char **pl = p->pgmlist;

    /* The list is in reversed order so print
     * it reversed to get right
     */
    print_pgm(p->next);
    printf("            * [ ");
    while (*pl)
    {
      printf("%s ", *pl++);
    }
    printf("]\n");
  }
}


/* Strip whitespace from the start and end of a string.
 *
 * Helper function, no need to change.
 */
void stripwhite(char *string)
{
  size_t i = 0;

  while (isspace(string[i]))
  {
    i++;
  }

  if (i)
  {
    memmove(string, string + i, strlen(string + i) + 1);
  }

  i = strlen(string) - 1;
  while (i > 0 && isspace(string[i]))
  {
    i--;
  }

  string[++i] = '\0';
}
