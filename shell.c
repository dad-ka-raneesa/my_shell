#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include "alias.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"

Alias *aliases = NULL;

void handle_cmd_not_found(char_ptr command)
{
  printf(ANSI_COLOR_YELLOW "zsh: " ANSI_COLOR_RESET);
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

int handle_built_in(char_ptr *command, int_ptr color_ind)
{
  if (strcmp(command[0], "") == 0)
  {
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
    *color_ind = chdir(command[1]);
    if (*color_ind)
      perror("cd");
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

    char_ptr *command = parse_command(instruction, ' ');

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
      signal(SIGINT, NULL);
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