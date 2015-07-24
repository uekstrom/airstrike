#include "engine.h"

static animation_t *fire_left;
static animation_t *turn_lr;
static animation_t *fire_right;
static animation_t *turn_rl;

static void shoot_left(void *sp)
{
  float v;
  sprite_t *b, *s = sp;
  b = sprite_create(&cannonball);
  sprite_set_pos(b,s->pos.x - 28, s->pos.y - 26);
  v = -(frand() + 0.5)*2;
  b->vel.x = v;
  b->vel.y = v;
}

static void shoot_right(void *sp)
{
  float v;
  sprite_t *b, *s = sp;
  b = sprite_create(&cannonball);
  sprite_set_pos(b,s->pos.x + 28, s->pos.y - 26);
  v = (frand() + 0.5)*2;
  b->vel.x = v;
  b->vel.y = -v;
}

static int setup(void)
{
  fire_left = rc_load_animation("cannon-left");
  fire_right = rc_load_animation("cannon-right");
  turn_lr = rc_load_animation("cannon-turn-lr");
  turn_rl = rc_load_animation("cannon-turn-rl");
  animation_last_frame(fire_left)->trigger = shoot_left;
  animation_last_frame(fire_left)->next_frame = turn_lr;
  animation_last_frame(fire_right)->trigger = shoot_right;
  animation_last_frame(fire_right)->next_frame = turn_rl;
  animation_last_frame(turn_lr)->next_frame = fire_right;
  animation_last_frame(turn_rl)->next_frame = fire_left;
  return 0;
}

static sprite_t *create(void)
{
  sprite_t *s;
  mech_sprite_t *ms = obj_alloc(sizeof(*ms),&cannon.object_type);
  s = (sprite_t *)ms;
  s->flags |= SPRITE_IMMOBILE|SPRITE_INBG;
  mech_defaults(ms);
  ms->rmass = 0.000000001;
  ms->gravity = 0;
  s->animation = fire_left;
  return s;
}

sprite_type_t cannon =
  {
    .object_type = {
      .name = "cannon",
      .base_type = &mech_sprite_type,
    },
    .default_group = &mech_group,
    .setup = setup,
    .create_sprite = create,
  };

REGISTER_SPRITE_TYPE(cannon)

/* ------------------------------------------------------------ */

static animation_t *ball;

static void ball_frame_trigger(void *sp)
{
  sprite_t *s = sp;
  create_effect(&blacksmoke,s->pos);
}

static int ball_setup(void)
{
  ball = rc_load_animation("cannonball");
  ball->trigger = ball_frame_trigger;
  return 0;
}

static sprite_t *ball_create(void)
{
  sprite_t *s;
  mech_sprite_t *ms = obj_alloc(sizeof(*ms),&cannonball.object_type);
  s = (sprite_t *)ms;
  mech_defaults(ms);
  ms->rmass = 4;
  ms->gravity = 0.25;
  s->animation = ball;
  return s;
}

static enum msgret ball_message(void *obj, msg_t msg)
{
  sprite_t *s = obj;
  switch(msg.type)
    {
    case MSG_UPDATE:
      mech_update((mech_sprite_t *)s);
      break;
    case MSG_SET_DAMAGE:
      create_effect(&explosion,s->pos);
      sprite_kill(s);
      break;
    case MSG_OUTOFBOUNDS:
    case MSG_WARP:
    case MSG_KILL:
      sprite_kill(s);
      break;
    default:
      return MSG_RET_UNKNOWN;
      break;
    }
  return MSG_RET_ACK;
}

static void collide(struct sprite *this_sprite, 
		    struct sprite *other_sprite, 
		    int x, int y)
{
  unsigned int damage = 5;
  sprite_msg(other_sprite,msg_set_damage(damage));
  create_effect(&explosion,this_sprite->pos);
  sprite_kill(this_sprite);
}

static void collide_world(struct sprite *this_sprite, int x, int y)
{
  create_effect(&explosion,this_sprite->pos);
  sprite_kill(this_sprite);
}

sprite_type_t cannonball =
  {
    .object_type = {
      .name = "cannonball",
      .base_type = &mech_sprite_type,
      .message = ball_message,
    },
    .default_group = &mech_group,
    .setup = ball_setup,
    .create_sprite = ball_create,
    .collide = collide,
    .bg_collide = collide_world,
  };

REGISTER_SPRITE_TYPE(cannonball)
