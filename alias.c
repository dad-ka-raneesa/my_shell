#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alias.h"

void add_alias(Alias **aliases, char_ptr exp)
{
  char_ptr *alias = parse_alias(exp, '=');

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

char_ptr get_actual(Alias *aliases, char_ptr aka)
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