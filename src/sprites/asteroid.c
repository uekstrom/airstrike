#include "engine.h"

static animation_t *anim;

static int setup(void)
{
	anim = rc_load_animation("asteroid");
	animation_make_loop(anim);
	return 0;
}

static sprite_t *create(void)
{
	mech_sprite_t *ms = obj_alloc(sizeof(*ms), &asteroid.object_type);
	sprite_t *s = (sprite_t *)ms;
	mech_defaults(ms);
	ms->rmass = 0.0001;
	ms->gravity = 0.0;
	s->animation = anim;
	return s;
}

static enum msgret message(void *obj, msg_t msg)
{
	sprite_t *s = obj;
	switch(msg.type) {
	case MSG_UPDATE:
		mech_update((mech_sprite_t *)s);
		break;
	case MSG_SET_DAMAGE:
		((mech_sprite_t *)s)->damage += msg_get_damage(msg);
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

sprite_type_t asteroid =
{
	.object_type = {
		.name = "asteroid",
		.message = message,
		.base_type = &mech_sprite_type
	},
	.default_group = &mech_group,
	.setup = setup,
	.create_sprite = create,
};

REGISTER_SPRITE_TYPE(asteroid)
