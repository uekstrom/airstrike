/* 
 * (w) 2003 by Eero Tamminen
 */
#include "engine.h"

static animation_t *anim;

static int setup(void)
{
  anim = rc_load_animation("airballoon");
  animation_make_loop(anim);
  return 0;
}

static sprite_t *create(void)
{
  mech_sprite_t *s;
  s = obj_alloc(sizeof(*s),&airballoon.object_type);
  mech_defaults(s);
  s->rmass = 0.4;
  s->gravity = -0.01;
  ((sprite_t *)s)->animation = anim;
  return (sprite_t *)s;
}

static enum msgret message(void *this, msg_t msg)
{
  sprite_t *s = this;
  mech_sprite_t *ms = this;
  switch(msg.type)
    {
    case MSG_UPDATE:
      mech_update(ms);
      break;
    case MSG_SET_DAMAGE:
      ms->damage += msg_get_damage(msg);
      if (ms->damage > 10)
	{
	  create_effect(&explosion,s->pos);
	  sprite_kill(s);
	}
      ms->gravity *= -1; /*weird but fun */
      break;
    case MSG_KILL:
      create_effect(&explosion,s->pos);
      sprite_kill(s);
      break;
    default:
      return MSG_RET_UNKNOWN;
      break;
    }
  return MSG_RET_ACK;
}

sprite_type_t airballoon =
  {
    .object_type = {
      .name = "airballoon",
      .message = message,
      .base_type = &mech_sprite_type
    },
    .default_group = &mech_group,
    .setup = setup,
    .create_sprite = create,
  };

REGISTER_SPRITE_TYPE(airballoon)
