/*
 * List of event trigger and code for different game trigger types
 * 
 * Mandatory syntax:
 * 	 trigger TAGS TRIGGERTYPE [type options]
 * Special tags:
 * 	level-done, level-fail, level-restart
 * Optional keywords:
 *	tag, active, msg
 * 
 * Trigger types:
 * - player ALIVE DEAD
 * - spritecreate SPRITETYPE COUNT
 * - sprites SPRITETYPE MIN MAX
 * - spritekill SPRITETYPE COUNT
 * - tagkill SPRITETAG
 * - timer COUNT MIN-MS MAX-MS
 * - proximity X Y DISTANCE DIRECTION TAG
 * 
 * TODO:
 * - with proximity trigger we might got a lot of activations and therefore
 *   want to cache tagged trigger/generator IDs in tags_activate()
 *   (triggers & generators objects don't change within level)
 * - new trigger: aicontrol AI TAG
 *   (requires API for requesting which AI tagged sprite has)
 * 
 * (w) 2005 for Airstrike by Eero Tamminen
 */

#include <string.h>
#include "engine.h"

/* ------------- trigger type ------------------ */

/* trigger bitflags */
enum {
	TRIGGER_ACTIVE = 1
};
#define	TRIGGER_DEACTIVATE(trigger)	((trigger)->flags &= ~TRIGGER_ACTIVE)
#define	TRIGGER_ACTIVATE(trigger)	((trigger)->flags |= TRIGGER_ACTIVE)
#define	TRIGGER_IS_ACTIVE(trigger)	((trigger)->flags & TRIGGER_ACTIVE)


typedef struct trigger {
	object_t object;
	unsigned int flags;
	const char **tags;
	char *msg;
} trigger_t;

object_type_t trigger_type = {
	.name = "trigger"
};

/* ------------ handle tags --------- */

static void trigger_tags_activate(trigger_t *trigger)
{
	const char *tag, **tags = trigger->tags;
	object_t *obj;
	obj_id_t id;

	fprintf(stderr, "Triggering objects tagged with:\n");
	while ((tag = *tags)) {
		fprintf(stderr, "- %s\n", tag);

		/* check special tags */
		if (strcmp(tag, "level-done") == 0) {
			level_state_set(LEVEL_STATE_DONE, trigger->msg);
		} else if (strcmp(tag, "level-fail") == 0) {
			level_state_set(LEVEL_STATE_FAIL, trigger->msg);
		} else if (strcmp(tag, "level-restart") == 0) {
			level_state_set(LEVEL_STATE_RESTART, trigger->msg);
		} else {
			id = objdict_lookup(trigger_tags, tag);
			if (!id) {
				shell_warning("no object with tag '%s' for the trigger to activate.\n",
					    tag);
			} else {
				/* can't know what objects are being
				 * invoked, so need to use NULL
				 */
				obj = obj_lock(id, NULL);
				obj_message(obj, msg_activate());
				obj_unlock(obj);
			}
		}
		tags++;
	}
}

/* copies the given string and splits it at ',' chars.
 * returns a string pointer array pointing to the split strings.
 * caller is responsible to free the array.
 */
static const char **trigger_parse_tags(const char *str)
{
	char *tags;
	const char **tag_ptr, *start;
	int len, count;
	
	len = 0;
	count = 1;
	start = str;
	while (*str) {
		if (*str == ',') {
			count++;
		}
		len++;
		str++;
	}

	tag_ptr = xalloc(sizeof(*tag_ptr) * (count+1) + sizeof(*tags) * len + 1);
	if (!tag_ptr) {
		return NULL;
	}
	tags = (char *)(tag_ptr + count + 1);

	count = 0;
	tag_ptr[count] = tags;
	for (str = start; *str; str++) {
		if (*str == ',') {
			*tags++ = '\0';
			tag_ptr[++count] = tags;
		} else {
			*tags++ = *str;
		}
	}
	tag_ptr[++count] = NULL;
	*tags = '\0';
#ifdef OBJ_DEBUG
	{
		const char **t = tag_ptr;
		printf ("Trigger tags:\n");
		while (*t) {
			printf ("- %s\n", *t);
			t++;
		}
	}
#endif
	return tag_ptr;
}

