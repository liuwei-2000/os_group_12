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

// The <unistd.h> header is your gateway to the OS's process management facilities.
#include <unistd.h>

#include "parse.h"
           
static void run_cmds(Command *);
static void print_cmd(Command *cmd);
static void print_pgm(Pgm *p);
static void execute_ls_function();

void stripwhite(char *);

static void execute_who_function();

static void execute_pwd_function();

int main(void)
{
  for (;;)
  {
    char *line;
    line = readline("> ");

    // If EOF encountered, exit shell
    if (!line)
    {
      break;
    }

    // Remove leading and trailing whitespace from the line
    stripwhite(line);

    // If stripped line not blank
    if (*line)
    {
      add_history(line);

      Command cmd;
      if (parse(line, &cmd) == 1)
      {
        run_cmds(&cmd);
      }
      else
      {
        printf("Parse ERROR\n");
      }
    }

    // Clear memory
    free(line);
  }

  return 0;
}

/* Execute the given command(s).
 *
 * TODO:
 * 1. Execute ls, data, and who command. PATH is also required, for example
 *    ls /certain/path, who /certain_path
 * 2. sleep 30 &
 * 2. Remove the debug printing before the final submission.
 */
static void run_cmds(Command *cmd_list)
{

    Pgm *pgm = cmd_list->pgm;
    char **pl = pgm->pgmlist;
    // Fork a new process
    pid_t pid = fork();
    if (pid < 0) {
        // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        switch (*pl[0]) {
            case 'l':
                if (strcmp(*pl, "ls") == 0) {
                    execute_ls_function();
                } else {
                    printf("Unknown command\n");
                }
                break;
            case 'w':
                if (strcmp(*pl, "who") == 0) {
                    execute_who_function();
                } else {
                    printf("Unknown command\n");
                }
                break;
            case 'p':
                if (strcmp(*pl, "pwd") == 0) {
                    execute_pwd_function();
                } else {
                    printf("Unknown command\n");
                }
                break;
            case 'c':
                if (strcmp(*pl, "cd") == 0) {
                    execute_ls_function();
                } else {
                    printf("Unknown command\n");
                }
                break;
            case 'd':
                if (strcmp(*pl, "date") == 0) {
                    execute_ls_function();
                } else {
                    printf("Unknown command\n");
                }
                break;
            default:printf("unknown command");
        }

    } else {
        // Parent process: wait for the child to complete
        int status;
        if (!cmd_list->background) {
            // Only wait if the command is not set to run in the background
            waitpid(pid, &status, 0);
        }
    }
        print_cmd(cmd_list);
}

static void execute_pwd_function() {
    int error = 0;
    error = execlp("pwd", "pwd", (char *)NULL);
    perror("Execute pwd fails");
    if(error==-1){
        exit(EXIT_FAILURE);
    }
}

static void execute_who_function() {
    int error = 0;
    error = execlp("who", "who", (char *)NULL);
    perror("Execute who fails");
    if(error==-1){
        exit(EXIT_FAILURE);
    }
}

static void execute_ls_function() {
    int error = 0;
    error = execlp("ls", "ls", "-l", (char *)NULL);
    perror("Execute lp fails");
    if(error==-1){
        exit(EXIT_FAILURE);
    }
}

/*
 * Print a Command structure as returned by parse on stdout.
 *
 * Helper function, no need to change. Might be useful to study as inpsiration.
 */
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


