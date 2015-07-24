#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "object.h"
#include "xalloc.h"

#define OBJ_TAB_SIZE 1024

static obj_id_t next_id = 1;

static object_t *obj_tab[OBJ_TAB_SIZE];

static object_type_t *type_list;

static inline obj_id_t get_new_id(void)
{
  return next_id++;
}

static unsigned int hash_id(obj_id_t id)
{
  unsigned int i, hash = 0;
  for (i=0;i<sizeof(id);i++)
    hash = hash*33 + ((unsigned char *)&id)[i];
  return hash % OBJ_TAB_SIZE;
}

static void register_object(object_t *obj)
{
  unsigned int hash = hash_id(obj->id);
  obj->objlist_next = obj_tab[hash];
  obj_tab[hash] = obj;
}

static void unregister_object(object_t *obj)
{
  unsigned int hash = hash_id(obj->id);
  if (obj_tab[hash] == obj)
    {
      obj_tab[hash] = obj_tab[hash]->objlist_next;
    }
  else
    {
      object_t *p = obj_tab[hash];
      while (p && p->objlist_next != obj)
	p = p->objlist_next;
      assert(p && "Unregistering unknown object");
      p->objlist_next = obj->objlist_next;
    }
}

void *obj_alloc(size_t size, object_type_t *type) 
{
  object_t *o;
  assert(type != NULL && "Cannot create NULL-typed object");
  assert(size >= sizeof(object_t));
  o = xcalloc(1, size);
  o->refcount = 0;
  o->type = type;
  o->type->nr_created++;
  o->id = get_new_id();
  register_object(o);
#ifdef OBJ_DEBUG      
  fprintf(stderr,"obj_alloc(): Created object of type %s with id %lu\n",type->name,o->id);
#endif
  return o;
}

#ifndef OBJ_DEBUG
void *obj_ref(void *object, const object_type_t *type)
{
  object_t *o = object;
  if (obj_has_type(object,type))
    {
      o->refcount += 2;
    }
  else
    {
      fprintf(stderr,"Error in obj_ref(): Expected object of type %s, got %s.\n",type ? type->name : "NULL",o->type->name);
      assert(o->type != type);
    }
  return object;
}
#else
void *obj_ref_debug(void *object, const object_type_t *type,const char *file, int line)
{
  object_t *o = object;
  fprintf(stderr,"%s:%i obj_ref(%p <%s>, %s)\n",file,line,object,obj_type(object)->name,type ? type->name : "NULL");
  if (obj_has_type(object,type))
    {
      o->refcount += 2;
      fprintf(stderr,"obj_ref(): Refcount of %s object up to %i.\n",obj_type(object)->name,o->refcount/2);
    }
  else
    {
      fprintf(stderr,"Error in obj_ref(): Expected object of type %s, got %s.\n",type ? type->name : "NULL",o->type->name);
      assert(o->type != type);
    }
  return object;
}
#endif

int obj_alive(void *object)
{
  return (((object_t *)object)->refcount & 1) != 1;
}

#ifndef OBJ_DEBUG
void *obj_deref(void *object)
{
  /* Note: 'o' is static so that I can set a watchpoint on it:
   *   break obj_deref
   *   -> Breakpoint 1
   *   cont
   *   [...]
   *   del 1
   *   next
   *   watch (o->type == &(biplane.object_type))
   *   cont
   *   [...]
   *   print *o
   *   print *o.type
   * Note that this breaks only when the value of the given expression
   * changes, not when it's true...
   */
  static object_t *o;
  o = object;
  o->refcount -= 2;
  if (o->refcount <= 1)
    {
      if (o->refcount >= 0)
	{
	  o->type->nr_freed++;
	  unregister_object(object);
	  if (o->type->kill)	
	    o->type->kill(object);
	  free(object);
	}
      else
	{
	  /* This point is reached if we manage to keep a reference
	     and deref it even when refcount is 0, or if we deref an
	     object which was never referenced. In principle this can
	     never happen in a correct program. */
	  BUG("Error: Orphan or dying object dereferenced, have to quit.");
	}
    }
  return 0;
}
#else
void *obj_deref_debug(void *object, const char *file, int line)
{
  object_t *o = object;
  fprintf(stderr,"%s:%i obj_deref(%p <%s>)\n",file,line,object,obj_type(object)->name);
  o->refcount -= 2;
  if (o->refcount <= 1)
    {
      if (o->refcount >= 0)
	{
	  o->type->nr_freed++;
	  if (o->type->kill)	
	    o->type->kill(object);
	  fprintf(stderr,"obj_deref(): freeing object of type %s\n",obj_type(object)->name);
	  unregister_object(object);
	  free(object);
	}
      else
	{
	  fprintf(stderr,"obj_deref(): Orphan or dying object dereferenced (refcount var = %i)\n",
		  o->refcount);
	  BUG("Error: Orphan or dying object dereferenced, have to quit.");
	}
    }
  else
    {
      fprintf(stderr,"obj_deref(): Refcount of %s object down to %i.\n",obj_type(object)->name,o->refcount/2);
    }  
  return 0;
}
#endif

