#include "utils.h"

char_ptr ltrim(char_ptr string)
{
  while (isspace(*string))
    string++;
  return string;
}

char_ptr rtrim(char_ptr string)
{
  int i;
  for (i = strlen(string) - 1; isspace(string[i]); i--)
  {
  }
  string[i + 1] = '\0';
  return string;
}

char_ptr trim(char_ptr string)
{
  return rtrim(ltrim(string));
}

char_ptr copy_string(char_ptr str, int start, int end)
{
  char_ptr n_str = malloc(sizeof(char) * (end - start));

  for (int i = start; i < end; i++)
  {
    n_str[i - start] = str[i];
  }

  return n_str;
}

char_ptr *parse_alias(char_ptr exp, char key)
{
  char_ptr *split = malloc(sizeof(char_ptr) * 2);
  int ins_len = strlen(exp);

  for (int i = 0; i < ins_len; i++)
  {
    if (exp[i] == key)
    {
      split[0] = copy_string(exp, 0, i);
      split[1] = copy_string(exp, i + 1, ins_len);
      return split;
    }
  }

  return NULL;
}

char_ptr *parse_command(char_ptr instruction, char key)
{
  char_ptr *split = malloc(sizeof(char_ptr) * 10);
  int s_count = 0;

  int ins_len = strlen(instruction);

  int start = 0;
  for (int i = 0; i <= ins_len; i++)
  {
    if (instruction[i] == key || instruction[i] == 0)
    {
      split[s_count++] = copy_string(instruction, start, i);
      start = i + 1;
    }
  }
  split[s_count] = NULL;

  return split;
}

int includes(char_ptr text, char delimiter)
{
  int is_found = 0;
  for (int i = 0; i < strlen(text) && !is_found; i++)
  {
    if (text[i] == delimiter)
    {
      is_found = 1;
    }
  }
  return is_found;
}

int includes_array(char_ptr *command, char delimiter)
{
  int is_found = 0;
  int index = -1;
  int i = 0;
  while (!is_found && (command[i]))
  {
    if (includes(command[i], delimiter))
    {
      is_found = 1;
      index = i;
    }
    i++;
  }

  return index;
}
