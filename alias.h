#ifndef __ALIAS_H_
#define __ALIAS_H_
#include "utils.h"

typedef struct alias
{
  char_ptr aka;
  char_ptr actual;
  struct alias *next;
} Alias;

void add_alias(Alias **aliases, char_ptr exp);
char_ptr get_actual(Alias *aliases, char_ptr aka);
void show(Alias *aliases);

#endif