void *
#ifndef OBJ_DEBUG
obj_lock
#else
obj_lock_debug
#endif
(obj_id_t id, const object_type_t *assumed_type
#ifdef OBJ_DEBUG
 ,const char *file, int line
#endif
)
{
#ifdef OBJ_DEBUG
  if (assumed_type)
    fprintf(stderr,"%s:%i obj_lock(%lu, %s)\n",file,line,id,assumed_type->name);
  else
    fprintf(stderr,"%s:%i obj_lock(%lu, NULL)\n",file,line,id);
#endif  
  if (id)
    {
      unsigned int hash = hash_id(id);
      object_t *p = obj_tab[hash];
      while (p && p->id != id)
	p = p->objlist_next;
      if (p)
	obj_ref(p,assumed_type);
      return p;
    }
  else
    {
      return NULL;
    }
}

#ifndef OBJ_DEBUG
void obj_unlock(void *obj)
{
  if (obj)
    obj_deref(obj);
}
#else
void obj_unlock_debug(void *obj, const char *file, int line)
{
  if (obj)
    {
      fprintf(stderr,"%s:%i obj_unlock(%p <%s id: %lu>)\n",file,line,obj,obj_type(obj)->name,obj_id(obj));
      obj_deref(obj);
    } 
} 
#endif


void *obj_validate(void *object)
{  
  if (object && !obj_alive(object))
    return obj_deref(object);
  else
    return object;
}

#ifndef OBJ_DEBUG
void obj_kill(void *object)
{
  ((object_t *)object)->refcount |= 1;
}
#else
void obj_kill_debug(void *object, const char *file, int line)
{
  fprintf(stderr,"%s:%i obj_kill(%p <%s>)\n",file,line,object,obj_type(object)->name);
  ((object_t *)object)->refcount |= 1;
}
#endif

int obj_has_type(const void *object, const object_type_t *type)
{
  const object_type_t *otype = ((const object_t *)object)->type;
  while (otype)
    {
      if (otype == type)
	return 1;
      otype = otype->base_type;
    }
  if (type == 0)
    return 1;
  else
    return 0;
}

enum msgret obj_message(void *obj, msg_t msg)
{
  object_type_t *type = ((object_t *)obj)->type;
  while (!type->message)
    {
      type = type->base_type;
      if (!type)
	return MSG_RET_UNKNOWN;
    }
  return type->message(obj,msg);
}

/*
enum msgret obj_call(void *obj, char *args[])
{
  object_type_t *type = ((object_t *)obj)->type;
  while (!type->call)
    {
      type = type->base_type;
      if (!type)
	return MSG_RET_UNKNOWN;
    }
  return type->call(obj,args);
}
*/

/* Register a new object type ("class"), with a name
   and a function to kill the data of each object. The message and
   call functions can be set to NULL to disable that functionality. 
   In that case the function of the base_type is called instead, if 
   applicable. 
*/
void obj_type_init(object_type_t *type)
{
  type->nr_created = 0;
  type->nr_freed = 0;
  type->next = type_list;
  type_list = type;
#ifdef OBJ_DEBUG      
  fprintf(stderr,"obj_type_init(): Initialized type %s\n",type->name);
#endif
}

