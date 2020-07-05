#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include "alias.h"
#include <fcntl.h>

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
  printf(*color_ind ? ANSI_COLOR_RED : ANSI_COLOR_GREEN);
  printf("$ " ANSI_COLOR_RESET);
}

void handle_file_write(char **args, int index)
{
  int fd = open(args[index + 1], O_WRONLY | O_CREAT, 0644);
  dup2(fd, 1);
  args[index] = NULL;
}

void handle_redirection(char_ptr *command)
{
  int index;
  index = includes_array(command, '>');
  if (index > 0)
  {
    handle_file_write(command, index);
  }
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
      handle_redirection(command);
      execvp(actual, command);
      handle_cmd_not_found(actual);
    }
    else
    {
      waitpid(pid, color_ind, 0);
    }
  }

  return 0;
}