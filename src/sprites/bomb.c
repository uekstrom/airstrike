#include "engine.h"


#define BOMB_DAMAGE 20

static animation_t *anim;

static void killme(void *sp)
{
  sprite_t *s = (sprite_t *)sp;
  create_effect(&explosion,s->pos);
  sprite_kill(s);
}

static int setup(void)
{
  anim = rc_load_animation("bomb");
  animation_last_frame(anim)->trigger = killme;
  return 0;
}

static sprite_t *create(void)
{
  sprite_t *s;
  s = obj_alloc(sizeof(mech_sprite_t), &bomb.object_type);

  s->anim_p = 0;
  s->animation = anim;
  s->flags |= SPRITE_CANROTATE;
  mech_defaults((mech_sprite_t *)s);
  ((mech_sprite_t *)s)->angle = 0;
  ((mech_sprite_t *)s)->rmass = 4;
  ((mech_sprite_t *)s)->gravity = 0.45;
  ((mech_sprite_t *)s)->air_resistance_ang = 0.01;
  
  sprite_alarm(5,s,msg_activate());
  return s;
}

  /* stabilizing force from air */
static void aerodynamics(mech_sprite_t *s)
{
  vector_t vn;
  float x;
  vn = vnormalize2(s->sprite.vel);
  x = vdot(vn,mech_heading(s));
  if (x < 0)
    x*=0.5;
  s->ang_impulse -= x*vcross(s->sprite.vel,mech_heading(s))*0.08;

};

static enum msgret message(void *obj, msg_t msg)
{
  switch (msg.type)
    {
    case MSG_UPDATE:
      aerodynamics(obj);
      mech_update(obj);
      break;
    case MSG_ACTIVATE: /* Arm the bomb, so that it explodes on collision */
      ((sprite_t *)obj)->flags |= SPRITE_ACTIVE;
      break;
    default:
      return MSG_RET_UNKNOWN;
    }
  return MSG_RET_ACK;
}

static void collide(struct sprite *this_sprite, 
		    struct sprite *other_sprite, 
		    int x, int y)
{
  if (this_sprite->flags & SPRITE_ACTIVE)
    {
      sprite_msg(other_sprite,msg_set_damage(BOMB_DAMAGE));
      create_effect(&explosion,this_sprite->pos);
      sprite_kill(this_sprite);
    }
}

static void collide_world(struct sprite *this_sprite, int x, int y)
{
  if (this_sprite->flags & SPRITE_ACTIVE)
    {
      create_effect(&explosion,this_sprite->pos);
      sprite_kill(this_sprite);
    }
}

sprite_type_t bomb =
  {
    .object_type = {
      .name = "bomb",
      .base_type = &mech_sprite_type,
      .message = message,
    },
    .default_group = &mech_group,
    .setup = setup,
    .create_sprite = create,
    .collide = collide,
    .bg_collide = collide_world,
  };

REGISTER_SPRITE_TYPE(bomb)