static void trigger_tags_free(const char **tags)
{
	free(tags);
}

static void trigger_kill(void *obj)
{
	trigger_t *t = obj;
	if (t->tags) {
		trigger_tags_free(t->tags);
		t->tags = NULL;
	}
	if (t->msg) {
		free(t->msg);
		t->msg = NULL;
	}
}

/* ----------- player trigger ------------------- */

object_type_t trigger_player_type;

typedef struct trigger_player {
	trigger_t trigger;
	int alive;
	int dead;
} trigger_player_t;

static trigger_t *trigger_player_create(const char *alive, const char *dead)
{
	trigger_player_t *t;
	t = obj_alloc(sizeof(trigger_player_t), &trigger_player_type);
	sscanf(alive, "%u", &(t->alive));
	sscanf(dead, "%u", &(t->dead));
	objlist_insert(trigger_group, t);
	return (trigger_t*)t;
}

static enum msgret trigger_player_message(void *obj, msg_t msg)
{
	trigger_player_t *p = obj;
	trigger_t *t = obj;
	int alive, dead;
	
	switch (msg.type) {
	case MSG_DEACTIVATE:
		TRIGGER_DEACTIVATE(t);
		break;
	case MSG_ACTIVATE:
		TRIGGER_ACTIVATE(t);
		break;
	case MSG_UPDATE:
		if (!TRIGGER_IS_ACTIVE(t)) {
			break;
		}
		player_live_count(&alive, &dead);
		if (alive <= p->alive || dead >= p->dead) {
			trigger_tags_activate(t);
			TRIGGER_DEACTIVATE(t);
		}
		break;
	default:
		return MSG_RET_UNKNOWN;
	}
	return MSG_RET_ACK;
}

object_type_t trigger_player_type = {
	.name = "trigger_player",
	.base_type = &trigger_type,
	.message = &trigger_player_message,
	.kill = &trigger_kill
};

/* ----------- sprite trigger ------------------- */

enum {
	TRIGGER_SPRITE_CREATED,
	TRIGGER_SPRITE_ALIVE,
	TRIGGER_SPRITE_KILLED
};

object_type_t trigger_sprite_type;

typedef struct trigger_sprite {
	trigger_t trigger;
	sprite_type_t *sprite_type;
	unsigned int type;
	unsigned int count;
	unsigned int min, max;  /* needed only by alive counts */
} trigger_sprite_t;

static trigger_t *trigger_sprite_create(int type, const char *sprite_type_name, const char *min, const char *max)
{
	trigger_sprite_t *t;
	sprite_type_t *stype = get_sprite_type(sprite_type_name);
	if (!stype) {
		shell_error("unknown sprite type %s\n",
			    sprite_type_name);
		return NULL;
	}
	t = obj_alloc(sizeof(trigger_sprite_t), &trigger_sprite_type);
	t->sprite_type = stype;
	t->type = type;

	switch(type) {
	case TRIGGER_SPRITE_CREATED:
		sscanf(min, "%u", &(t->count));
		break;
	case TRIGGER_SPRITE_ALIVE:
		sscanf(min, "%u", &(t->min));
		sscanf(max, "%u", &(t->max));
		assert(min < max);
		break;
	case TRIGGER_SPRITE_KILLED:
		sscanf(min, "%u", &(t->count));
		break;
	default:
		shell_error("Unknown sprite trigger type %d\n", type);
		return NULL;
	}
	objlist_insert(trigger_group, t);
	return (trigger_t*)t;
}

static enum msgret trigger_sprite_message(void *obj, msg_t msg)
{
	object_type_t *otype;
	trigger_sprite_t *p = obj;
	trigger_t *t = obj;
	unsigned int created, alive, killed;
	
