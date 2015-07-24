#include "engine.h"

static animation_t *anim;

static int setup(void)
{
  anim = rc_load_animation("hangar");
  return 0;
}

static sprite_t *create(void)
{
	sprite_t *s;
	s = obj_alloc(sizeof(mech_sprite_t), &hangar.object_type);
	s->flags |= SPRITE_IMMOBILE|SPRITE_INBG;
	mech_defaults((mech_sprite_t *)s);
	((mech_sprite_t *)s)->rmass = 0.000000001;
	((mech_sprite_t *)s)->gravity = 0;
	s->animation = anim;
	return s;
}

static enum msgret message(void *obj, msg_t msg)
{
	mech_sprite_t *ms = obj;
	sprite_t *s = obj;

	switch(msg.type) {
	case MSG_UPDATE:
		s->anim_p = ms->damage/10;
		if (s->anim_p > 7) {
			s->anim_p = 7;
		}
		break;
	case MSG_SET_DAMAGE:
		ms->damage += msg_get_damage(msg);
		break;
	default:
		return MSG_RET_UNKNOWN;
		break;
	}
	return MSG_RET_ACK;
}

sprite_type_t hangar = {
	.object_type = {
		.name = "hangar",
		.message = message,
		.base_type = &mech_sprite_type
	},
	.default_group = &mech_group,
	.setup = setup,
	.create_sprite = create
};

REGISTER_SPRITE_TYPE(hangar)
