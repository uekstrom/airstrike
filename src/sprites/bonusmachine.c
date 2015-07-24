/* 
 * Bonus objects:
 * - bonus machine
 * - bonus balloons
 * - animation (ring)
 * - bonus objects
 *
 * Note that the order of the bonus frames need to match
 * the bonus names in bonus.h.
 * 
 * (w) 2003-2005 by Eero Tamminen & Ulf Ekstrom
 */
#include "engine.h"

/* ------------------------------------------------------------ */

static animation_t *machine;
static animation_t *machine_open;


static void shoot_balloon(void *sp)
{
	sprite_type_t *type;
	sprite_t *b, *s = (sprite_t *)sp;
	vector_t v = {0,1};

	type = get_bonus_sprite_type();
	if (type) {
		/* create a bonus object */
		b = sprite_create(type);
	} else {
		/* otherwise create a balloon containing a bonus */
		b = sprite_create(&balloon);
	}
	sprite_set_pos(b, s->pos.x, s->pos.y + 32);
	b->vel = v;
}

static int machine_setup(void)
{
	machine = rc_load_animation("bonus-machine");

	/* last frame shoots a bonus */
	animation_last_frame(machine)->trigger = shoot_balloon;

	/* show first frame until we get a signal */
	machine_open = machine->next_frame;
	machine->next_frame = machine;
	return 0;
}

static sprite_t *machine_create(void)
{
	sprite_t *s;
	s = obj_alloc(sizeof(mech_sprite_t), &bonusmachine.object_type);
	s->flags |= SPRITE_IMMOBILE|SPRITE_INBG;
	mech_defaults((mech_sprite_t *)s);
	((mech_sprite_t *)s)->rmass = 0.000000001;
	((mech_sprite_t *)s)->gravity = 0;
	sprite_alarm(100,s, msg_fire());
	s->animation = machine;
	return s;
}

static enum msgret machine_message(void *obj, msg_t msg)
{
	sprite_t *s = obj;

	switch(msg.type) {
	case MSG_FIRE:
		/* proceed with machine animation */
		sprite_set_animation(s, machine_open);
		sprite_alarm(300, s, msg_fire());
		break;
	default:
		return MSG_RET_UNKNOWN;
	}
	return MSG_RET_ACK;
}

sprite_type_t bonusmachine = {
	.object_type = {
		.name = "bonusmachine",
		.message = machine_message,
		.base_type = &mech_sprite_type
	},
	.default_group = &mech_group,
	.setup = machine_setup,
	.create_sprite = machine_create
};


/* ------------------------------------------------------------ */

enum {
	BALLOON_FLOAT,
	BALLOON_HIT,
	BALLOON_DEFLATE
};

static animation_t *float_anim;
static animation_t *hit_anim;
static animation_t *deflate_anim;

static void balloon_trigger(void *sp)
{
	sprite_t *s = (sprite_t *)sp;
	sprite_set_animation(s, float_anim);
	s->state = BALLOON_FLOAT;
}

static void balloon_killme(void *sp)
{
	sprite_t *b, *s = (sprite_t *)sp;
	b = sprite_create(&bonusring);
	sprite_set_pos(b,s->pos.x, s->pos.y);
	sprite_kill(s);
}

static int balloon_setup(void)
{
	float_anim = rc_load_animation("balloon-float");
	hit_anim = rc_load_animation("balloon-bounce");
	deflate_anim = rc_load_animation("balloon-deflate");
	animation_last_frame(hit_anim)->trigger = balloon_trigger;
	animation_last_frame(deflate_anim)->trigger = balloon_killme;
	return 0;
}

static sprite_t *balloon_create(void)
{
	sprite_t *s;
	s = obj_alloc(sizeof(mech_sprite_t), &balloon.object_type);
	mech_defaults((mech_sprite_t *)s);
	((mech_sprite_t *)s)->rmass = 3;
	((mech_sprite_t *)s)->gravity = 0.0;
	s->animation = float_anim;
	s->state = BALLOON_FLOAT;
	return s;
}

static void balloon_collide(
	struct sprite *this_sprite, 
	struct sprite *other_sprite, 
	int x, int y)
{
	if (this_sprite->state == BALLOON_FLOAT) {
		sprite_set_animation(this_sprite, hit_anim);
		this_sprite->state = BALLOON_HIT;
	}
}

static void balloon_update(sprite_t *s)
{
	mech_sprite_t *ms = (mech_sprite_t *)s;
	mech_boundary(ms);
	mech_update(ms);
	if ((ms->damage > 5) && (s->state != BALLOON_DEFLATE)) {
		s->state = BALLOON_DEFLATE;
		sprite_set_animation(s, deflate_anim);
	}
}

static enum msgret balloon_message(void *obj, msg_t msg)
{
	sprite_t *s = obj;
	mech_sprite_t *ms = obj;