	switch (msg.type) {
	case MSG_DEACTIVATE:
		TRIGGER_DEACTIVATE(t);
		break;
	case MSG_ACTIVATE:
		TRIGGER_ACTIVATE(t);
		break;
	case MSG_UPDATE:
		if (!TRIGGER_IS_ACTIVE(t)) {
			break;
		}
		otype = &(p->sprite_type->object_type);
		switch(p->type) {
		case TRIGGER_SPRITE_CREATED:
			created = obj_type_count_created(otype);
			if (created < p->count) {
				/* no action yet */
				return MSG_RET_ACK;
			}
			break;
		case TRIGGER_SPRITE_ALIVE:
			alive = obj_type_count_alive(otype);
			if (alive >= p->min && alive <= p->max) {
				/* within range, no action yet */
				return MSG_RET_ACK;
			}
			break;
		case TRIGGER_SPRITE_KILLED:
			killed = obj_type_count_freed(otype);
			if (killed < p->count) {
				/* no action yet */
				return MSG_RET_ACK;
			}
			break;
		}
		/* do the action and deactivate */
		trigger_tags_activate(t);
		TRIGGER_DEACTIVATE(t);
		break;
	default:
		return MSG_RET_UNKNOWN;
	}
	return MSG_RET_ACK;
}

object_type_t trigger_sprite_type = {
	.name = "trigger_sprite",
	.base_type = &trigger_type,
	.message = &trigger_sprite_message,
	.kill = &trigger_kill
};

/* ----------- tagkill trigger ------------------- */

object_type_t trigger_tagkill_type;

typedef struct trigger_tagkill {
	trigger_t trigger;
	char *tag;
} trigger_tagkill_t;

static trigger_t *trigger_tagkill_create(const char *tag)
{
	trigger_tagkill_t *t;
	t = obj_alloc(sizeof(*t), &trigger_tagkill_type);
	assert(tag && *tag);
	t->tag = xstrdup(tag);
	objlist_insert(trigger_group, t);
	return (trigger_t*)t;
}

static enum msgret trigger_tagkill_message(void *obj, msg_t msg)
{
	trigger_tagkill_t *p = obj;
	trigger_t *t = obj;
	
	switch (msg.type) {
	case MSG_DEACTIVATE:
		TRIGGER_DEACTIVATE(t);
		break;
	case MSG_ACTIVATE:
		TRIGGER_ACTIVATE(t);
		break;
	case MSG_UPDATE:
		if (!TRIGGER_IS_ACTIVE(t)) {
			break;
		}
		/* sprite tag not (anymore) found, trigger + deactivate */
		if (!objdict_lookup(sprite_tags, p->tag)) {
#ifdef OBJ_DEBUG
			fprintf(stderr, "trigger_tagkill_message(): tag '%s' not found, activating tagged trigger objects\n",p->tag);
#endif
			trigger_tags_activate(t);
			TRIGGER_DEACTIVATE(t);
		}
		break;
	default:
		return MSG_RET_UNKNOWN;
	}
	return MSG_RET_ACK;
}

static void trigger_tag_kill(void *obj)
{
	trigger_tagkill_t *t = obj;
	free(t->tag);
	t->tag = NULL;
	trigger_kill(obj);
}

object_type_t trigger_tagkill_type = {
	.name = "trigger_tagkill",
	.base_type = &trigger_type,
	.message = &trigger_tagkill_message,
	.kill = &trigger_tag_kill
};

/* ----------- proximity trigger ------------------- */

object_type_t trigger_proximity_type;

enum {
	PROXIMITY_AROUND,
	PROXIMITY_ABOVE,
	PROXIMITY_BELOW,
	PROXIMITY_LEFT,
	PROXIMITY_RIGHT
};

typedef struct trigger_proximity {
	trigger_t trigger;
	int x;
	int y;
	int r2; /* distance squared */
	int dir;
	char *tag;
} trigger_proximity_t;

