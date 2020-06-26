#include <stdlib.h>
#include "utils.h"

char_ptr copy_string(char_ptr str, int start, int end)
{
  char_ptr n_str = malloc(sizeof(char) * (end - start));

  for (int i = start; i < end; i++)
  {
    n_str[i - start] = str[i];
  }

  return n_str;
}