void obj_type_clear_counts(void)
{
   object_type_t *type = type_list;
   int count = 0;
   while (type)
     {
       type->nr_created = 0;
       type->nr_freed = 0;
       type = type->next;
       count++;
     }
#ifdef OBJ_DEBUG      
  fprintf(stderr,"obj_type_clear_counts(): Counts cleared for %d types\n",count);
#endif
}

unsigned int obj_type_count_created(object_type_t *type)
{
  assert(type);
  return type->nr_created;
}
unsigned int obj_type_count_alive(object_type_t *type)
{
  assert(type);
  return (type->nr_created - type->nr_freed);
}
unsigned int obj_type_count_freed(object_type_t *type)
{   
  assert(type);
  return type->nr_freed;
}

static int max_type_name_len(void)
{
  int len, max_len = 0;
  object_type_t *type = type_list;
  while (type)
    {
      len = strlen(type->name);
      if (len > max_len)
	max_len = len;
      type = type->next;
    }
  return max_len;
}

void obj_print_stats(void)
{  
  object_type_t *type = type_list;
  int len = max_type_name_len();
  fprintf(stderr,"Object statistics: (only used types printed)\n");
  fprintf(stderr," type %*c current (created)\n", len-12, ' ');
  while (type)
    {
      if (type->nr_created > 0)
	fprintf(stderr,"%-*s %i (%i)\n", len, type->name, type->nr_created-type->nr_freed, type->nr_created);
      type = type->next;
    }
}

static INLINE int obj_no_mulref(void *op)
{
  return ((object_t*)op)->refcount/2 > 1 ? 0 : 1;
}
static INLINE int obj_no_orphan(void *op)
{
  return ((object_t*)op)->refcount/2 < 1 ? 0 : 1;
}

static void obj_print_unok_types(int (*ok)(void*))
{
  object_t *op;
  int i;

  for (i=0;i<OBJ_TAB_SIZE;i++)
    {
      for (op = obj_tab[i]; op; op = op->objlist_next)
	{
	  if (!ok(op))
	    fprintf(stderr, "- %s\n",op->type->name);
	}
    }
}

void obj_check_refs(void)
{
  static int nr_multiple = 0, nr_orphan = 0, nr_killed = 0;
  int i, nr_mul = 0, nr_orph = 0, nr_ki = 0;
  object_t *op;

  for (i=0;i<OBJ_TAB_SIZE;i++)
    {
      for (op = obj_tab[i]; op; op = op->objlist_next)
	{
	  if (!obj_alive(op))
	    nr_ki++;
	  if (!obj_no_mulref(op))
	    nr_mul++;
	  else if (!obj_no_orphan(op))
	    nr_orph++;
	}
    }
  if (nr_mul && nr_mul != nr_multiple)
    {
      fprintf(stderr,"Warning: %i objects with multiple references.\n",nr_mul);
      obj_print_unok_types(obj_no_mulref);
      nr_multiple = nr_mul;
    }
  if (nr_orph && nr_orph != nr_orphan)
    {
      fprintf(stderr,"Warning: %i objects leaked in total.\n",nr_orph);
      obj_print_unok_types(obj_no_orphan);
      nr_orphan = nr_orph;
    }
  if (nr_ki && nr_ki != nr_killed)
    {
      fprintf(stderr,"Warning: %i objects killed but not dereferenced.\n",nr_ki);
      obj_print_unok_types(obj_alive);
      nr_killed = nr_ki;
    }
}

void obj_print_living(void)
{
  int i, len = max_type_name_len();
  object_t *op;
  fprintf(stderr,"   id  type%*c refcount\n", len-5, ' ');  
  for (i=0;i<OBJ_TAB_SIZE;i++)
    {
      for (op = obj_tab[i]; op; op = op->objlist_next)
	fprintf(stderr,"%5lu %-*s %2i\n",op->id,len,op->type->name,op->refcount/2);
    }
}


struct objlist *objlist_new(void)
{
  struct objlist *a = xalloc(sizeof(*a));
  a->size = 0;
  a->max_size = 16;
  a->items = xalloc(sizeof(*a->items)*a->max_size);
  a->items[0] = 0;
  return a;
}

