#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

char *copy_string(char *str, int start, int end)
{
  char *n_str = malloc(sizeof(char) * (end - start));

  for (int i = start; i < end; i++)
  {
    n_str[i - start] = str[i];
  }

  return n_str;
}

char **parse_command(char *instruction)
{
  char **command = malloc(sizeof(char *) * 10);
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

void exit_process(int signal)
{
  exit(0);
}

int handle_built_in(char **command)
{
  if (strcmp(command[0], "") == 0)
  {
    return 1;
  }

  if (strcmp(command[0], "exit") == 0)
  {
    exit(0);
  }

  if (strcmp(command[0], "cd") == 0)
  {
    chdir(command[1]);
    return 1;
  }

  return 0;
}

int main(void)
{
  signal(SIGINT, SIG_IGN);

  while (1)
  {
    char instruction[255];

    printf("my-shell $ ");
    gets(instruction);

    char **command = parse_command(instruction);

    if (handle_built_in(command))
    {
      continue;
    }

    int pid = fork();

    if (pid == 0)
    {
      signal(SIGINT, exit_process);

      execvp(command[0], command);

      printf("command not found: %s\n", command[0]);
      exit(127);
    }
    else
    {
      wait(&pid);
    }
  }

  return 0;
}