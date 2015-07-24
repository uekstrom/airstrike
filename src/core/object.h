#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>
#include "compat.h"

/* Airstrike object system

   The ultimate base type of all types is the NULL type, which
   can be passed to the obj_type_init() and obj_ref() functions,
   but not to object_init(), since no objects of this type are
   allowed to exists. Thus it is always safe to look at
   object->type->name, for example.
*/

/* Define OBJ_DEBUG to turn on verbose object logging */
/*#define OBJ_DEBUG */

typedef struct msg
{
  int type; /* Meaning defined depending on object type */
  union
  {
    /* Private members! No peeking, poking */
    int d_int;
    unsigned int d_uint;
    float d_float;
    void *d_ptr;
  } data;
} msg_t;

/* Signal/message function return values */
enum msgret {
        MSG_RET_UNKNOWN = 0,	/* message unknown/unhandled */
	MSG_RET_ACK,
        MSG_RET_YES,		/* message known/handled or 
				   reply to sprite_ask() */
        MSG_RET_NO,		/* reply to sprite_ask() */
	MSG_RET_CONT,           /* message handled, but it can
				   continue down a message chain. */
 };

struct object_type;

typedef unsigned long obj_id_t;

/* object_t must be the first member of every object struct. */  
typedef struct object
{
  struct object_type *type;
  obj_id_t id;
  struct object *objlist_next; /* for internal use */
  int refcount;
} object_t;

typedef struct object_type
{
  /* These members have to be filled before obj_type_init(): */
  const char *name;
  void (*kill)(void *object); /* Release all data owned by object, but doesn't
			       free() object. */
  enum msgret (*message)(void *obj, msg_t message);

  struct object_type *base_type;
  
  /* Private members, do not fill before obj_type_init(): */

  unsigned int nr_created;
  unsigned int nr_freed;

  struct object_type *next; /* For an internal list of types */
} object_type_t;

/* A list that refcounts all items.*/

typedef struct objlist
{
  void **items;
  int size, max_size;
} objlist_t;

/* objdict is same as dict, but it keeps a reference
   count to all objs */
typedef struct objdict
{
  int len;
  int max_len;
  struct objdict_def
  {
    char *name;
    obj_id_t value;
  } *def;
} objdict_t;

/* Allocates memory for a new object and initialize the object_t part
   of it. The size argument must be at least sizeof(struct object) */
void *obj_alloc(size_t size, object_type_t *type); 


/* Take a reference to object, and possibly perform type checking.
   May give a fatal error if obj_has_type(object,assumed_type) == 0
 
   You should close all function calls (which take an object as argument)
   inside obj_ref()/obj_deref() pair if they can potentially deref
   the object you gave it, and you'd like to use it afterwards.
 */
#ifdef OBJ_DEBUG
void *obj_ref_debug(void *object, const object_type_t *type, const char *file, int line); 
#define obj_ref(object,type) obj_ref_debug(object,type,__FILE__,__LINE__)
#else
void *obj_ref(void *object, const object_type_t *assumed_type);
#endif

/* If id gives a live object increase its refcount and
   return a pointer, else return NULL. Calling with id = 0
   will always return NULL.
 */
#ifndef OBJ_DEBUG 
void *obj_lock(obj_id_t id, const object_type_t *assumed_type);
#else
#define obj_lock(id,type) obj_lock_debug(id,type,__FILE__,__LINE__)
void *obj_lock_debug(obj_id_t id, const object_type_t *assumed_type,
		     const char *file, int line);
#endif

/* Decrease the reference count to obj, possibly freeing it.
   Calling with obj == NULL is ok (and is a nop).
 */
#ifndef OBJ_DEBUG
void obj_unlock(void *obj);
#else
#define obj_unlock(obj) obj_unlock_debug(obj,__FILE__,__LINE__)
void obj_unlock_debug(void *obj, const char *file, int line);
#endif


void *obj_create(object_type_t *type, char *args[]);

/* mark object as dead, don't do anything else */
#ifdef OBJ_DEBUG
void obj_kill_debug(void *object, const char *file, int line); 
#define obj_kill(object) obj_kill_debug(object,__FILE__,__LINE__)
#else
void obj_kill(void *object); 
#endif

/* Check if object is alive (was not killed) */
int obj_alive(void *object);

/* If dead release ref and return 0, else return object.
   Recommended usage for doing something with an object which
   may have been killed is

   obj = obj_validate(obj);
   if (obj)
     do_stuff(obj);

   It's ok to call obj_validate(NULL), which returns NULL.
 */
void *obj_validate(void *object); 


/* Release the reference to object previously taken with obj_ref().
   Returns NULL.

   The recommended usage is
     obj = obj_deref(obj);
   which then also sets obj to NULL.

   As a side-effect, this frees the object if it doesn't have any
   references left.  If the code gives the control of an object to
   to another object, pair the call with obj_ref/obj_deref like this:
     ai = obj_ref(ai_create(),&ai_type);
     sprite_msg(s,msg_set_ai(ai));
     ai = obj_deref(ai);
   That way the object isn't leaked.
 */
#ifndef OBJ_DEBUG
void *obj_deref(void *object); 
#else
void *obj_deref_debug(void *object, const char *file, int line);
#define obj_deref(object) obj_deref_debug(object,__FILE__,__LINE__)
#endif


/* Returns true if object is of assumed_type, or of a type that
   inherits from assumed_type. All types inherit from NULL. */
int obj_has_type(const void *object, const object_type_t *assumed_type);

/* Return the type of the object. */
static INLINE object_type_t *obj_type(void *object)
{
  return ((object_t *)object)->type;
}

static INLINE obj_id_t obj_id(void *object)
{
  return ((object_t *)object)->id;
}


/* Send a message to the object */
enum msgret obj_message(void *obj, msg_t msg);

enum msgret obj_call(void *obj, char *args[]);

void obj_print_info(FILE *dst, void *obj);

/* Print statistics on stderr */
void obj_print_stats(void);

/* return object type statistics */
unsigned int obj_type_count_alive(object_type_t *type);
unsigned int obj_type_count_created(object_type_t *type);
unsigned int obj_type_count_freed(object_type_t *type);

/* clear all object type counts */
void obj_type_clear_counts(void);

/* List all living objects to stderr */
void obj_print_living(void);

/* Register a new object type ("class"), with a name
   and a function to free the data of each object. The message and
   call functions can be set to NULL to disable that functionality. */
void obj_type_init(object_type_t *type);

/* Check for three kinds of mistakes: multiple references, leaked objects
   and dead objects. To be called between game frames. Print warnings on stderr.*/
void obj_check_refs(void);

objlist_t *objlist_new(void);
void objlist_delete(struct objlist *list);
int objlist_insert(objlist_t *list, void *item);
void objlist_remove(objlist_t *list, const void *item);
void objlist_clean(objlist_t *list);
void objlist_killall(objlist_t *list); /* kill all objects in list and clean  */

objdict_t *objdict_new(void);
void objdict_delete(objdict_t *dict);
void objdict_define(objdict_t *dict, const char *name, obj_id_t id);
void objdict_remove_name(objdict_t *dict, const char *name);
void objdict_remove_value(objdict_t *dict, obj_id_t id);
obj_id_t objdict_lookup(objdict_t *dict, const char *name);
/* Remove all items which are no longer valid. */
void objdict_clean(objdict_t *dict); 
/* Remove all items */ 
void objdict_clear(objdict_t *dict);

#endif
