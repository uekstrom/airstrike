#ifndef AI_H
#define AI_H

#include "sprite.h"
#include "dict.h"
#include "object.h"

#define AI_DEFAULT_USER "player1"
#define AI_DEFAULT_DUMMY "koala"

/* An AI is are owned by the sprite it controls, and
   is created by calling the apropriate function. The sprite
   must then call ai->update() on each frame and ai->free() on the
   ai when the sprite dies (or switches ai). The sprite should 
   also let the ai listen to the signals it gets. */

typedef struct ai
{
  object_t object;
  /* Should return MSG_RET_CONT if the message should go through to
     the sprite. */
  enum msgret (*intercept_msg)(struct ai *ai, sprite_t *target, msg_t msg);
  const char *name; // used to copy the AI, e.g. with the CLONE bonus
} ai_t;

extern object_type_t ai_type;

void ai_setup(void);

/* sets AI of given type to given sprite
 * returns AI object ID or zero
 */
obj_id_t ai_set_sprite_ai(sprite_t *s, const char *ai_type_name); 

typedef ai_t *ai_create_func_t(void);
/* return function that creates ai with given AI type name
 * can also be used to validate the AI type name
 */
ai_create_func_t *ai_get_func(const char *ai_type_name);

#endif