static trigger_t *trigger_proximity_create(const char *x, const char  *y, const char *r, const char *dir, const char *tag)
{
	int xx, yy, rr;
	trigger_proximity_t *t;
	t = obj_alloc(sizeof(trigger_proximity_t), &trigger_proximity_type);

	assert(*dir);
	switch(*dir) {
	case 'O':
	case 'o':
		t->dir = PROXIMITY_AROUND;
		break;
	case 'A':
	case 'a':
		t->dir = PROXIMITY_ABOVE;
		break;
	case 'B':
	case 'b':
		t->dir = PROXIMITY_BELOW;
		break;
	case 'L':
	case 'l':
		t->dir = PROXIMITY_LEFT;
		break;
	case 'R':
	case 'r':
		t->dir = PROXIMITY_RIGHT;
		break;
	default:
		return NULL;
	}
	sscanf(x, "%d", &(xx));
	sscanf(y, "%d", &(yy));
	sscanf(r, "%d", &(rr));
	assert(xx >= 0 && yy >= 0);
	assert(rr >  0 && rr < 600);
	assert(tag && *tag);
	t->x = xx;
	t->y = yy;
	t->r2 = rr * rr;
	t->tag = xstrdup(tag);
	
	objlist_insert(trigger_group, t);
	return (trigger_t*)t;
}

static enum msgret trigger_proximity_message(void *obj, msg_t msg)
{
	trigger_proximity_t *p = obj;
	trigger_t *t = obj;
	obj_id_t tag_id;
	sprite_t *s;
	int dx, dy, r2;
	
	switch (msg.type) {
	case MSG_DEACTIVATE:
		TRIGGER_DEACTIVATE(t);
		break;
	case MSG_ACTIVATE:
		TRIGGER_ACTIVATE(t);
		break;
	case MSG_UPDATE:
		if (!TRIGGER_IS_ACTIVE(t)) {
			break;
		}
		/* check where the sprite is */
		tag_id = objdict_lookup(sprite_tags, p->tag);
		if (!tag_id) {
#ifdef OBJ_DEBUG
			fprintf(stderr, "trigger_proximity_message(): tag '%s' not found\n",p->tag);
#endif
			break;
		}
		s = (sprite_t*)obj_lock(tag_id, &sprite_type);
		if (!s) {
#ifdef OBJ_DEBUG
			fprintf(stderr, "trigger_proximity_message(): sprite with id '%lu' not found\n", tag_id);
#endif
			break;
			
		}
		/* distance squared */
		dx = (p->x - s->pos.x);
		dy = (p->y - s->pos.y);
		r2 = dx*dx + dy*dy;
		obj_unlock(s);

		if (r2 < p->r2) {
			int act;
			switch (p->dir) {
			case PROXIMITY_AROUND:
				act = 1;
				break;
			case PROXIMITY_ABOVE:
				act = (dy > 0);
				break;
			case PROXIMITY_BELOW:
				act = (dy < 0);
				break;
			case PROXIMITY_LEFT:
				act = (dy > 0);
				break;
			case PROXIMITY_RIGHT:
				act = (dy < 0);
				break;
			default:
				exit(-1);
			}
			if (act) {
				trigger_tags_activate(t);
			}
		}
		break;
	default:
		return MSG_RET_UNKNOWN;
	}
	return MSG_RET_ACK;
}

static void trigger_proximity_kill(void *obj)
{
	trigger_proximity_t *p = obj;
	free(p->tag);
	p->tag = NULL;
	trigger_kill(obj);
}

object_type_t trigger_proximity_type = {
	.name = "trigger_proximity",
	.base_type = &trigger_type,
	.message = &trigger_proximity_message,
	.kill = &trigger_proximity_kill
};

/* ----------- timer trigger ------------------- */

object_type_t trigger_timer_type;

typedef struct trigger_timer {
	trigger_t trigger;
	unsigned int idx;
	unsigned int count;
	unsigned int min_time; /* 1/10 secs */
	unsigned int variance;
} trigger_timer_t;

static trigger_t *trigger_timer_create(const char *count, const char *min_time, const char *max_time)
{
	unsigned int min, max;
	trigger_timer_t *t;
	t = obj_alloc(sizeof(trigger_timer_t), &trigger_timer_type);
	sscanf(count, "%u", &(t->count));
	sscanf(min_time, "%u", &(min));
	sscanf(max_time, "%u", &(max));
	assert(t->count > 0 && max >= min);
	t->variance = max - min;
	t->min_time = min;
	objlist_insert(trigger_group, t);
	return (trigger_t*)t;
}

