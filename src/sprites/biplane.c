#include "engine.h"
#include "string.h"

enum biplane_flags /* Different states the biplane can be in */
  {
    BIPLANE_ACCELERATING = 1,
    BIPLANE_UP = 2,
    BIPLANE_DOWN = 4,
    BIPLANE_CRASHING = 8,
    BIPLANE_DYING = 16,
    BIPLANE_INLOVE = 32
  };

/* how much bonuses give more stuff */
#define FUEL_BONUS_AMOUNT 200
#define BOMB_BONUS_COUNT  3

/* what are the maximums, these could be also model specific... */
#define FUEL_MAX_AMOUNT 1000
#define BOMB_MAX_COUNT  12

/* how often (in 100 ms) plane can do something */
#define PLANE_SAY_DELAY 5
#define PLANE_PUFF_DELAY 2

typedef struct biplane_model
{
  /* some parameters we get from config files */
  float engine_strength;
  float turn_amount;
  unsigned int bomb_delay;
  unsigned int bullet_delay;
  unsigned int hitpoints;
  float mass;
  float air_isotropic;
  float air_turnrate;
  float air_normal;  
  unsigned int nr_bombs;
  unsigned int fuel;

  animation_t *right_anim;

  struct biplane_model *crashing_model; /* Which model to switch to when the plane is out of hp */
} biplane_model_t;

static biplane_model_t red_baron_crashing = 
{
  .engine_strength = 0.0,
  .turn_amount = 0.001,
  .bomb_delay = 1200000,	/* N * 100ms */
  .bullet_delay = 30000000,
  .hitpoints = 0,
  .mass = 1,
  .air_isotropic = 0.0001,
  .air_turnrate = 0.002,
  .air_normal = 0.00,
  .nr_bombs = 0,
  .fuel = 0,
};

static biplane_model_t red_baron = 
{
  .engine_strength = 0.08,
  .turn_amount = 0.05,
  .bomb_delay = 12,	/* N * 100ms */
  .bullet_delay = 3,
  .hitpoints = 30,
  .mass = 1,
  .air_isotropic = 0.004,
  .air_turnrate = 0.005,
  .air_normal = 0.1,
  .nr_bombs = 5,
  .fuel = 200,		/* puffs */
  .crashing_model = &red_baron_crashing,
};

static biplane_model_t blue_duke_crashing = 
{
  .engine_strength = 0.0,
  .turn_amount = 0.02,
  .bomb_delay = 1200,	/* N * 100ms */
  .bullet_delay = 3000,
  .hitpoints = 30,
  .mass = 1,
  .air_isotropic = 0.0004,
  .air_turnrate = 0.005,
  .air_normal = 0.0,
  .nr_bombs = 5,
  .fuel = 200,		/* puffs */
};

static biplane_model_t blue_duke = 
{
  .engine_strength = 0.08,
  .turn_amount = 0.05,
  .bomb_delay = 12,	/* N * 100ms */
  .bullet_delay = 3,
  .hitpoints = 30,
  .mass = 1,
  .air_isotropic = 0.000,
  .air_turnrate = 0.005,
  .air_normal = 0.01,
  .nr_bombs = 5,
  .fuel = 200,		/* puffs */
  .crashing_model = &blue_duke_crashing,
};


struct biplane
{
  mech_sprite_t sprite; /* the biplane is also a mech sprite */
  sprite_type_t *bullet_type;
  unsigned int say_timer;
  unsigned int gun_timer; /* keeps track of when it last fired */
  unsigned int bomb_timer;
  unsigned int puff_timer;
  unsigned int nr_bombs;
  unsigned int fuel;
  struct ai *ai;
  biplane_model_t *model;
};

static void crashing_trigger(void *sp)
{
  sprite_t *p, *s = sp;
  vector_t v = { 0, -1 };
  p = sprite_create(&blacksmoke);
  sprite_set_pos(p, s->pos.x, s->pos.y);
  p->vel = v;
  create_effect(&fire, s->pos);
}

static int setup(void)
{
  red_baron.right_anim = rc_load_animation("red_baron_right");
  red_baron_crashing.right_anim = rc_load_animation("red_baron_crashing_right");
  red_baron_crashing.right_anim->trigger = crashing_trigger;

  blue_duke.right_anim = rc_load_animation("blue_duke_right");
  blue_duke_crashing.right_anim = rc_load_animation("blue_duke_crashing_right");
  blue_duke_crashing.right_anim->trigger = crashing_trigger;

  return 0;
}

static void set_model(struct biplane *bip, biplane_model_t *model)
{
  bip->model = model;
  ((mech_sprite_t *)bip)->rmass = 1/model->mass;
  ((mech_sprite_t *)bip)->gravity = model->mass;
  ((mech_sprite_t *)bip)->air_resistance = model->air_isotropic;
  bip->nr_bombs = model->nr_bombs;
  bip->fuel = model->fuel;
  ((sprite_t *)bip)->animation = model->right_anim;
}

