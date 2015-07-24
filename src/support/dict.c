#include <assert.h>
#include <string.h>
#include "xalloc.h"
#include "dict.h"

dict_t *dict_new(void)
{
  dict_t *dict = xalloc(sizeof(*dict));
  dict->max_len = 8;
  dict->len = 0;
  dict->def = xalloc(sizeof(*dict->def)*dict->max_len);
  return dict;
}

void dict_delete(dict_t *dict)
{
  int i;
  for (i=0;i<dict->len;i++)
    free(dict->def[i].name);
  free(dict->def);
  free(dict);
}

void dict_define(dict_t *dict, const char *name, void *value)
{
  assert(value != 0);
  while (dict->len >= dict->max_len)
    {
      dict->max_len*=2;
      dict->def = xrealloc(dict->def,sizeof(*dict->def)*dict->max_len);
    }
  dict->def[dict->len].name = xstrdup(name);
  dict->def[dict->len].value = value;
  dict->len++;
}

void dict_remove_name(dict_t *dict, const char *name)
{
  int i;
  for (i=0;i<dict->len;i++)
    if (strcmp(name,dict->def[i].name) == 0)
      {
	free(dict->def[i].name);
	if (dict->len > i + 1)
	  dict->def[i] = dict->def[dict->len - 1];
	dict->len--;
	i--;
      }
}

void dict_remove_value(dict_t *dict, const void *value)
{
  int i;
  for (i=0;i<dict->len;i++)
    if (value == dict->def[i].value)
      {
	free(dict->def[i].name);
	if (dict->len > i + 1)
	  dict->def[i] = dict->def[dict->len - 1];
	dict->len--;
	i--;
      }
}

void *dict_lookup(dict_t *dict, const char *name)
{
  int i;
  for (i=0;i<dict->len;i++)
    if (strcmp(name,dict->def[i].name) == 0)
      return dict->def[i].value;
  return 0;
}