void objlist_delete(struct objlist *list)
{
  int i;
  for (i=0;i<list->size;i++)
    if (list->items[i])
      obj_deref(list->items[i]);
  free(list->items);
  free(list);
}

int objlist_insert(struct objlist *list, void *obj)
{
  if (list->size >= list->max_size)
    {
      list->max_size *= 2;
      list->items = xrealloc(list->items,
			      list->max_size*sizeof(*list->items));
    }
  list->items[list->size] = obj_ref(obj,NULL);
#ifdef OBJ_DEBUG
  printf("objlist_insert(): inserted object of type %s\n",obj_type(obj)->name);
#endif
  return list->size++;
}

void objlist_remove(struct objlist *list, const void *obj)
{
  int i;
  for (i = 0;i < list->size; i++)
    {
      if (obj == list->items[i])
	{
	  obj_deref(list->items[i]);
	  list->items[i] = list->items[list->size-1];
	  list->size--;
	  return;
	}
    }
}

void objlist_clean(objlist_t *list)
{
  int i,j = 0;
  for (i=0;i<list->size;i++)
    {
      list->items[i] = obj_validate(list->items[i]);
      if (list->items[i])
	{
	  if (j < i)
	    list->items[j] = list->items[i];
	  j++;
	}
    }
  list->size = j;
}

void objlist_killall(objlist_t *list)
{
  int i;
  for (i=0; i < list->size; i++)
    obj_kill(list->items[i]);
  objlist_clean(list);
}

objdict_t *objdict_new(void)
{
  objdict_t *dict = xalloc(sizeof(*dict));
  dict->max_len = 8;
  dict->len = 0;
  dict->def = xalloc(sizeof(*dict->def)*dict->max_len);
  return dict;
}

void objdict_delete(objdict_t *dict)
{
  int i;
  for (i=0;i<dict->len;i++)
    free(dict->def[i].name);
  free(dict->def);
  free(dict);
}

void objdict_define(objdict_t *dict, const char *name, obj_id_t id)
{
  assert(id != 0);
  while (dict->len >= dict->max_len)
    {
      dict->max_len*=2;
      dict->def = xrealloc(dict->def,sizeof(*dict->def)*dict->max_len);
    }
  dict->def[dict->len].name = xstrdup(name);
  dict->def[dict->len].value  = id;
  dict->len++;
}

void objdict_remove_name(objdict_t *dict, const char *name)
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

void objdict_remove_value(objdict_t *dict, obj_id_t id)
{
  int i;
  for (i=0;i<dict->len;i++)
    if (id == dict->def[i].value)
      {
	free(dict->def[i].name);
	if (dict->len > i + 1)
	  dict->def[i] = dict->def[dict->len - 1];
	dict->len--;
	i--;
      }
}

obj_id_t objdict_lookup(objdict_t *dict, const char *name)
{
  int i;
  for (i=0;i<dict->len;i++)
    if (strcmp(name,dict->def[i].name) == 0)
      return dict->def[i].value;
  return 0;
}

void objdict_clean(objdict_t *dict)
{
  int i;
  void *obj;
  for (i=0;i<dict->len;i++)
    {
      obj = obj_lock(dict->def[i].value,NULL);
      if (obj)
	obj_unlock(obj);
      else
	dict->def[i].value = 0;
      if (!dict->def[i].value)
	{
	  free(dict->def[i].name);
	  if (dict->len > i + 1)
	    dict->def[i] = dict->def[dict->len - 1];
	  dict->len--;
	  i--;
	}
    }
}

void objdict_clear(objdict_t *dict)
{
  dict->len = 0;
}


void obj_print_info(FILE *dst, void *obj)
{
  if (!obj)
    fprintf(dst,"<NULL object>\n");
  else
    fprintf(dst,"<%s at %p, id: %lu, refcount: %i, alive: %i>\n",
	    ((object_t *)obj)->type->name, obj, obj_id(obj), 
	    ((object_t *)obj)->refcount/2, obj_alive(obj));
}
