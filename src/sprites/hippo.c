/* 
 * Bouncing & rotating hippo
 * 
 * (w) 2003,2004 by Eero Tamminen
 */
#include "engine.h"

static animation_t *anim;

static int setup(void)
{
  anim = rc_load_animation("hippo");
  return 0;
}

static sprite_t *create(void)
{
	sprite_t *s;
	s = obj_alloc(sizeof(mech_sprite_t), &hippo.object_type);
	s->animation = anim;
	s->anim_p = 0;
	s->flags |= SPRITE_CANROTATE;
	mech_defaults((mech_sprite_t *)s);
	((mech_sprite_t *)s)->rmass = 6;  /* fairly light */
	((mech_sprite_t *)s)->rinmom = 4;
	((mech_sprite_t *)s)->air_resistance = 0.0001;
	((mech_sprite_t *)s)->gravity = 0.4;  /* and full of gas */
	((mech_sprite_t *)s)->bounce = 1.0;
	sprite_alarm(10, s, msg_activate());
	s->state = get_random() % 8;
	return s;
}

static void update(sprite_t *s)
{
	mech_sprite_t *ms = (mech_sprite_t*)s;
	if (s->state > 3) {
		ms->ang_impulse += s->state * 0.02;
	} else {
		ms->ang_impulse -= s->state * 0.02;
	}
	mech_update(ms);
}

static enum msgret message(void *obj, msg_t msg)
{
	sprite_t *s = obj;

	switch(msg.type) {
	case MSG_UPDATE:
		update(s);
		break;
	case MSG_ACTIVATE:
		/* randomize rotation direction */
		s->state = get_random() % 8;
		sprite_alarm(20, s, msg_activate());
		break;
	case MSG_OUTOFBOUNDS:
		fprintf(stderr, "Hippo: Out of screen (top or bottom)!\n");
		sprite_kill(s);
		break;
	default:
		return MSG_RET_UNKNOWN;
	}
	return MSG_RET_ACK;
}

sprite_type_t hippo = {
	.object_type = {
		.name = "hippo",
		.message = message,
		.base_type = &mech_sprite_type
	},
	.default_group = &mech_group,
	.setup = setup,
	.create_sprite = create
};

REGISTER_SPRITE_TYPE(hippo)
