#ifndef LIST_H
#define LIST_H

typedef struct list
{
  void **items;
  int size,max_size;
} list_t;

struct list *list_new(void);
void list_delete(struct list *list);
int list_insert(struct list *list, void *obj); // Append the item, return the index of the item
void list_remove(struct list *list, const void *obj); //Remove obj, not keeping order.
void list_clean(struct list *list); // Remove all null pointers in the list, keeping order.


#endif
