#ifndef __UTILS_H_
#define __UTILS_H_

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef char *char_ptr;
typedef int *int_ptr;

char_ptr ltrim(char_ptr string);
char_ptr rtrim(char_ptr string);
char_ptr trim(char_ptr string);
char_ptr copy_string(char_ptr str, int start, int end);
char_ptr *parse_alias(char_ptr exp, char key);
int get_no_of_commands(char_ptr string, char splitBy);
char_ptr *parse_command(char_ptr instruction, char key);
int includes(char_ptr text, char delimiter);
int includes_array(char_ptr *command, char delimiter);

#endif