static sprite_t *create(void)
{
  sprite_t *s;
  struct biplane *bip;
  bip = obj_alloc(sizeof(*bip),&biplane.object_type);
  s = (sprite_t *)bip;

  set_model(bip,&red_baron);
  
  s->anim_p = 0;
  s->flags |= SPRITE_CANROTATE;
  mech_defaults((mech_sprite_t *)s);
  bip->bullet_type = &bullet;

  ((mech_sprite_t *)s)->rinmom = 3;
  ((mech_sprite_t *)s)->air_resistance_ang = 0;//air_turnrate;
  ((mech_sprite_t *)s)->bounce = 0.2;

  return s;
}

static void aerodynamics(struct biplane *p)
{
  float lift,attack;
  vector_t vn;
  /* Lift power is proportional to (v.n)^2*f(v x n),
     where f is linear for small v x n,
   */
  vn = vnormalize2(p->sprite.sprite.vel);
  attack = sin(M_PI*vcross(vn,mech_heading(&p->sprite)));
  lift = sq(vdot(p->sprite.sprite.vel,mech_heading(&p->sprite)))*attack*p->model->air_normal;
  p->sprite.lin_impulse = vadd(p->sprite.lin_impulse,vmul(mech_right(&p->sprite),lift));

  /* stabilizing force from air */
  p->sprite.ang_impulse += vdot(vn,mech_heading(&p->sprite))*vcross(p->sprite.sprite.vel,mech_heading(&p->sprite))*0.01;

};

static void update(sprite_t *s)
{
  mech_sprite_t *ms = (mech_sprite_t *)s;

  aerodynamics((struct biplane *)s);

  if (!(s->state & BIPLANE_CRASHING))
    {
      if (ms->damage >= ((struct biplane *)s)->model->hitpoints)
	{
	  s->state |= BIPLANE_CRASHING;
	  if (((struct biplane *)s)->model->crashing_model)
	    {
	      set_model((struct biplane *)s,((struct biplane *)s)->model->crashing_model);
	    }
	  sprite_alarm(200,s,msg_kill());
	}
    }
  mech_update(ms);
}

static void collide_world(struct sprite *s, int x, int y)
{
  /* make sure that plane dies very shortly after crashlanding */
  if (s->state & BIPLANE_CRASHING && !(s->state & BIPLANE_DYING))
    {
      s->state |= BIPLANE_DYING;
      sprite_alarm(30,s,msg_kill());
    }
}

static void plane_say(struct biplane *bip, const char *message)
{
  sprite_t *s = (sprite_t*)bip;
  if (rate_limit(PLANE_SAY_DELAY, &bip->say_timer)) {
     ui_message(message, s->pos.x, s->pos.y - 20, ALIGN_CENTER, tag_font, 20);
  }
}

static int handle_control(struct biplane *bip, int action)
{
  biplane_model_t *model = bip->model;
  mech_sprite_t *ms = (mech_sprite_t *)bip;
  sprite_t *s = (sprite_t*)bip;
  sprite_t *p;
  float head;

  switch(action)
    {      
    case MSG_ACCELERATE:
      if (bip->fuel > 0) {
	s->state |= BIPLANE_ACCELERATING;
	ms->lin_impulse = vmadd(ms->lin_impulse,mech_heading(ms),model->engine_strength);
	if (rate_limit(PLANE_PUFF_DELAY, &bip->puff_timer)) {
	  vector_t v = mech_global_pos(ms,vector(-10,0));
	  if (ms->damage + 10 < bip->model->hitpoints) {
	    create_effect(&puff, v);
	  } else {
	   create_effect(&blacksmoke, v);
	  }
	  /* less fuel each puff... */
	  bip->fuel--;
	}
      } else {
	plane_say(bip, "no fuel");
      }
      break;

    case MSG_UP:
      head = vdot(s->vel,mech_heading(ms));
      if (head < 0.0) {
	// turn faster when going backwards & turn same way as forward
        ms->ang_vel += 2*model->turn_amount*head;
      } else {
        ms->ang_vel -= model->turn_amount*head;
      }
      s->state |= BIPLANE_UP;
      s->state &= ~BIPLANE_DOWN;
      break;

    case MSG_DOWN:
      head = vdot(s->vel,mech_heading(ms));
      if (head < 0.0) {
	// turn faster when going backwards & turn same way as forward
        ms->ang_vel -= 2*model->turn_amount*head;
      } else {
        ms->ang_vel += model->turn_amount*head;
      }
      s->state |= BIPLANE_DOWN;
      s->state &= ~BIPLANE_UP;
      break;

    case MSG_FIRE: /* create bullet */
      if (rate_limit(model->bullet_delay,&(bip->gun_timer)))
	{
	  if (s->state & BIPLANE_INLOVE) 
	    {
	      plane_say(bip, "Honey!");
	    } 
	  else 
	    {
	      sound_effect(&sound_gunfire, s->pos.x, s->pos.y);
	      p = sprite_create(bip->bullet_type);
	      p->pos = mech_global_pos(ms,vector(20,0));
	      p->vel = vadd(s->vel, vmul(mech_heading(ms),5));
	    }
	}
      break;

    case MSG_DROP: /* create bomb */
      if (rate_limit(model->bomb_delay,&bip->bomb_timer))
	{
	  if (bip->nr_bombs > 0)
	    {
	      sound_effect(&sound_bombdrop, s->pos.x, s->pos.y);
	      bip->nr_bombs--;
	      p = sprite_create(&bomb);
	      ((mech_sprite_t *)p)->angle = ms->angle;
	      mech_refresh_image((mech_sprite_t *)p);
	      p->pos = mech_global_pos(ms,vector(-4,10));
	      p->vel = vadd(s->vel, mech_heading(ms));
	    }
	  else
	    {
	      plane_say(bip, "no bombs");
	    }
	}
      break;
   }
   return 0;
}

