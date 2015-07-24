/*
 * List of generators and code for different game object generator types
 * 
 * Mandatory syntax:
 *	generator TYPE SPRITETYPE X Y COUNT
 * Optional keywords: 
 *	gentag, objtag, ai, vx, vy, rx, ry, timeout, active
 * 
 * Generator types:
 * - player
 * - singleshot
 * - singleton
 * - multi
 * 
 * (w) 2005 for Airstrike by Eero Tamminen
 */

#include <string.h>
#include "engine.h"

/* generator bitflags */
enum {
	GENERATOR_ACTIVE = 1,
	GENERATOR_COUNTED = 2
};
#define	GENERATOR_ACTIVATE(g)	((g)->flags |= GENERATOR_ACTIVE)
#define	GENERATOR_DEACTIVATE(g)	((g)->flags &= ~GENERATOR_ACTIVE)
#define	GENERATOR_IS_ACTIVE(g)	((g)->flags & GENERATOR_ACTIVE)

#define	GENERATOR_LIMIT_COUNT(g)	((g)->flags |= GENERATOR_COUNTED)
#define	GENERATOR_UNLIMIT_COUNT(g)	((g)->flags &= ~GENERATOR_COUNTED)
#define	GENERATOR_IS_COUNT_LIMITED(g)	((g)->flags & GENERATOR_COUNTED)

/* ------------- generator type ------------------ */

typedef struct generator {
	object_t object;
	unsigned int flags;
	sprite_type_t *sprite_type;
	obj_id_t obj_id;
	char *tag;		/* object tag, not used for multi type */
	const char *ai;		/* object AI type name, not used for player type */
	vector_t pos;		/* object position */
	vector_t offset;	/* object position random offset */
	vector_t vel;		/* object velocity */
        unsigned int timer;	/* timer for timeouts */
	unsigned int timeout;	/* seconds between objects */
	unsigned int count;	/* type specific */
} generator_t;

object_type_t generator_type = {
	.name = "generator"
};

static void generator_kill(void *obj)
{
	generator_t *g = (generator_t *)obj;
	if (g->tag) {
		free(g->tag);
		g->tag = NULL;
	}
}

/* returns NULL if sprite creation timeout hasn't expired,
 * otherwise the correct sprite for the generator
 */
static sprite_t *create_generator_sprite(generator_t *g)
{
	int x, y;
	sprite_t *s;
	if (g->timeout && !rate_limit(g->timeout, &(g->timer))) {
		/* enforce timeout between sprite creations */
		return NULL;
	}
	s = sprite_create(g->sprite_type);
	x = g->pos.x + (frand() - 0.5) * g->offset.x;
	y = g->pos.y + (frand() - 0.5) * g->offset.y;
	sprite_set_pos(s, x, y);
	s->vel = g->vel;
	if (g->tag) {
		if (objdict_lookup(sprite_tags, g->tag)) {
			shell_warning("sprite tag '%s' already defined.\n", g->tag);
		} else {
			objdict_define(sprite_tags, g->tag, obj_id(s));
		}
	}
	return s;
}

/* ----------- player generator ------------------- */

object_type_t player_generator_type;

typedef struct player_generator {
	generator_t generator;
	int player;
} player_generator_t;

static generator_t *player_generator_create(sprite_type_t *stype)
{
	generator_t *g;
	g = obj_alloc(sizeof(player_generator_t), &player_generator_type);
	((player_generator_t*)g)->player = player_set_current_alive();
	g->sprite_type = stype;

	objlist_insert(generator_group, g);
	return g;
}

static enum msgret player_generator_message(void *obj, msg_t msg)
{
	player_generator_t *p = obj;
	generator_t *g = obj;
	sprite_t *s;
	ai_t *ai;
	
	switch (msg.type) {
	case MSG_DEACTIVATE:
		GENERATOR_DEACTIVATE(g);
		break;
	case MSG_ACTIVATE:
		GENERATOR_ACTIVATE(g);
		break;
	case MSG_UPDATE:
		if (!GENERATOR_IS_ACTIVE(g)) {
			break;
		}
		ai = obj_lock(g->obj_id, &ai_type);
		if (ai) {
			/* ai still valid, continue */
			obj_unlock(ai);
		} else {
			if (g->count <= 0 &&
			    GENERATOR_IS_COUNT_LIMITED(g)) {
				player_set_dead(p->player);
				/* generator has finished */
				GENERATOR_DEACTIVATE(g);
				break;
			}
			/* create new sprite and set it player AI */
			if (!(s = create_generator_sprite(g))) {
				break;
			}
			g->obj_id = player_set_sprite_ai(p->player, s);
			g->count--;
		}
		break;
	default:
		return MSG_RET_UNKNOWN;
	}
	return MSG_RET_ACK;
}

object_type_t player_generator_type = {
	.name = "player_generator",
	.base_type = &generator_type,
	.message = &player_generator_message,
	.kill = &generator_kill
};

/* ----------- singleshot generator ------------------- */

object_type_t singleshot_generator_type;

