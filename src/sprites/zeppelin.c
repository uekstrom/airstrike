/*
 * (w) 2003 by Eero Tamminen
 */

#include "engine.h"

/* how far from the edge the zeppelin will turn around */
#define TURN_DIST 120

/* zeppelin state */
#define GO_LEFT   1
#define	GO_RIGHT  2
#define WRECKED   3	/* or larger value */
#define KILLED    255	/* larger enough */

static animation_t *z_turn_lr;
static animation_t *z_turn_rl;
static animation_t *z_wreck;


static sprite_t *create(void)
{
	mech_sprite_t *ms = obj_alloc(sizeof(*ms),&zeppelin.object_type);
	sprite_t *s;
	s = (sprite_t *)ms;
	
	mech_defaults(ms);
	(ms)->rmass = 0.1; /* very heavy */
	(ms)->gravity = 0; /* floats */
	(ms)->air_resistance = 0.05; /* catches a lot of air */
	(ms)->bounce = 0.9; /* quite bouncy */
	sprite_set_pos(s,50,200);
	s->animation = animation_last_frame(z_turn_lr);
	s->flags |= SPRITE_PAUSED;
	s->state = GO_RIGHT;
	return s;
}

static void update(sprite_t *s)
{
	mech_sprite_t *ms = (mech_sprite_t *)s;
	
	if (ms->damage > 50) {
		if (s->state < WRECKED) {
			/* explode once & wreck it */
			create_effect(&explosion, vector(s->pos.x - 30, s->pos.y - 15));
			create_effect(&explosion, vector(s->pos.x + 25, s->pos.y - 6));
			create_effect(&explosion, vector(s->pos.x + 10, s->pos.y + 14));
			sprite_set_animation(s, z_wreck);
			s->state = WRECKED;
			ms->gravity = 1; /* drops */
			ms->air_resistance = 0;
		} else {
			/* explode it second time & remove */
			if (s->state > WRECKED + 8 && s->state != KILLED) 
			{
				create_effect(&explosion, vector(s->pos.x, s->pos.y + 20));
				sprite_alarm(20,s,msg_kill());
				s->state = KILLED;
			}
		}
		/* enable animation */
		s->flags &= ~SPRITE_PAUSED;
	} else {
	        global_aim.zeppelin = obj_validate(global_aim.zeppelin);
		if (global_aim.zeppelin) {
			/* follow another sprite */
			mech_follow((mech_sprite_t *)s,global_aim.zeppelin, 0,0.3);
		}
		/* goes back and forth */
		if (s->pos.x < TURN_DIST) {
			ms->lin_impulse.x = 0.1;
		}
		if (s->pos.x > current_level->width - TURN_DIST) {
			ms->lin_impulse.x = -0.1;
		}
		/* turn the zeppelin right way */
		if (s->state == GO_LEFT && s->vel.x > 0) {
			sprite_set_animation(s, z_turn_lr);
			s->flags &= ~SPRITE_PAUSED;
			s->state = GO_RIGHT;
		}
		if (s->state == GO_RIGHT && s->vel.x < 0) {
			sprite_set_animation(s, z_turn_rl);
			s->flags &= ~SPRITE_PAUSED;
			s->state = GO_LEFT;
		}
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
	case MSG_SET_DAMAGE:
		((mech_sprite_t *)s)->damage += msg_get_damage(msg);
		break;
	case MSG_KILL:
		create_effect(&explosion, s->pos);
		sprite_kill(s);
		// TODO: use a zeppelin generator
		//s = sprite_create(&zeppelin);
		break;
	default:
		return MSG_RET_UNKNOWN;
		break;
	}
	return MSG_RET_ACK;
}

static void wreck_trigger(void *s)
{
	/* the wreck ticks until it explodes... */
	((sprite_t *)s)->state++;
}

static void anim_pause(void *s)
{
	((sprite_t *)s)->flags |= SPRITE_PAUSED;
}

static int setup(void)
{
	struct animation *last;

	z_turn_lr = rc_load_animation("zeppelin_lr");
	z_turn_rl = rc_load_animation("zeppelin_rl");	
	z_wreck = rc_load_animation("zeppelin_wreck");
	
	animation_last_frame(z_turn_lr)->trigger = anim_pause;
	animation_last_frame(z_turn_rl)->trigger = anim_pause;

	z_wreck->delay = 1000; /* make first wreck frame quite long */
	
	/* loop the last wreck frame */
	last = animation_last_frame(z_wreck);
	last->trigger = wreck_trigger;
	last->next_frame = last;
	return 0;
}

sprite_type_t zeppelin = {
	.object_type = {
		.name = "zeppelin",
		.message = message,
		.base_type = &mech_sprite_type
	},
	.default_group = &mech_group,
	.setup = setup,
	.create_sprite = create
};

REGISTER_SPRITE_TYPE(zeppelin)
