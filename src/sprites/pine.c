#include "engine.h"

static animation_t *anim;
static animation_t *hit_anim;

static void frame_trigger(void *s)
{
  sprite_set_animation(s,anim);
  ((sprite_t *)s)->state = 0;
}

static int setup(void)
{
  anim = rc_load_animation("pine_still");
  hit_anim = rc_load_animation("pine_wave");
  animation_last_frame(hit_anim)->trigger = frame_trigger;
  return 0;
}

static sprite_t *create(void)
{
  mech_sprite_t *ms = obj_alloc(sizeof(*ms),&pine.object_type);
  sprite_t *s = (sprite_t *)ms;
  mech_defaults(ms);
  ms->rmass = 0;
  ms->gravity = 0;
  ms->sprite.flags |= SPRITE_IMMOBILE;
  s->animation = anim;
  return s;
}

static enum msgret message(void *obj, msg_t msg)
{
  sprite_t *s = obj;
  switch(msg.type)
    {
    case MSG_UPDATE:
      mech_boundary(obj);
      mech_update(obj);
      break;
    case MSG_SET_DAMAGE:
      ((mech_sprite_t *)s)->damage += msg_get_damage(msg);
      if (s->state == 0)
	{
	  sprite_set_animation(s, hit_anim);
	  s->state = 1;
	}
      break;
    default:
      return MSG_RET_UNKNOWN;
      break;
    }
  return MSG_RET_ACK;
}

sprite_type_t pine =
  {
    .object_type = {
      .name = "pine",
      .base_type = &mech_sprite_type,
      .message = message
    },
    .default_group = &mech_group,
    .setup = setup,
    .create_sprite = create,
  };

REGISTER_SPRITE_TYPE(pine)
