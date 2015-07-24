/* 
 * Level bonus parsing and handling
 * 
 * NOTES:
 * - This sends MSG_SET_BONUS message to objects that catch
 *   bonuses. Only human and AI player sprites should ACK
 *   that signal!
 * 
 * TODO:
 * - Make biplane bonuses work:
 *   - parachute jump
 * - Make AI bonuses work:
 *   - love
 * - Rework global aim to use tags?
 * 
 * (w) 2003-2005 by Eero Tamminen
 */

#include <string.h>
#include "engine.h"
#include "bonus.h"


/* towards which object certain objects should move */
struct global_aim_t global_aim  = {
	/* .bird =*/ NULL,
	/* .missile =*/ NULL,
	/* .zeppelin =*/ NULL
};


static int bonustypecount;		/* how many bonus types are allowed */
static unsigned long bonustypes;	/* flags which they are */
static sprite_type_t *bonus_sprite;	/* type of the bonus sprite */


void bonuses_clear(void)
{
	bonus_sprite = NULL;
	bonustypecount = 0;
	bonustypes = 0;
}

static int bonus_shell_fun(int argc, const char *argv[])
{
	struct {
		int id;
		const char *name;
	} names[ALL_BONUSES] = {
		/* These need to be in same order as in bonus.h */
		{ BONUS_AIRBALLOON, "AIRBALLOON" },
		{ BONUS_BIRD, "BIRD" },
		{ BONUS_BOMB, "BOMB" },
		{ BONUS_CLONE, "CLONE" },
		{ BONUS_CLOUD, "CLOUD" },
		{ BONUS_ENERGY, "ENERGY" },
		{ BONUS_FUEL, "FUEL" },
		{ BONUS_HIPPO, "HIPPO" },
		{ BONUS_LOVE, "LOVE" },
		{ BONUS_MISSILE, "MISSILE" },
		{ BONUS_PARACHUTE, "PARACHUTE" },
		{ BONUS_UFO, "UFO" },
		{ BONUS_ZEPPELIN, "ZEPPELIN" }
	};
	int i, arg;

	/* Make sure that bonuses are in right order */
	for (i = 0; i < ALL_BONUSES; i++) {
		assert(names[i].id == i);
	}
	/* Make sure that we don't have more bonuses than fit into flags... */
	assert(ALL_BONUSES < sizeof(bonustypes)*8);
	
	if (argc == 0) {
		shell_print("Usage: bonuses [bonus1 bonus2...]\n");
		shell_print("Enables given bonuses for the level.\n");
		shell_print("Available bonuses are:\n");
		for (i = 0; i < ALL_BONUSES; i++) {
			shell_print("- %s\n", names[i].name);
		}
		return 0;
	}

	bonuses_clear();
	for (arg = 0; arg < argc; arg++) {
		for (i = 0; i < ALL_BONUSES; i++) {
			if (strcmp(names[i].name, argv[arg]) == 0) {
				/* set the corresponding bonus flag */
				bonustypes |= (1 << names[i].id);
				bonustypecount++;
				break;
			}
		}
		if (i >= ALL_BONUSES) {
			return shell_error("Unknown bonus '%s'\n", argv[arg]);
		}
	}
	return 0;
}

void bonuses_setup(void)
{
	bonuses_clear();
	shell_defun("bonuses", bonus_shell_fun);
}

unsigned int get_random_bonus(void)
{
	unsigned int i, count, nth;
	
	if (!bonustypecount) {
		fprintf(stderr, "Bonus requested although no bonuses are specified for this level.\n");
		exit(EXIT_FAILURE);
	}

	/* get nth bonus type of the types allowed on this level */
	nth = get_random() % bonustypecount;
	count = 0;
	for (i = 0; i < ALL_BONUSES; i++) {
		/* is this type valid on this level? */
		if (bonustypes & (1<<i)) {
			if (count == nth) {
				return i;
				break;
			}
			count++;
		}
	}
	return 0;
}

extern sprite_type_t *get_bonus_sprite_type(void)
{
	sprite_type_t *type = bonus_sprite;
	/* return current bonus sprite type and NULL it */
	bonus_sprite = NULL;
	return type;
}


/* targeting functions */
static INLINE void clear_it(sprite_t **pos, sprite_t *it)
{
	if (*pos == it) {
		*pos = obj_deref(*pos);
	}
}

