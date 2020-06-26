#ifndef __UTILS_H_
#define __UTILS_H_

typedef char *char_ptr;
typedef int *int_ptr;

char_ptr copy_string(char_ptr str, int start, int end);
char_ptr *parse_alias(char_ptr exp, char key);
char_ptr *parse_command(char_ptr instruction, char key);

#endif