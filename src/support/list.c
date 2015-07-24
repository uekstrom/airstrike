#include "list.h"
#include "xalloc.h"
#include <stdio.h>

struct list *list_new(void)
{
  struct list *a = xalloc(sizeof(*a));
  a->size = 0;
  a->max_size = 16;
  a->items = xalloc(sizeof(*a->items)*a->max_size);
  a->items[0] = 0;
  return a;
}

void list_delete(struct list *list)
{
  free(list->items);
  free(list);
}

int list_insert(struct list *list, void *obj)
{
  if (list->size >= list->max_size)
    {
      list->max_size *= 2;
      list->items = xrealloc(list->items,
			     list->max_size*sizeof(*list->items));
    }
  list->items[list->size] = obj;
  return list->size++;
}

void list_remove(struct list *list, const void *obj)
{
  int i;
  for (i = 0;i < list->size; i++)
    {
      if (obj == list->items[i])
	{
	  list->items[i] = list->items[list->size-1];
	  list->size--;
	  return;
	}
    }
}

void list_clean(list_t *list)
{
  int i,j = 0;
  for (i=0;i<list->size;i++)
    {
      if (list->items[i])
	{
	  if (j < i)
	    list->items[j++] = list->items[i];
	}
    }
  list->size = j;
}

