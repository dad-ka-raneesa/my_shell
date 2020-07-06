#ifndef __FILE_UTILS_H_
#define __FILE_UTILS_H_

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "utils.h"

int handle_file_append(char_ptr *args, int index, int fd, mode_t mode);
int handle_file_overwrite(char_ptr *args, int index, int fd);
int handle_redirection(char_ptr *command);

#endif