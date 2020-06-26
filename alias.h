#ifndef __ALIAS_H_
#define __ALIAS_H_

typedef struct alias
{
  char *aka;
  char *actual;
  struct alias *next;
} Alias;

char *copy_s(char *str, int start, int end);
char **parse_alias(char *exp);
void add_alias(Alias **aliases, char *exp);
char *get_actual(Alias *aliases, char *aka);
void show(Alias *aliases);

#endif