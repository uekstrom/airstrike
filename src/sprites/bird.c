/*
 * (w) 2003-2005 by Eero Tamminen
 * 
 * TODO:
 * - add particle effect for bird collisions and death
 */
#include "engine.h"

#define DISTANCE	50	/* distance to keep from level border */


/* bird flags */
#define BIRD_LEAD	1	/* leads others */
#define BIRD_AVOID	2	/* avoids instead of following (for awhile) */

static animation_t *anim;
static obj_id_t follow = 0;

static int bird_setup(void)
{
	anim = rc_load_animation("bird");
	animation_make_loop(anim);
	return 0;
}

static sprite_t *bird_create(void)
{
	sprite_t *s;
	s = obj_alloc(sizeof(mech_sprite_t),&bird.object_type);
	mech_defaults((mech_sprite_t *)s);
	((mech_sprite_t *)s)->rmass = 20;	// very little mass
	((mech_sprite_t *)s)->gravity = 0.0;
	s->animation = anim;
	return s;
}

static void bird_kill(void *s)
{
	sprite_t *oldleader = obj_lock(follow,&sprite_type);
	/* let birds "vote" a new leader */
	if (oldleader) {
		oldleader->state &= ~BIRD_LEAD;
		obj_unlock(oldleader);
	}
	follow = 0;
}

/* follow_or_avoid():
 *   makes sprite 's' to follow sprite 'it' but avoid getting too close.
 * s -- sprite which follows
 * it -- sprite which is followd
 * min -- distance at which to avoid object
 * max -- distance at which to start again following
 * speed -- maximum following speed
 */
static void follow_or_avoid(sprite_t *s, sprite_t *it, int min, int max, int speed)
{
	float h, vx, vy;
	int x, y, h2;
	
	/* make "s" follow "it" */
	x = (it->pos.x - s->pos.x);
	y = (it->pos.y - s->pos.y);
	/* but don't get too close */
	h2 = x*x + y*y;
	if (s->state & BIRD_AVOID) {
		if (!x) {
			x++;
		}
		if (!y) {
			y++;
		}
		x = -x;
		y = -y;
		if (h2 > max*max) {
			/* follow again */
			s->state &= ~BIRD_AVOID;
		}
	} else {
		if (h2 < min*min) {
			/* avoid for a while */
			s->state |= BIRD_AVOID;
		}
	}
	
	/* limit to max speed? */
	if (h2 > speed*speed) {
		h = sqrt(h2);
		vx = x * speed / h;
		vy = y * speed / h;
	} else {
		vx = x;
		vy = y;
	}
	s->vel.x = vx;
	s->vel.y = vy;
}

static void avoid_edges(sprite_t *s)
{
	float x = 0, y = 0;
	/* keep within screen */
	if (s->pos.x < DISTANCE) {
		x += 0.05;
	}
	if (s->pos.x > current_level->width - DISTANCE) {
		x -= 0.05;
	}
	if (s->pos.y < DISTANCE) {
		y += 0.05;
	}
	if (s->pos.y > current_level->height - DISTANCE) {
		y -= 0.05;
	}
	((mech_sprite_t *)s)->lin_impulse.x = x;
	((mech_sprite_t *)s)->lin_impulse.y = y;
}

static void bird_update(sprite_t *s)
{
	sprite_t *it = NULL;

	if (!follow) {		
		/* make me first */
		s->state |= BIRD_LEAD;
	}
	if (s->state & BIRD_LEAD) {
		/* do we have global aim which is not a bird? */
		global_aim.bird = obj_validate(global_aim.bird);
		if (global_aim.bird &&
		    obj_type(global_aim.bird) != (object_type_t *)&bird) {
			/* follow that instead of it */
			follow_or_avoid(s, global_aim.bird, 4, 32, 3);
		}
		avoid_edges(s);
	} else {
	  it = obj_lock(follow,&sprite_type);
	  if (it)
	    {
	      follow_or_avoid(s, it, 6, 40, 2);	  
	      obj_unlock(it);
	    }
	}
	mech_update((mech_sprite_t *)s);
	
	/* next bird follows me */
	follow = obj_id(s);
}

static enum msgret bird_message(void *obj, msg_t msg)
{
	mech_sprite_t *ms = obj;
	sprite_t *s = obj;

	switch(msg.type) {
	case MSG_UPDATE:
		bird_update(s);
		break;
	case MSG_SET_DAMAGE:
		sound_effect(&sound_bird, s->pos.x, s->pos.y);
		ms->damage += msg_get_damage(msg);
		if (ms->damage > 3) {
			/* TODO: blood or feather particles */
			sprite_kill(s);
		}
		break;
	default:
		return MSG_RET_UNKNOWN;
		break;
	}
	return MSG_RET_ACK;
}

sprite_type_t bird = {
	.object_type = {
		.name = "bird",
		.message = bird_message,
		.kill = bird_kill,
		.base_type = &mech_sprite_type
	},
	.default_group = &mech_group,
	.setup = bird_setup,
	.create_sprite = bird_create
};

REGISTER_SPRITE_TYPE(bird)