static enum msgret trigger_timer_message(void *obj, msg_t msg)
{
	trigger_timer_t *p = obj;
	trigger_t *t = obj;
	unsigned int timeout;
	
	switch (msg.type) {
	case MSG_DEACTIVATE:
		TRIGGER_DEACTIVATE(t);
		break;
	case MSG_FIRE:
		trigger_tags_activate(t);
		if (++(p->idx) >= p->count) {
			TRIGGER_DEACTIVATE(t);
		}
		if (!TRIGGER_IS_ACTIVE(t)) {
			break;
		}
		/* fall through... */
	case MSG_ACTIVATE:
		TRIGGER_ACTIVATE(t);
		/* next activation */
		timeout = p->min_time;
		if (p->variance) {
			timeout += get_random() % p->variance;
		}
		obj_alarm(timeout, t, msg_fire());
		/* increase count */
		break;
	default:
		return MSG_RET_UNKNOWN;
	}
	return MSG_RET_ACK;
}

object_type_t trigger_timer_type = {
	.name = "trigger_timer",
	.base_type = &trigger_type,
	.message = &trigger_timer_message,
	.kill = &trigger_kill
};

/* --------- shell and list handling --------------- */

static int trigger_shell_fun(int argc, const char *argv[])
{
	const char *type;
	if (argc == 0) {
		shell_print("Usage: trigger TAGS EVENTTYPE <event options>\n");
		shell_print("Create a new trigger to objects with given TAGS for EVENTTYPE.\n"
			    "'level-done', 'level-fail' and 'level-restart' are special tags\n"
			    "for ending/restarting the level.\n\n");
		
		shell_print("Possible trigger event types and their options are:\n");
		shell_print("- player ALIVE DEAD\n"
			    "  Triggers when only ALIVE players are alive or at least DEAD players are dead.\n");
		shell_print("- spritecreate SPRITETYPE COUNT\n"
			    "  Triggers when COUNT SPRITETYPEs created.\n");
		shell_print("- sprites SPRITETYPE MIN MAX\n"
			    "  Triggers when count of SPRITETYPEs not within MIN/MAX range.\n");
		shell_print("- spritekill SPRITETYPE COUNT\n"
			    "  Triggers when COUNT SPRITETYPEs killed.\n");
		shell_print("- tagkill SPRITETAG\n"
			    "  Triggers when a sprite with the TAG is killed.\n");
		shell_print("- timer COUNT MIN-MS MAX-MS\n"
			    "  Triggers COUNT times at random intervals between MIN and MAX 1/10 secs.\n");
		shell_print("- proximity X Y DISTANCE DIRECTION TAG\n"
			    "  Triggers when sprite with TAG comes within DISTANCE pixels from X Y\n"
			    "  in the DIRECTION O|A|B|R|L (arOund, Above, Below, Left, Right).\n");
		shell_print("Triggering activates the tagged generator/trigger and then deactivates\n"
			    "itself except for the 'proximity' and 'timer' triggers.  Former doesn't\n"
			    " deactivate and latter does deactivation only after COUNT triggerings.\n\n");
		
		shell_print("Following additional keywords can be used:\n");
		shell_print(" - tag (tag for the trigger itself)\n");
		shell_print(" - active (whether trigger is active, by default it is)\n");
		shell_print(" - msg (explanation for triggering, useful with 'level-*' targets)\n\n");

		shell_print("Examples:\n");
		shell_print("  sprite airballoon 200 200 vx=1 tag=balloon\n");
		shell_print("  sprite zeppelin 400 300 vx=1 tag=zepp\n");
		shell_print("  generator singleton fuel 400 200 9 gentag=fuelgen timeout=5 active=0\n");
		shell_print("  generator multi bird 400 200 5 gentag=birdgen rx=100 ry=100 vy=-2 active=0\n");
		shell_print("  trigger birdgen,fuelgen tagkill zepp\n");
		shell_print("  trigger birdgen proximity 600 0 200 B balloon\n");
		shell_print("  trigger level-done spritekill bird 5 msg='All birds destroyed!'\n");
		shell_print("  trigger level-fail sprites airballoon 1 1 msg='Airballoon destroyed'\n");
		shell_print("(i.e. kill first zeppelin, then birds, do not kill airballoon.)\n");
		return 0;
	}
	
	type = argv[1];
	if (argc > 2 && *(argv[0]) && *type) {
		const char **tags, *tag, *msg;
		trigger_t *trigger;
		int i;
		
		/* tags for the objects trigger activates */
		tags = trigger_parse_tags(argv[0]);
		if (!tags) {
			shell_error("error in parsing the tags in '%s'.\n",
				    argv[0]);
			return -1;
		}
		
		if (strcmp(type, "player") == 0) {
			if (argc < 4) {
				shell_error("player trigger needs 2 arguments.\n");
				return -1;
			}
			trigger = trigger_player_create(argv[2], argv[3]);
		} else if (strcmp(type, "spritecreate") == 0) {
			if (argc < 4) {
				shell_error("spritecreate trigger needs 2 arguments.\n");
				return -1;
			}
			trigger = trigger_sprite_create(TRIGGER_SPRITE_CREATED, argv[2], argv[3], 0);
		} else if (strcmp(type, "sprites") == 0) {
			if (argc < 5) {
				shell_error("sprites trigger needs 3 arguments.\n");
				return -1;
			}
			trigger = trigger_sprite_create(TRIGGER_SPRITE_ALIVE, argv[2], argv[3], argv[4]);
		} else if (strcmp(type, "spritekill") == 0) {
			if (argc < 4) {
				shell_error("spritekill trigger needs 2 arguments.\n");
				return -1;
			}
			trigger = trigger_sprite_create(TRIGGER_SPRITE_KILLED, argv[2], argv[3], 0);
		} else if (strcmp(type, "tagkill") == 0) {
			if (argc < 3) {
				shell_error("player trigger needs an argument.\n");
				return -1;
			}
			trigger = trigger_tagkill_create(argv[2]);
		} else if (strcmp(type, "timer") == 0) {
			if (argc < 5) {
				shell_error("timer trigger needs 3 arguments.\n");
				return -1;
			}
			trigger = trigger_timer_create(argv[2], argv[3], argv[4]);
		} else if (strcmp(type, "proximity") == 0) {
			if (argc < 7) {
				shell_error("proximity trigger needs 5 arguments.\n");
				return -1;
			}
			trigger = trigger_proximity_create(argv[2], argv[3], argv[4], argv[5], argv[6]);
		} else {
			shell_error("unknown trigger type %s\n", type);
			trigger_tags_free(tags);
			return -1;
		}
		if (!trigger) {
			return -1;
		}
		trigger->tags = tags;
		
		/* tag for the trigger itself */
		if (getarg_str(argv, "tag", &tag)) {
			if (objdict_lookup(trigger_tags, tag)) {
				shell_warning("trigger tag '%s' already defined.\n",
					    tag);
			} else {
				objdict_define(trigger_tags, tag, obj_id(trigger));
			}
		}
		if (getarg_str(argv, "msg", &msg)) {
			trigger->msg = xstrdup(msg);
		}
		if (getarg_int(argv, "active", &i) && !i) {
			TRIGGER_DEACTIVATE(trigger);
		} else {
			/* by default trigger starts active,
			 * need to do it with message for the
			 * timer trigger.
			 */
			obj_message(trigger, msg_activate());
		}
		return 0;
	}
	/* error */
	shell_error("Usage: trigger TAGS EVENTTYPE <event options>\n");
	return -1;
}

void triggers_setup(void)
{
	shell_defun("trigger", trigger_shell_fun);
	obj_type_init(&trigger_type);
	/* trigger types */
	obj_type_init(&trigger_player_type);
	obj_type_init(&trigger_sprite_type);
	obj_type_init(&trigger_tagkill_type);
	obj_type_init(&trigger_timer_type);
	obj_type_init(&trigger_proximity_type);
}

void triggers_update(objlist_t *group)
{
	int i;
	for (i = 0; i < group->size; i++) {
		obj_message(group->items[i], msg_update());
	}
}