static generator_t *singleshot_generator_create(sprite_type_t *stype)
{
	generator_t *g;
	g = obj_alloc(sizeof(*g), &singleshot_generator_type);
	g->sprite_type = stype;

	objlist_insert(generator_group, g);
	return g;
}

static enum msgret singleshot_generator_message(void *obj, msg_t msg)
{
	generator_t *g = obj;
	sprite_t *s;
	
	switch (msg.type) {
	case MSG_ACTIVATE:
		if (g->count <= 0 &&
		    GENERATOR_IS_COUNT_LIMITED(g)) {
			break;
		}
		/* creates new sprite each time activated */
		if (!(s = create_generator_sprite(g))) {
			break;
		}
		if (g->ai) {
			/* ...and set its AI */
			ai_set_sprite_ai(s, g->ai);
		}
		/* at the bottom so that it works with the sprite
		 * creation rate limit
		 */
		g->count--;
		break;
	default:
		return MSG_RET_UNKNOWN;
	}
	return MSG_RET_ACK;
}

object_type_t singleshot_generator_type = {
	.name = "singleshot_generator",
	.base_type = &generator_type,
	.message = &singleshot_generator_message,
	.kill = &generator_kill
};

/* ----------- singleton generator ------------------- */

object_type_t singleton_generator_type;

static generator_t *singleton_generator_create(sprite_type_t *stype)
{
	generator_t *g;
	g = obj_alloc(sizeof(*g), &singleton_generator_type);
	g->sprite_type = stype;

	objlist_insert(generator_group, g);
	return g;
}

static enum msgret singleton_generator_message(void *obj, msg_t msg)
{
	generator_t *g = obj;
	sprite_t *s;
	void *old;
	
	switch (msg.type) {
	case MSG_DEACTIVATE:
		GENERATOR_DEACTIVATE(g);
		break;
	case MSG_ACTIVATE:
		GENERATOR_ACTIVATE(g);
		break;
	case MSG_UPDATE:
		if (!GENERATOR_IS_ACTIVE(g)) {
			break;
		}
		if (g->ai) {
			old = obj_lock(g->obj_id, &ai_type);
		} else {
			old = obj_lock(g->obj_id, &sprite_type);
		}
		if (old) {
			/* ai/sprite still valid */
			g->timer = frame_nr() + delay_to_frames(g->timeout);
			obj_unlock(old);
			break;
		}
		/* create new sprite... */
		if (!(s = create_generator_sprite(g))) {
			break;
		}
		if (g->ai) {
			/* ...and set&get its AI */
			g->obj_id = ai_set_sprite_ai(s, g->ai);
		} else {
			/* ...and get its ID */
			g->obj_id = obj_id(s);
		}
printf("Singleton: %d -> sprite %p with (AI) id %lx\n", g->count, s, g->obj_id);
		g->count--;
		if (g->count <= 0 &&
		    GENERATOR_IS_COUNT_LIMITED(g)) {
			/* generator has finished */
			GENERATOR_DEACTIVATE(g);
		}
		break;
	default:
		return MSG_RET_UNKNOWN;
	}
	return MSG_RET_ACK;
}

object_type_t singleton_generator_type = {
	.name = "singleton_generator",
	.base_type = &generator_type,
	.message = &singleton_generator_message,
	.kill = &generator_kill
};

/* ----------- multi generator ------------------- */

object_type_t multi_generator_type;

static generator_t *multi_generator_create(sprite_type_t *stype)
{
	generator_t *g;
	g = obj_alloc(sizeof(*g), &multi_generator_type);
	g->sprite_type = stype;

	objlist_insert(generator_group, g);
	return g;
}

static enum msgret multi_generator_message(void *obj, msg_t msg)
{
	generator_t *g = obj;
	sprite_t *s;
	
	switch (msg.type) {
	case MSG_DEACTIVATE:
		GENERATOR_DEACTIVATE(g);
		break;
	case MSG_ACTIVATE:
		GENERATOR_ACTIVATE(g);
		break;
	case MSG_UPDATE:
		if (!GENERATOR_IS_ACTIVE(g)) {
			break;
		}
		if (obj_type_count_alive(&(g->sprite_type->object_type)) < g->count) {
			/* create new sprite... */
			if (!(s = create_generator_sprite(g))) {
				break;
			}
			if (g->ai) {
				/* ...and set&get its AI */
				g->obj_id = ai_set_sprite_ai(s, g->ai);
			} else {
				/* ...and get its ID */
				g->obj_id = obj_id(s);
			}
		}
		break;
	default:
		return MSG_RET_UNKNOWN;
	}
	return MSG_RET_ACK;
}

object_type_t multi_generator_type = {
	.name = "multi_generator",
	.base_type = &generator_type,
	.message = &multi_generator_message,
	.kill = &generator_kill
};

/* --------- shell and list handling --------------- */


