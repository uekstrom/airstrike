/* 
 * (w) 2004 by Eero Tamminen
 */
#include "engine.h"

enum {
	WALK_LEFT,
	TURN_LEFT,
	TURN_RIGHT,
	WALK_RIGHT
};

static animation_t *left, *right, *turn;

static void walk_trigger(void *d)
{
	sprite_t *s = (sprite_t *)d;
	switch (s->state) {
	case TURN_LEFT:
		s->animation = left;
		s->state = WALK_LEFT;
		break;
	case TURN_RIGHT:
		s->animation = right;
		s->state = WALK_RIGHT;
		break;
	default:
		fprintf(stderr,"pingu:walk_trigger(): State is not turning!\n");
		exit(EXIT_FAILURE);
	}
}

static int setup(void)
{
  left = rc_load_animation("pingu_left");
  turn = rc_load_animation("pingu_turn");
  right = rc_load_animation("pingu_right");
  animation_last_frame(turn)->trigger = walk_trigger;
  return 0;
}

static sprite_t *create(void)
{
	mech_sprite_t *ms;
	ms = obj_alloc(sizeof(mech_sprite_t), &pingu.object_type);
	mech_defaults(ms);
	ms->rmass = 8;
	ms->gravity = 0.1;
	ms->bounce = 0.1;
	ms->sprite.state = WALK_RIGHT;
	ms->sprite.animation = right;
	return (sprite_t*)ms;
}

static enum msgret message(void *obj, msg_t msg)
{
	mech_sprite_t *ms = obj;
	sprite_t *s = obj;

	switch(msg.type) {
	case MSG_UPDATE:
		mech_update(ms);
		break;
	case MSG_SET_DAMAGE:

		/* TODO: create "puff" */
		switch (s->state) {
		case WALK_LEFT:
			s->animation = turn;
			s->state = TURN_RIGHT;
			ms->gravity *= -1;
			break;
		case WALK_RIGHT:
			s->animation = turn;
			s->state = TURN_LEFT;
			ms->gravity *= -1;
			break;
		}
		break;

	default:
		return MSG_RET_UNKNOWN;
		break;
	}
	return MSG_RET_ACK;
}

sprite_type_t pingu = {
	.object_type = {
		.name = "pingu",
		.message = message,
		.base_type = &mech_sprite_type
	},
	.default_group = &mech_group,
	.setup = setup,
	.create_sprite = create
};

REGISTER_SPRITE_TYPE(pingu)
