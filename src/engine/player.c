/*
 * Player related functionality
 */
#include <string.h>
#include "engine.h"

#define PLAYER_ALIVE 1
#define PLAYER_DEAD  0

typedef struct {
	const char *ai_type;
	const char *name;
	int alive;
} player_t;

/* number of user set player names and AIs, per game */
static int players_in_game;
static player_t player[MAX_PLAYERS];

/* current player, same as the number of player generators, per level */
static int players_in_level;
static int players_alive;

int player_set_current_alive(void)
{
	int idx = players_in_level;
	assert(idx < MAX_PLAYERS);
	player[players_in_level++].alive = PLAYER_ALIVE;
	players_alive++;
	return idx;
}

void player_set_dead(int idx)
{
	assert(player[idx].alive == PLAYER_ALIVE);
	player[idx].alive = PLAYER_DEAD;
	players_alive--;
}

void player_live_count(int *alive, int *dead)
{
	*alive = players_alive;
	*dead = players_in_level - players_alive;
}

void players_clear(void)
{
	while (players_in_level > 0) {
		player[--players_in_level].alive = PLAYER_DEAD;
	}
	players_alive = 0;
}


const char *player_get_name(int idx)
{
	assert(idx < MAX_PLAYERS);
	return player[idx].name;
}

int player_set_new(const char *ai_type_name, const char *name)
{
	if (players_in_game >= MAX_PLAYERS) {
		return shell_error("Maximum number of players (%d) exceeded\n",
				   MAX_PLAYERS);
	}
	if (!ai_get_func(ai_type_name)) {
		/* invalid AI name */
		return -1;
	}
	player[players_in_game].ai_type = ai_type_name;
	player[players_in_game].name = name;
	players_in_game++;
	shell_print("Setting player %d (%s) as type '%s'\n",
		    players_in_game, name, ai_type_name);
	return 0;
}

obj_id_t player_set_sprite_ai(int idx, sprite_t *s)
{
	assert(s);
	assert(idx < MAX_PLAYERS);
#ifdef OBJ_DEBUG      
	printf("set_player_sprite_ai(): Assigned '%s' AI to player %d ('%s')\n",
	       player[idx].ai_type, idx, player[idx].name);
#endif
	return ai_set_sprite_ai(s, player[idx].ai_type);
}

void player_setup(void)
{
	int i;
	/* check that this is not called too late or twice */
	assert(!player[0].ai_type);

	/* setup default player AIs and names */
	player[0].ai_type = AI_DEFAULT_USER;
	player[0].name = "Player";
	for (i = 1; i < MAX_PLAYERS; i++) {
		player[i].ai_type = AI_DEFAULT_DUMMY;
		player[i].name = "Computer";
	}
}
