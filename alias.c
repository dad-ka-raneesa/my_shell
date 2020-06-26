#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alias.h"

char *copy_s(char *str, int start, int end)
{
  char *n_str = malloc(sizeof(char) * (end - start));

  for (int i = start; i < end; i++)
  {
    n_str[i - start] = str[i];
  }

  return n_str;
}

char **parse_alias(char *exp)
{
  char **alias = malloc(sizeof(char *) * 2);

  int ins_len = strlen(exp);

  for (int i = 0; i < ins_len; i++)
  {
    if (exp[i] == 61)
    {
      alias[0] = copy_s(exp, 0, i);
      alias[1] = copy_s(exp, i + 1, ins_len);
      break;
    }
  }

  return alias;
}

void add_alias(Alias **aliases, char *exp)
{
  char **alias = parse_alias(exp);

  Alias *p_walk = *aliases;
  Alias *last = NULL;

  while (p_walk)
  {
    if (strcmp(p_walk->aka, alias[0]) == 0)
    {
      strcpy(p_walk->actual, alias[1]);
      return;
    }
    last = p_walk;
    p_walk = p_walk->next;
  }

  if (last)
  {
    last->next = malloc(sizeof(Alias));
    last = last->next;
  }
  else
  {
    last = malloc(sizeof(Alias));
    *aliases = last;
  }

  last->aka = malloc(sizeof(strlen(alias[0])));
  last->actual = malloc(sizeof(strlen(alias[1])));

  strcpy(last->aka, alias[0]);
  strcpy(last->actual, alias[1]);
}

char *get_actual(Alias *aliases, char *aka)
{
  Alias *p_walk = aliases;

  while (p_walk)
  {
    if (strcmp(p_walk->aka, aka) == 0)
    {
      return p_walk->actual;
    }
    p_walk = p_walk->next;
  }

  return aka;
}

void show(Alias *aliases)
{
  Alias *p_walk = aliases;

  while (p_walk)
  {
    printf("%s='%s'\n", p_walk->aka, p_walk->actual);
    p_walk = p_walk->next;
  }
}