
#include "engine.h"

static animation_t *anim;

static int setup(void)
{
  anim = rc_load_animation("box");
  return 0;
}

static sprite_t *create(void)
{
  sprite_t *s;
  s = obj_alloc(sizeof(mech_sprite_t), &box.object_type);
  s->flags |= SPRITE_CANROTATE;
  mech_defaults((mech_sprite_t *)s);
  ((mech_sprite_t *)s)->rmass = 2;
  ((mech_sprite_t *)s)->rinmom = 10;
  ((mech_sprite_t *)s)->air_resistance = 0.0001;
  ((mech_sprite_t *)s)->air_resistance_ang = 0.000001;
  //  ((mech_sprite_t *)s)->gravity = 0;
  s->animation = anim;
  return s;
}

static void collide(sprite_t *this_sprite, 
		    sprite_t *other_sprite, 
		    int x, int y)
{
  /* TODO? */
  return;
}

static void collide_world(sprite_t *this_sprite, int x, int y)
{
  /* TODO? */
  return;
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
    case MSG_KILL:
      create_effect(&explosion, s->pos);
      sprite_kill(s);
      break;
    default:
      return MSG_RET_UNKNOWN;
      break;
    }
  return MSG_RET_ACK;
}


sprite_type_t box =
  {
    .object_type = {
      .name = "box",
      .base_type = &mech_sprite_type,
      .message = message,
    },
    .default_group = &mech_group,
    .setup = setup,
    .create_sprite = create,
    .collide = collide,
    .bg_collide = collide_world
  };

REGISTER_SPRITE_TYPE(box)
