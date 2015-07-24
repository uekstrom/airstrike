#ifndef DICT_H
#define DICT_H

#include "object.h"

typedef struct dict
{
  int len;
  int max_len;
  struct dict_def
  {
    char *name;
    void *value;
  } *def;
} dict_t;

dict_t *dict_new(void);
void dict_delete(dict_t *dict);
void dict_define(dict_t *dict, const char *name, void *value);
void dict_remove_name(dict_t *dict, const char *name);
void dict_remove_value(dict_t *dict, const void *value);
void *dict_lookup(dict_t *dict, const char *name);


#endif