	switch(msg.type) {
	case MSG_UPDATE:
		balloon_update(s);
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

sprite_type_t balloon = {
	.object_type = {
		.name = "balloon",
		.message = balloon_message,
		.base_type = &mech_sprite_type
	},
	.default_group = &mech_group,
	.setup = balloon_setup,
	.create_sprite = balloon_create,
	.collide = balloon_collide
};


/* ------------------------------------------------------------ */


static animation_t *ring;
static animation_t *activate;

static void ring_create_bonus(sprite_t *s)
{
	sprite_t *b;

	b = sprite_create(&bonus);
	sprite_set_pos(b, s->pos.x, s->pos.y);
	
	/* tell bonus what is the ring and when to kill that
	 * (and itself)
	 */
	sprite_msg(b, msg_set_target(s));
	sprite_alarm(120, b, msg_kill());
}

static int ring_setup(void)
{
	ring = rc_load_animation("bonus-ring");
	activate = rc_load_animation("bonus-catch");
	animation_last_frame(activate)->trigger = sprite_kill;
	return 0;
}

static sprite_t *ring_create(void)
{
	sprite_t *s;
	s = obj_alloc(sizeof(sprite_t), &bonusring.object_type);
	/* wait a sec before showing the bonus */
	sprite_alarm(10, s, msg_fire());
	s->animation = ring;
	return s;
}

static enum msgret ring_message(void *obj, msg_t msg)
{
	sprite_t *s = obj;

	switch(msg.type) {
	case MSG_FIRE:
		ring_create_bonus(s);
		break;
	case MSG_ACTIVATE:
		/* bonus activated */
		s->animation = activate;
		break;
	case MSG_KILL:
		/* bonus expired */
		sprite_kill(s);
		break;
	default:
		return MSG_RET_UNKNOWN;
	}
	return MSG_RET_ACK;
}

sprite_type_t bonusring = {
	.object_type = {
		.name = "bonusring",
		.message = ring_message,
		.base_type = &sprite_type
	},
	.default_group = &foreground_group,
	.setup = ring_setup,
	.create_sprite = ring_create
};


/* ------------------------------------------------------------ */


static animation_t *bonusframes[ALL_BONUSES];

typedef struct
{
  mech_sprite_t sprite; /* bonus is also a mech sprite */
  sprite_t *ring;	/* pointer to bonus ring */
  unsigned int bonus;
} bonus_sprite_t;


static int bonus_setup(void)
{
	int i;
	animation_t *anim;

	/* object is killed before it moves into another frame */
	anim = rc_load_animation("bonus");

	for (i = 0; i < ALL_BONUSES; i++) {
		bonusframes[i] = anim;
		anim = anim->next_frame;
	}
	return 0;
}

static sprite_t *bonus_create(void)
{
	bonus_sprite_t *bs;
	sprite_t *s;

	bs = obj_alloc(sizeof(bonus_sprite_t), &bonus.object_type);

	s = (sprite_t *)bs;
	bs->bonus = get_random_bonus();
	s->animation = bonusframes[bs->bonus];

	/* don't animate */
	s->flags |= SPRITE_PAUSED;
	return s;
}

static void bonus_free(void *s)
{
	bonus_sprite_t *bs = s;
	bs->ring = obj_validate(bs->ring);
	fprintf(stderr, "DEBUG: killing bonus with ring (%p)\n", bs->ring);
	if (bs->ring) {
		bs->ring = obj_deref(bs->ring);
	}
}

static void bonus_collide(struct sprite *bonus_sprite,
			  struct sprite *other_sprite, 
			  int x, int y)
{
	bonus_sprite_t *bs = (bonus_sprite_t *)bonus_sprite;

	if (bonus_activate(bs->bonus, other_sprite)) {
		/* catcher acknowledged, activate bonus sprite */
		sprite_msg(bonus_sprite, msg_activate());
	} else {
		/* just remove bonus sprite */
		sprite_msg(bonus_sprite, msg_kill());
	}
}

static enum msgret bonus_message(void *obj, msg_t msg)
{
	sprite_t *s = obj;
	bonus_sprite_t *bs = obj;
	
	switch(msg.type) {
	case MSG_ACTIVATE:
		bs->ring = obj_validate(bs->ring);
		/* bonus catched */
		if (bs->ring) {
			/* activate bonus ring removal */
			sprite_msg(bs->ring, msg_activate());
			bs->ring = obj_deref(bs->ring);
		}
		/* remove bonus */
		sprite_kill(s);
		break;
	case MSG_KILL:
		bs->ring = obj_validate(bs->ring);
		/* bonus expired */
		if (bs->ring) {
			/* remove bonus ring */
			sprite_msg(bs->ring, msg_kill());
			bs->ring = obj_deref(bs->ring);
		}
		/* remove bonus */
		sprite_kill(s);
		break;
	case MSG_SET_TARGET:
		assert(!bs->ring);
		/* get pointer to bonus ring */
		bs->ring = obj_ref(msg_get_target(msg), &bonusring.object_type);
		break;
	default:
		return MSG_RET_UNKNOWN;
	}
	return MSG_RET_ACK;
}

sprite_type_t bonus = {
	.object_type = {
		.name = "bonus",
		.kill = bonus_free,
		.message = bonus_message,
		.base_type = &sprite_type
	},
	.default_group = &bullet_group,
	.setup = bonus_setup,
	.create_sprite = bonus_create,
	.collide = bonus_collide
};


REGISTER_SPRITE_TYPE(bonusmachine)
REGISTER_SPRITE_TYPE(balloon)
REGISTER_SPRITE_TYPE(bonusring)
REGISTER_SPRITE_TYPE(bonus)
