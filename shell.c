#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <fcntl.h>
#include "alias.h"
#include "file_utils.h"

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

void executeCommand(char_ptr instruction, int_ptr color_ind, int *pipes, int *fd_set)
{
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
    return;
  }

  int pid = fork();
  int status;

  if (pid == 0)
  {
    signal(SIGINT, NULL);
    fd_set[0] && close(pipes[1]);
    fd_set[1] && dup2(pipes[0], 0);
    fd_set[2] && close(pipes[0]);
    fd_set[3] && dup2(pipes[1], 1);
    int new_fd = handle_redirection(command);
    if (new_fd == -1)
      exit(1);
    execvp(command[0], command);
    handle_cmd_not_found(command[0]);
  }
  else
  {
    waitpid(pid, color_ind, 0);
  }
}

void execute(char_ptr instruction, int_ptr color_ind)
{
  int pipes[2];
  pipe(pipes);
  int fd_set[] = {0, 0, 0, 0};
  if (!includes(instruction, '|'))
  {
    executeCommand(instruction, color_ind, pipes, fd_set);
    return;
  }
  char_ptr *piped_args = parse_command(instruction, '|');
  fd_set[2] = 1;
  fd_set[3] = 1;
  executeCommand(piped_args[0], color_ind, pipes, fd_set);
  close(pipes[1]);

  fd_set[0] = 1;
  fd_set[1] = 1;
  fd_set[2] = 0;
  fd_set[3] = 0;
  executeCommand(piped_args[1], color_ind, pipes, fd_set);
  close(pipes[0]);
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
    execute(instruction, color_ind);
  }

  return 0;
}