void global_aim_clear(sprite_t *it)
{
	clear_it(&(global_aim.bird), it);
	clear_it(&(global_aim.missile), it);
	clear_it(&(global_aim.zeppelin), it);
}

static int toggle_it(sprite_t **pos, sprite_t *it)
{
	if (*pos == it) {
		/* got the same bonus twice -> cancel */	  
		*pos = obj_deref(*pos);
		return 1;
	}
	
	if (!sprite_ask(it, MSG_ACCEPTS_BONUS)) {
		/* sprite doesn't accept bonus attentions */
		return 0;
	}
	
	if (*pos) {
		/* objects should change what they follow */
		*pos = obj_deref(*pos);
	}
	/* follow */
	*pos = obj_ref(it,&sprite_type);
	return 1;
}

int bonus_activate(unsigned int bonustype, struct sprite *it)
{
	sprite_t *s;
	int got_it = 0;	/* whether bonus was used */

	switch (bonustype) {
		
	case BONUS_AIRBALLOON:
		/* create next an airballoon */
		bonus_sprite = &airballoon;
		got_it = 1;
		break;

	case BONUS_BIRD:
		/* create next new bird and aim all birds at catcher */
		bonus_sprite = &bird;
		got_it = toggle_it(&(global_aim.bird), it);
		break;

	case BONUS_BOMB:
		/* ask catcher to fill up on bombs */
		if (sprite_msg(it, msg_set_bonus(BONUS_BOMB)) == MSG_RET_ACK) {
			got_it = 1;
		}
		break;
	
	case BONUS_CLONE:
		if (obj_has_type(it, &mech_sprite_type)) {
			/* create a clone of the catcher sprite */
			s = sprite_create((sprite_type_t *)obj_type(it));
			sprite_msg(it, msg_copy_ai(s));
			s->pos = it->pos;
			it->vel.x = 0.2;
			s->vel.y = -0.2;
			got_it = 1;
		}
		break;

	case BONUS_CLOUD:
		/* Create cloud at bonus catcher position */
		s = sprite_create(&cloud);
		sprite_set_pos(s, it->pos.x, it->pos.y);
		s->vel = vnormalize2(it->vel);
		got_it = 1;
		break;

	case BONUS_ENERGY:
		/* ask catcher to repair damage */
		if (sprite_msg(it, msg_set_bonus(BONUS_ENERGY)) == MSG_RET_ACK){
			got_it = 1;
		}
		break;
		
	case BONUS_FUEL:
		/* ask catcher to fill up on fuel */
		if (sprite_msg(it, msg_set_bonus(BONUS_FUEL)) == MSG_RET_ACK) {
			got_it = 1;
		}
		break;

	case BONUS_HIPPO:
		/* create next a hippo */
		bonus_sprite = &hippo;
		got_it = 1;
		break;

	case BONUS_LOVE:
		/* makes computer AIs non-aggressive */
		current_level->flags |= LEVEL_FLAG_LOVE;
		got_it = 1;
		break;

	case BONUS_MISSILE:
		/* create next missile and aim it at catcher */
		if (sprite_ask(it, MSG_ACCEPTS_BONUS)) {
			bonus_sprite = &missile;
			clear_it(&(global_aim.missile), it);
			got_it = toggle_it(&(global_aim.missile), it);
		}
		break;

	case BONUS_PARACHUTE:
		/* make pilot jump ship */
		if (sprite_msg(it, msg_set_bonus(BONUS_PARACHUTE)) == MSG_RET_ACK) {
			got_it = 1;
		}
		break;

	case BONUS_UFO:
		/* if player accepts bonuses, create next ufo and make
		 * its missiles aim at the bonus catcher
		 */
		if (sprite_ask(it, MSG_ACCEPTS_BONUS)) {
			bonus_sprite = &ufo;
			clear_it(&(global_aim.missile), it);
			got_it = toggle_it(&(global_aim.missile), it);
		}
		break;

	case BONUS_ZEPPELIN:
		/* if player accepts bonuses, make zeppelin follow catcher */
		if (sprite_ask(it, MSG_ACCEPTS_BONUS)) {
			got_it = toggle_it(&(global_aim.zeppelin), it);
		}
		break;

	default:
		shell_warning("Unknown bonus ID %d (valid ones are 0...%d)\n", bonustype, ALL_BONUSES);
		break;
	}
	return got_it;
}
