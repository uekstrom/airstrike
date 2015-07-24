/* 
 * A clumsy dragon
 * 
 * (w) 2003 by Eero Tamminen
 */
#include "engine.h"

/* dragon states */
#define DRACO_GLIDE 0
#define DRACO_UP 1
#define DRACO_DOWN 2
#define DRACO_RIGHT 3

static animation_t *anim;

static void flap(void *s)
{
	mech_sprite_t *ms = (mech_sprite_t *)s;
	ms->lin_impulse.y -= 0.5;
}

static int setup(void)
{
	anim = rc_load_animation("draco");
	animation_frame_n(anim,5)->trigger = flap;
	return 0;
}

static sprite_t *create(void)
{
	sprite_t *s;
	s = obj_alloc(sizeof(mech_sprite_t), &draco.object_type);
	mech_defaults((mech_sprite_t *)s);
	((mech_sprite_t *)s)->rmass = 0.2;
	((mech_sprite_t *)s)->gravity = 0.1;
	s->flags |= SPRITE_PAUSED;
	s->state = DRACO_GLIDE;
	s->animation = anim;
	return s;
}

static void update(sprite_t *s)
{
	mech_sprite_t *ms = (mech_sprite_t *)s;
	float x, y;
	
	if (s->vel.x < 0 && s->state == DRACO_GLIDE) {
		/* if going into wrong direction, activate
		 * forward flapping for a while 
		 */
		s->state = DRACO_RIGHT;
		s->flags &= ~SPRITE_PAUSED;
		sprite_alarm(40,s,msg_deactivate());
	}
	x = 0;
	y = 0;
	switch (s->state) {
	case DRACO_UP:
		/* flapping when not paused takes care of this:
		 * y = -0.03;
		 */
		break;
	case DRACO_DOWN:
		y = 0.02;
		break;
	case DRACO_RIGHT:
		x = 0.02;
		break;
	default:
		break;
	}
	ms->lin_impulse.x += x;
	ms->lin_impulse.y += y;
	mech_update(ms);
}

static void collide(struct sprite *s, 
		    struct sprite *other_sprite, 
		    int x, int y)
{
	if (s->state == DRACO_GLIDE) {
		/* going down */
		if (s->vel.y > -1) {
			s->state = DRACO_UP;
			sprite_alarm(30,s,msg_deactivate());
		} else {
			/* going up */
			if (s->vel.y < 0) {
				s->state = DRACO_DOWN;
				sprite_alarm(20,s,msg_deactivate());
			}
			/* otherwise just flap */
		}
		s->flags &= ~SPRITE_PAUSED;
	}
}

static enum msgret message(void *obj, msg_t msg)
{
	sprite_t *s = obj;

	switch(msg.type) {
	case MSG_UPDATE:
		update(s);
		break;
	case MSG_DEACTIVATE:
		s->flags |= SPRITE_PAUSED;
		s->state = DRACO_GLIDE;
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

sprite_type_t draco = {
	.object_type = {
		.name = "draco",
		.message = message,
		.base_type = &mech_sprite_type
	},
	.default_group = &mech_group,
	.setup = setup,
	.create_sprite = create,
	.collide = collide
};

REGISTER_SPRITE_TYPE(draco)
