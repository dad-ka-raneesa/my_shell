#ifndef __ALIAS_H_
#define __ALIAS_H_
#include "utils.h"

typedef struct alias
{
  char_ptr aka;
  char_ptr actual;
  struct alias *next;
} Alias;

char_ptr copy_s(char_ptr str, int start, int end);
char_ptr *parse_alias(char_ptr exp);
void add_alias(Alias **aliases, char_ptr exp);
char_ptr get_actual(Alias *aliases, char_ptr aka);
void show(Alias *aliases);

#endif