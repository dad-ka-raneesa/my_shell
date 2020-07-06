#include "file_utils.h"

int handle_file_append(char_ptr *args, int index, int fd, mode_t mode)
{
  int new_fd = open(args[index + 1], mode, 0644);
  if (new_fd == -1)
  {
    perror("zsh: ");
    return new_fd;
  }
  dup2(new_fd, fd);
  args[index] = NULL;
  return new_fd;
}

int handle_file_overwrite(char_ptr *args, int index, int fd)
{
  int new_fd = creat(args[index + 1], 0644);
  dup2(new_fd, fd);
  args[index] = NULL;
  return new_fd;
}

int handle_redirection(char_ptr *command)
{
  int index;
  index = includes_array(command, '>');
  if (index > 0 && strcmp(command[index], ">>") == 0)
  {
    return handle_file_append(command, index, 1, O_WRONLY | O_CREAT | O_APPEND);
  }
  if (index > 0 && strcmp(command[index], "2>>") == 0)
  {
    return handle_file_append(command, index, 2, O_WRONLY | O_CREAT | O_APPEND);
  }
  if (index > 0 && strcmp(command[index], "2>") == 0)
  {
    return handle_file_overwrite(command, index, 2);
  }
  if (index > 0)
  {
    return handle_file_overwrite(command, index, 1);
  }
  index = includes_array(command, '<');
  if (index > 0)
  {
    return handle_file_append(command, index, 0, O_RDONLY);
  }
  return 0;
}