#ifndef MSG_TYPES_H
#define MSG_TYPES_H

#include "engine.h"

/* Naming conventions:
 * - Message creation functions are named msg_*
 * - To test for a specific message type check use msg.type
 * - To analyze message contents, use msg_get_* functions,
 *   for example:
 *      int msg_get_damage(msg_t msg).
 */
enum msg_type /* every message must be in msg_names (in msg_types.c) as well */
  {
    MSG_INVALID=-1, /* Indicates a programming error, not to be used
		       for actual communication. */
    /* message that should be ignored */
    MSG_IGNORE=0,

    /* Update message, sent every frame. Replaces old update function. */
    MSG_UPDATE,

    /* object de/activation */
    MSG_KILL,		/* object should kill itself */
    MSG_SELF_DESTRUCT,	/* object should kill itself visually pleasing */
    MSG_DEACTIVATE,
    MSG_ACTIVATE,

    /* query messages about sprite capabilities/properties:
     * - asked with sprite_ask()
     * - answer either MSG_RET_YES or MSG_RET_NO
     */
    MSG_CAN_HAVE_AI,
    MSG_IS_ACTIVE,
    MSG_IS_HARMLESS,
    MSG_ACCEPTS_BONUS,

    /* set sprite properties */
    MSG_SET_AI,
    MSG_SET_BONUS,
    MSG_SET_DAMAGE,
    MSG_SET_TARGET,
    MSG_SET_VARIANT,

    /* sprite control messages */
    MSG_ACCELERATE,
    MSG_BREAK,
    MSG_UP,
    MSG_DOWN,
    MSG_FIRE,
    MSG_DROP,	/* drop one of the contained objects (e.g. bomb) */

    /* ask the sprite to send a message to another sprite */
    MSG_COPY_AI, /* copy own AI to the other sprite */

    /* messages from the sprite framework */
    MSG_WARP, /* sprite was moved instantly (teleport or edge of level) */
    MSG_BGSTUCK,
    MSG_OUTOFBOUNDS,

    MSG_ENDOFTYPES /* Must be the last message type! */
  };

/* Try to convert a message type name to a
   message type number. Return MSG_INVALID
   if no matching name was found. 
   Names are exactly like the enum symbols.
*/
enum msg_type str_to_msg_type(const char *type_name);
/* If msg_type is a valid message type return
   a string representation of its name. The string remains
   property of this function. */
const char *msg_type_name(enum msg_type type);

/* These two functions should be used only by the framework, not by sprites */
static INLINE msg_t msg_ignore(void)
{
  msg_t m = {MSG_IGNORE};
  return m;
}
static INLINE msg_t msg_short(enum msg_type type)
{
  msg_t m;
  m.type = type;
  return m;
}

static INLINE msg_t msg_update(void)
{
  msg_t m = {MSG_UPDATE};
  return m;
}

static INLINE msg_t msg_kill(void)
{
  msg_t m = {MSG_KILL};
  return m;
}

static INLINE msg_t msg_deactivate(void)
{
  msg_t m = {MSG_DEACTIVATE};
  return m;
}

static INLINE msg_t msg_activate(void)
{
  msg_t m = {MSG_ACTIVATE};
  return m;
}

/* asks whether object has given capability, returns zero if false */
static int INLINE sprite_ask(sprite_t *s, enum msg_type type)
{
  int ret;
  msg_t m;
  m.type = type;
  assert(type == MSG_CAN_HAVE_AI ||
	 type == MSG_IS_ACTIVE   ||
	 type == MSG_IS_HARMLESS ||
	 type == MSG_ACCEPTS_BONUS);

  ret = sprite_msg(s,m);
  assert(ret == MSG_RET_YES ||
	 ret == MSG_RET_NO ||
	 ret == MSG_RET_UNKNOWN);

  return (ret == MSG_RET_YES);
}

static INLINE msg_t msg_set_ai(ai_t *ai)
{
  msg_t m = {MSG_SET_AI};
  m.data.d_ptr = ai;
  return m;
}

static INLINE ai_t *msg_get_ai(msg_t msg)
{
  assert(msg.type == MSG_SET_AI);
  return msg.data.d_ptr;
}

static INLINE msg_t msg_copy_ai(sprite_t *s)
{
  msg_t m = {MSG_COPY_AI};
  m.data.d_ptr = s;
  return m;
}

static INLINE sprite_t *msg_get_sprite(msg_t msg)
{
  assert(msg.type == MSG_COPY_AI);
  return msg.data.d_ptr;
}

static INLINE msg_t msg_set_damage(unsigned int damage)
{
  msg_t m = {MSG_SET_DAMAGE};
  m.data.d_int = damage;
  return m;
}

static INLINE unsigned int msg_get_damage(msg_t msg)
{
  assert(msg.type == MSG_SET_DAMAGE);
  return msg.data.d_int;
}

static INLINE msg_t msg_set_bonus(unsigned int bonustype)
{
  msg_t m = {MSG_SET_BONUS};
  m.data.d_int = bonustype;
  return m;
}

static INLINE int msg_get_bonus(msg_t msg)
{
  assert(msg.type == MSG_SET_BONUS);
  return msg.data.d_int;
}

static INLINE msg_t msg_set_target(sprite_t *target)
{
  msg_t m = {MSG_SET_TARGET};
  m.data.d_ptr = target;
  return m;
}

static INLINE sprite_t *msg_get_target(msg_t msg)
{
  assert(msg.type == MSG_SET_TARGET);
  return msg.data.d_ptr;
}

static INLINE msg_t msg_set_variant(char *variant)
{
  msg_t m = {MSG_SET_VARIANT};
  m.data.d_ptr = variant;
  return m;
}

static INLINE char *msg_get_variant(msg_t msg)
{
  assert(msg.type == MSG_SET_VARIANT);
  return msg.data.d_ptr;
}

/* Send a simple message to control a sprite */
static INLINE msg_t msg_ctrl(enum msg_type type)
{
  msg_t m;
  m.type = type;
  assert(type == MSG_ACCELERATE ||
	 type == MSG_BREAK ||
	 type == MSG_UP ||
	 type == MSG_DOWN ||
	 type == MSG_FIRE ||
	 type == MSG_DROP);
  return m;
}

static INLINE msg_t msg_fire(void)
{
  msg_t m = {MSG_FIRE};
  return m;
}

#endif
