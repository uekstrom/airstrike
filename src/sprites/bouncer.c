#include "engine.h"

static animation_t *anim;
static animation_t *hit_anim;
static animation_t *deflate_anim;

/* states */
enum {
  BOUNCER_FLOAT,
  BOUNCER_HIT,
  BOUNCER_DEFLATE
};

static void frame_trigger(void *sprite)
{
  sprite_t *s = sprite;
  sprite_set_animation(s,anim);
  s->state = BOUNCER_FLOAT;
}

static int setup(void)
{
  anim = rc_load_animation("balloon-float");
  hit_anim = rc_load_animation("balloon-bounce");
  deflate_anim = rc_load_animation("balloon-deflate");
  animation_last_frame(hit_anim)->trigger = frame_trigger;
  animation_last_frame(deflate_anim)->trigger = sprite_kill;
  return 0;
}

static sprite_t *create(void)
{
  mech_sprite_t *ms;
  ms = obj_alloc(sizeof(*ms),&bouncer.object_type);
  mech_defaults(ms);
  ms->rmass = 3;
  ms->gravity = 0.0;
  ms->bounce = 0.9;
  ms->sprite.animation = anim;
  return (sprite_t *)ms;
}

static void collide(struct sprite *this_sprite, 
		    struct sprite *other_sprite, 
		    int x, int y)
{
  if (this_sprite->state == BOUNCER_FLOAT)
    {
      sprite_set_animation(this_sprite, hit_anim);
      this_sprite->state = BOUNCER_HIT;
    }
}

static enum msgret message(void *this, msg_t msg)
{
  mech_sprite_t *ms = this;
  sprite_t *s = this;
  switch(msg.type)
    {
    case MSG_UPDATE:
      mech_boundary(ms);
      mech_update(ms);
      if (ms->damage > 5 && s->state != BOUNCER_DEFLATE)
	{
	  s->state = BOUNCER_DEFLATE;
	  sprite_set_animation(s,deflate_anim);
	}
      break;
    case MSG_SET_DAMAGE:
      ms->damage += msg_get_damage(msg);
      break;
    default:
      return MSG_RET_UNKNOWN;
      break;
    }
  return MSG_RET_ACK;
}

sprite_type_t bouncer =
  {
    .object_type = {
      .name = "bouncer",
      .message = message,
      .base_type = &mech_sprite_type
    },
    .default_group = &mech_group,
    .setup = setup,
    .create_sprite = create,
    .collide = collide,
  };

REGISTER_SPRITE_TYPE(bouncer)
