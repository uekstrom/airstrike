#include "engine.h"

#define DEF_MISSILE_AI "seeker" // default ai to use for missiles

typedef struct missile_model
{
  float engine_strength;
  float turn_amount;
  float mass;
  float air_resistance;
  unsigned int damage;
  unsigned int hitpoints;
  unsigned int fuel;
} missile_model_t;

static missile_model_t aim_nine =
{
  .engine_strength = 0.8,
  .turn_amount = 0.15,
  .mass = 0.5,
  .air_resistance = 0.015,
  .damage = 10,
  .hitpoints = 15,
  .fuel = 1000,
};

struct missile
{
  mech_sprite_t base;
  unsigned int fuel;
  ai_t *ai; /* ai knows about target to follow */
  missile_model_t *model;
  sprite_t *mark;
  sprite_t *target;
};

static animation_t *anim;

static void make_smoke(void *s)
{
  if( ((struct missile *)s)->fuel > 0 )
    create_effect(&puff, ((sprite_t *)s)->pos);
}

static int setup(void)
{
  anim = rc_load_animation("missile");
  anim->trigger = make_smoke;
  return 0;
}

static void kill_missile(void *s)
{
  if (((struct missile *)s)->ai)
    obj_deref(((struct missile *)s)->ai);
  if (((struct missile *)s)->mark)
    sprite_kill(((struct missile *)s)->mark);
}

static void set_model(struct missile *mis, missile_model_t *model)
{
  mis->model = model;
  ((mech_sprite_t *)mis)->rmass = 1/model->mass;
  ((mech_sprite_t *)mis)->gravity = model->mass;
  ((mech_sprite_t *)mis)->air_resistance = model->air_resistance;
  mis->fuel = model->fuel;
}

static sprite_t *create(void)
{
  struct missile *mis = obj_alloc(sizeof(*mis), &missile.object_type);
  mech_sprite_t *ms = (mech_sprite_t *)mis;
  sprite_t *s = (sprite_t *)mis;

  ms->sprite.flags |= SPRITE_CANROTATE;
  mech_defaults(ms);

  set_model(mis,&aim_nine);

  s->animation = anim;

  ai_set_sprite_ai(s, DEF_MISSILE_AI); // set default missile ai
  if(global_aim.missile && obj_alive(global_aim.missile)) {
    sprite_msg(s, msg_set_target(global_aim.missile)); // set target
    mis->target = global_aim.missile;
    mis->mark = create_effect(&mark, global_aim.missile->pos); // target marker
    s->vel = vnormalize(vsub(global_aim.missile->pos, s->pos)); // face target
  } else {
    mis->target = mis->mark = NULL;
  }

  return (sprite_t *)ms;
}

static void collide(sprite_t *this_sprite, 
		    sprite_t *other_sprite, 
		    int x, int y)
{
  unsigned int damage = ((struct missile *)this_sprite)->model->damage;
  
#if 0 /* let missiles collide with other missiles */
  if (obj_type(other_sprite) == &missile.object_type) 
    return;
#endif

  create_effect(&explosion, vector(x,y));
  sprite_msg(other_sprite, msg_set_damage(damage));
  sprite_kill(this_sprite);
}

static void collide_world(sprite_t *this_sprite, int x, int y)
{
  sprite_t *s;
  
  s = sprite_create(&explosion);
  sprite_set_pos(s,x,y);
  sprite_kill(this_sprite);
}

static void aerodynamics(mech_sprite_t *ms)
{
  vector_t nvel, nhead;
  float s_angle, abs_vel;
  static const float stab_c = 0.05;
  sprite_t *s = (sprite_t *)ms;

  /* airflow around missile stabilizes it */
  abs_vel = vabs(s->vel);
  nvel = vnormalize(s->vel);
  nhead = vnormalize(mech_heading(ms));
  s_angle = vcross(nvel, nhead);
  ms->ang_vel -= s_angle * abs_vel * stab_c;
}

static enum msgret message(void *obj, msg_t msg)
{
  sprite_t *s = obj;
  mech_sprite_t *ms = (mech_sprite_t *)obj;
  struct missile *mis = (struct missile *)obj;
  enum msgret res;

  /* Let the ai listen to the signals */
  if (mis->ai) {			/* if there is some ai listening */
    res = mis->ai->intercept_msg(mis->ai,obj,msg);
  } else {
    res = MSG_RET_UNKNOWN;		/* let missiles fall straight down */
  }
  if (res != MSG_RET_UNKNOWN && res != MSG_RET_CONT)
    return res;

  switch(msg.type)
    {
    case MSG_UPDATE:
      aerodynamics(ms);
      mech_update(obj);      
      if(mis->target && obj_alive(mis->target)) {
        sprite_set_pos(mis->mark, mis->target->pos.x, mis->target->pos.y);
      } else {
        if(mis->mark) {
          sprite_kill(mis->mark);
	  mis->mark = NULL;
	}
      }
      break;
    case MSG_CAN_HAVE_AI:
      return MSG_RET_YES;
      break;
    case MSG_SET_AI:
      if(mis->ai)
        obj_deref(mis->ai);
      mis->ai = obj_ref(msg_get_ai(msg),&ai_type);
      break;
    case MSG_SET_TARGET:
      mis->target = msg_get_target(msg);
      break;
    case MSG_IS_HARMLESS:
      if (s->state & SPRITE_CRASHING)
	return MSG_RET_YES;
      else
	return MSG_RET_NO;
      break;
    case MSG_SET_DAMAGE:
      ((mech_sprite_t *)s)->damage += msg_get_damage(msg);
      if( ((mech_sprite_t *)s)->damage >= mis->model->hitpoints ) {
	s->state |= SPRITE_CRASHING;
	create_effect(&explosion, s->pos);
	sprite_kill(s);
      }
      break;
    case MSG_UP:
      if(mis->fuel > 0) {
        if(vdot(s->vel,mech_heading(ms)) < 0.0) {
          ms->ang_vel += mis->model->turn_amount;
        } else {
          ms->ang_vel -= mis->model->turn_amount;
        }
        s->state |= SPRITE_UP;
        s->state &= ~SPRITE_DOWN;
      }
      break;
    case MSG_DOWN:
      if(mis->fuel > 0) {
        if(vdot(s->vel,mech_heading(ms)) < 0.0) {
          ms->ang_vel -= mis->model->turn_amount;
        } else {
          ms->ang_vel += mis->model->turn_amount;
        }
        s->state |= SPRITE_DOWN;
        s->state &= ~SPRITE_UP;
      }
      break;
    case MSG_ACCELERATE:
      if(mis->fuel > 0) {
        s->state |= SPRITE_ACCELERATING;
        ms->lin_impulse = vmadd(ms->lin_impulse,mech_heading(ms),
			        mis->model->engine_strength);
	mis->fuel--;
      } else {
	if(mis->target && obj_alive(mis->target)) {
	  msg_t clear_target_msg = msg_set_target(NULL);
	  sprite_msg(s, clear_target_msg);
	}
      }
      break;
    case MSG_OUTOFBOUNDS: // fall through to MSG_KILL
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


sprite_type_t missile =
  {
    .object_type = {
      .name = "missile",
      .message = message,
      .kill = kill_missile,
      .base_type = &mech_sprite_type,
    },
    .default_group = &mech_group,
    .setup = setup,
    .create_sprite = create,
    .collide = collide,
    .bg_collide = collide_world,
  };

REGISTER_SPRITE_TYPE(missile)