static enum msgret message(void *obj, msg_t msg)
{
  int res;
  sprite_t *s = obj;
  mech_sprite_t *ms = obj;
  struct biplane *bip = (struct biplane *)s;
  
  if (bip->ai)
    {
      /* Let the ai listen to the signals */
      res = bip->ai->intercept_msg(bip->ai,obj,msg);
      if (res != MSG_RET_UNKNOWN && res != MSG_RET_CONT)
	return res;
    }
  else
    {
      if (msg.type == MSG_CAN_HAVE_AI)
	return MSG_RET_YES;
      shell_print("NOTE: message %s to brainless plane\n",
		  msg_type_name(msg.type));
    }

  switch(msg.type)
    {      
    case MSG_UPDATE:
      update(s);
      break;

    case MSG_IS_HARMLESS:
      if (s->state & BIPLANE_CRASHING)
	return MSG_RET_YES;
      else
	return MSG_RET_NO;
      break;

    case MSG_SET_DAMAGE:
      {
	char cb[12];
	int damage = msg_get_damage(msg);
	if (damage > 0)
	  {
	    ((mech_sprite_t *)s)->damage += damage;
	    sprintf(cb,"-%i", damage);
	    ui_message(cb, s->pos.x,s->pos.y + 10, ALIGN_CENTER, tag_font, 20);
	  }
      }
      break;

    case MSG_SELF_DESTRUCT:
      if(ms->damage < bip->model->hitpoints)
	sprite_msg(s, msg_set_damage(bip->model->hitpoints - ms->damage));
      return MSG_RET_YES;
      break;

    case MSG_ACCELERATE:
    case MSG_UP:
    case MSG_DOWN:
    case MSG_FIRE:
    case MSG_DROP:
      if (handle_control(bip, msg.type))
        /* sprite cannot be controlled ATM, i.e. message not handled */
	return MSG_RET_UNKNOWN;
      break;

    case MSG_KILL:
      create_effect(&explosion,s->pos);
      global_aim_clear(s);
      sprite_kill(s);
      break;

    case MSG_SET_AI:
      if (bip->ai)
	obj_deref(bip->ai);
      bip->ai = obj_ref(msg_get_ai(msg),&ai_type);
      break;

    case MSG_COPY_AI:
      if(bip->ai)
	if(!(ai_set_sprite_ai(msg_get_sprite(msg), bip->ai->name)))
	  fprintf(stderr, "ERROR: biplane could not copy AI\n");

    case MSG_ACCEPTS_BONUS:
      /* accepts bonus attentions */
      return MSG_RET_YES;

    case MSG_SET_BONUS:
      switch(msg_get_bonus(msg))
        {
	case BONUS_BOMB:
	  bip->nr_bombs += BOMB_BONUS_COUNT;
	  if (bip->nr_bombs > BOMB_MAX_COUNT)
	    bip->nr_bombs = BOMB_MAX_COUNT;
	  break;
	case BONUS_ENERGY:
	  ms->damage = 0;
	  break;
	case BONUS_FUEL:
	  bip->fuel += FUEL_BONUS_AMOUNT;
	  if (bip->fuel > FUEL_MAX_AMOUNT)
	    bip->fuel = FUEL_MAX_AMOUNT;
	  break;
	case BONUS_PARACHUTE:
#ifdef TODO
	  /* handle the parachute jump bonus */
#endif
	  plane_say(bip, "Eject!");
	  break;
	default:
	  return MSG_RET_UNKNOWN;
        }
      break;

    case MSG_SET_VARIANT:
      if (strcmp(msg_get_variant(msg),"red baron") == 0)
	set_model(bip,&red_baron);
      else if (strcmp(msg_get_variant(msg),"blue duke") == 0)
	set_model(bip,&blue_duke);
      else
	{
	  fprintf(stderr,"Unknown biplane model %s, quitting.\n",msg_get_variant(msg));
	  exit(EXIT_FAILURE);
	}      
      break;

    default:
      return MSG_RET_UNKNOWN;
      break;
    }
  return MSG_RET_ACK;
}

static void killbip(void *obj)
{
  struct biplane *bip = obj;
  if (bip->ai)
    bip->ai = obj_deref(bip->ai);
}

sprite_type_t biplane =
  {
    .object_type = {
      .name = "biplane",
      .message = message,
      .base_type = &mech_sprite_type,
      .kill = killbip
    },
    .default_group = &mech_group,
    .setup = setup,
    .create_sprite = create,
    .bg_collide = collide_world,
  };

REGISTER_SPRITE_TYPE(biplane)
