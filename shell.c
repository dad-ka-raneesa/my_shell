#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include "alias.h"
#include "utils.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"

Alias *aliases = NULL;

char_ptr *parse_command(char_ptr instruction)
{
  char_ptr *command = malloc(sizeof(char_ptr) * 10);
  int c_count = 0;

  int ins_len = strlen(instruction);

  int start = 0;
  for (int i = 0; i <= ins_len; i++)
  {
    if (instruction[i] == 32 || instruction[i] == 0)
    {
      command[c_count++] = copy_string(instruction, start, i);
      start = i + 1;
    }
  }
  command[c_count] = NULL;

  return command;
}

void handle_cmd_not_found(char_ptr command)
{
  printf(ANSI_COLOR_YELLOW "ash: " ANSI_COLOR_RESET);
  printf("command not found: %s\n", command);
  exit(127);
}

int includes(char *text, char delimiter)
{
  int is_found = 0;
  for (int i = 0; i < strlen(text) && !is_found; i++)
  {
    if (text[i] == delimiter)
    {
      is_found = 1;
    }
  }
  return is_found;
}

void handle_directory_not_found(char_ptr *command)
{
  char *message = "no such file or directory";
  if (includes(command[1], '.'))
  {
    message = "not a directory";
  }
  printf("cd: %s: %s\n", message, command[1]);
}

void exit_process(int signal)
{
  exit(130);
}

int handle_built_in(char_ptr *command, int_ptr color_ind)
{
  if (strcmp(command[0], "") == 0)
  {
    *color_ind = 1;
    return 1;
  }

  if (strcmp(command[0], "exit") == 0)
  {
    exit(0);
  }

  if (strcmp(command[0], "alias") == 0)
  {
    if (command[1])
    {
      add_alias(&aliases, command[1]);
    }
    else
    {
      show(aliases);
    }

    return 1;
  }

  if (strcmp(command[0], "cd") == 0)
  {
    if (chdir(command[1]) == -1)
    {
      handle_directory_not_found(command);
    }
    return 1;
  }

  return 0;
}

void prompt(int_ptr color_ind)
{
  char cwd[PATH_MAX];
  getcwd(cwd, sizeof(cwd));

  printf(ANSI_COLOR_CYAN "%s ", cwd);

  if (*color_ind)
  {
    printf(ANSI_COLOR_RED "$ ");
  }
  else
  {
    printf(ANSI_COLOR_GREEN "$ ");
  }

  printf(ANSI_COLOR_RESET);
}

int main(void)
{
  signal(SIGINT, SIG_IGN);

  int_ptr color_ind = malloc(sizeof(int));
  *color_ind = 0;

  while (1)
  {
    char instruction[255];

    prompt(color_ind);
    gets(instruction);

    char_ptr *command = parse_command(instruction);

    char_ptr aka = command[0];
    char_ptr actual = get_actual(aliases, aka);
    while (strcmp(aka, actual) != 0)
    {
      strcpy(aka, actual);
      actual = get_actual(aliases, actual);
    }

    command[0] = actual;
    if (handle_built_in(command, color_ind))
    {
      continue;
    }

    int pid = fork();
    int status;

    if (pid == 0)
    {
      signal(SIGINT, exit_process);
      execvp(actual, command);
      handle_cmd_not_found(actual);
    }
    else
    {
      wait(&status);
      *color_ind = WEXITSTATUS(status);
    }
  }

  return 0;
}