static generator_t *get_generator_with_sprite(const char *generator_type_name, const char *sprite_type_name)
{
	sprite_type_t *stype;
	shell_print("Creating generator '%s' with sprite type '%s'\n",
		    generator_type_name, sprite_type_name);	

	/* lookup target sprite */
	stype = get_sprite_type(sprite_type_name);
	if (!stype) {
		shell_error("unknown sprite type %s\n", sprite_type_name);
		return NULL;
	}
	
	/* set sprite to correct generator type */
	if (strcmp(generator_type_name, "player") == 0) {
		return player_generator_create(stype);
	}
	if (strcmp(generator_type_name, "singleshot") == 0) {
		return singleshot_generator_create(stype);
	}
	if (strcmp(generator_type_name, "singleton") == 0) {
		return singleton_generator_create(stype);
	}
	if (strcmp(generator_type_name, "multi") == 0) {
		return multi_generator_create(stype);
	}
	shell_error("No such generator '%s'.\n", generator_type_name);
	return NULL;
}

static int generator_shell_fun(int argc, const char *argv[])
{
	float x, y;
	int count;
	if (argc == 0) {
		shell_print("Usage: generator TYPE SPRITETYPE X Y COUNT\n");
		shell_print("Create a new generator at (X,Y) of TYPE for SPRITETYPE\n\n");

		shell_print("Possible generator types are:\n");
		shell_print("- player\n"
			    "  Generates player sprite COUNT times, after that marks player dead.\n");
		shell_print("- singleshot\n"
			    "  Generates sprite each time it's activated, at most once in\n"
			    "  timeout/10 seconds, and at most COUNT times.\n");
		shell_print("- singleton\n"
			    "  Generates sprite COUNT times, after previous instance died.\n");
		shell_print("- multi\n"
			    "  Keeps at least COUNT objects.\n");
		shell_print("COUNT of zero is understood as infinite.\n\n");

		shell_print("Following additional keywords can be used:\n");
		shell_print(" - gentag (tag for the generator)\n");
		shell_print(" - objtag (tag for the generated object)\n");
		shell_print(" - ai (AI to set for the generated objects)\n");
		shell_print(" - vx, vy (speed to give to the generated objects)\n");
		shell_print(" - rx, ry (randomized offset for the generated objects)\n");
		shell_print(" - timeout (timeout between object generation)\n");
		shell_print(" - active (whether generator is active, by default it is)\n\n");

		shell_print("Examples:\n");
		shell_print("  generator player biplane 400 200 3 gentag=gen1 objtag=player1 vx=4\n");
		shell_print("  generator singleton fuel 400 200 20 timeout=5 active=0\n");
		shell_print("  generator singleshot missile 400 500 timeout=20 active=0\n");
		shell_print("  generator multi bird 400 200 5 rx=100 ry=100 vy=-2\n");
		return 0;
	}
	
	if (argc >= 5 &&
	    sscanf(argv[2], "%f", &x) +
	    sscanf(argv[3], "%f", &y) +
	    sscanf(argv[4], "%d", &count) == 3) {
		const char *gentag, *objtag = NULL, *ai = NULL;
		generator_t *gen;
		float v;
		int i;
		
		if (getarg_str(argv, "ai", &ai) && strcmp(argv[0], "player") == 0) {
			return shell_error("'player' generator AIs cannot be overridden (with %s).\n", ai);
		}
		if (getarg_str(argv, "objtag", &objtag) && strcmp(argv[0], "multi") == 0) {
			return shell_error("'multi' generator object cannot be tagged (with %s).\n", objtag);
		}
		gen = get_generator_with_sprite(argv[0], argv[1]);
		if (!gen) {
			return -1;
		}
		gen->pos.x = x;
		gen->pos.y = y;
		gen->count = count;
		if (count) {
			GENERATOR_LIMIT_COUNT(gen);
		} else {
			GENERATOR_UNLIMIT_COUNT(gen);
		}
		gen->ai = ai;
		if (objtag) {
			gen->tag = xstrdup(objtag);
		}
		if (getarg_str(argv, "gentag", &gentag)) {
			if (objdict_lookup(trigger_tags, gentag)) {
				shell_warning("generator tag '%s' already defined.\n",
					    gentag);
			} else {
				objdict_define(trigger_tags, gentag, obj_id(gen));
			}
		}
		if (getarg_float(argv, "vx", &v)) {
			gen->vel.x = v;
		}
		if (getarg_float(argv, "vy", &v)) {
			gen->vel.y = v;
		}
		if (getarg_float(argv, "rx", &v)) {
			gen->offset.x = v;
		}
		if (getarg_float(argv, "ry", &v)) {
			gen->offset.y = v;
		}
		if (getarg_int(argv, "timeout", &i)) {
			gen->timeout = i;
		}
		if (getarg_int(argv, "active", &i) && !i) {
			GENERATOR_DEACTIVATE(gen);
		} else {
			/* by default generators are active */
			GENERATOR_ACTIVATE(gen);
		}
		return 0;
	}
	/* error */
	return -1;
}

void generators_setup(void)
{
	shell_defun("generator", generator_shell_fun);
	obj_type_init(&generator_type);
	/* generator types */
	obj_type_init(&player_generator_type);
	obj_type_init(&singleshot_generator_type);
	obj_type_init(&singleton_generator_type);
	obj_type_init(&multi_generator_type);
}

void generators_update(objlist_t *group)
{
	int i;
	for (i = 0; i < group->size; i++) {
		obj_message(group->items[i], msg_update());
	}
}
