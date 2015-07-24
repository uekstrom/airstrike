/*
 * (w) 2004 by Eero Tamminen
 */

#include "engine.h"


/* how far from the starting point the ufo will go */
#define TURN_DIST 120

/* define directions */
#define UFO_LEFT 1
#define UFO_RIGHT 2

/* number of missiles per ufo */
#define UFO_MISSILES 3

typedef struct {
	mech_sprite_t sprite; /* the biplane is also a mech sprite */
	int missiles;
	int middle;
} ufo_t;


static animation_t *anim;


static int setup(void)
{
  anim = rc_load_animation("ufo");
  return 0;
}

static sprite_t *create(void)
{
	sprite_t *s;
	s = obj_alloc(sizeof(ufo_t), &ufo.object_type);
	s->flags |= SPRITE_CANROTATE;
	mech_defaults((mech_sprite_t *)s);
	((mech_sprite_t *)s)->rmass = 0.1; /* heavier than plane */
	((mech_sprite_t *)s)->gravity = 0; /* floats */
	((mech_sprite_t *)s)->air_resistance = 0.05;
	((mech_sprite_t *)s)->bounce = 0.2;
	((mech_sprite_t *)s)->rinmom = 0.2;
	((mech_sprite_t *)s)->air_resistance_ang = 0.1;
	s->animation = anim;
	s->anim_p = 0;
	s->state = UFO_RIGHT;

	/* start with some downwards movement */
	((mech_sprite_t *)s)->lin_impulse.y = 0.02;

	/* middle position for moving */
	((ufo_t *)s)->middle = current_level->width / 2;

	/* load ordnance and start firing */
	((ufo_t *)s)->missiles = UFO_MISSILES;
	sprite_alarm(100, s, msg_fire());

	return s;
}

static void update(sprite_t *s)
{
	mech_sprite_t *ms = (mech_sprite_t *)s;
	ufo_t *us = (ufo_t *)s;

	/* when to turn */
	if (s->pos.x + TURN_DIST < us->middle) {
		s->state = UFO_RIGHT;
	}
	if (s->pos.x - TURN_DIST > us->middle) {
		s->state = UFO_LEFT;
	}
	/* go to that direction */
	switch (s->state) {
	case UFO_LEFT:
		ms->lin_impulse.x = -0.1;
		break;
	case UFO_RIGHT:
		ms->lin_impulse.x = 0.1;
		break;
	}
	if(ms->angle < -0.5) {
	  ms->ang_impulse = 0.01;
	} else if(ms->angle > 0.5) {
	  ms->ang_impulse = -0.01;
	}
	mech_update(ms);
}

static enum msgret message(void *obj, msg_t msg)
{
	sprite_t *s = obj;
	ufo_t *us = obj;
	sprite_t *m;

	switch(msg.type) {
	case MSG_UPDATE:
		update(s);
		break;
	case MSG_FIRE:
		if (us->missiles-- <= 0) {
			/* fired all missiles -> disappear */
			sprite_kill(s);
			break;
		}
		/* fire a missile */
		m = sprite_create(&missile);
		sprite_set_pos(m, s->pos.x, s->pos.y + 20);
		m->vel.y = 0.1; /* slowly away from ufo */

		/* shoot missile towards target */
		if(global_aim.missile && obj_alive(global_aim.missile)) {
		  if(global_aim.missile->pos.x > m->pos.x) {
		    ((mech_sprite_t *)m)->angle = 0; /* right */
		    m->vel.x = 1.0;
		  } else {
		    ((mech_sprite_t *)m)->angle = 127; /* left */
		    m->vel.x = -1.0;
		  }
		  if(global_aim.missile->pos.y > m->pos.y) { /* slightly down */
		    if(((mech_sprite_t *)m)->angle == 127) {
		      ((mech_sprite_t *)m)->angle = 107;
		    } else {
		      ((mech_sprite_t *)m)->angle = 20;
		    }
	            m->vel.y = 1.0;
		  }
		}

		/* queue next missile */
		sprite_alarm(100, s, msg_fire());
		break;
	case MSG_KILL:
		sprite_kill(s);
		break;
	default:
		return MSG_RET_UNKNOWN;
		break;
	}
	return MSG_RET_ACK;
}

sprite_type_t ufo = {
	.object_type = {
		.name = "ufo",
		.message = message,
		.base_type = &mech_sprite_type
	},
	.default_group = &mech_group,
	.setup = setup,
	.create_sprite = create
};

REGISTER_SPRITE_TYPE(ufo)
