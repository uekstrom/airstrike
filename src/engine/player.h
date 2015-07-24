#ifndef PLAYER_H
#define PLAYER_H

#define MAX_PLAYERS	4

/* called only when game is initialized */
void player_setup(void);
/* called by the command line parser */
int player_set_new(const char *ai_type_name, const char *player_name);

/* called by UI code to show player's names */
const char *player_get_name(int player_idx);

/* sets current player as alive and advances current
 * player to next one. Should be called only once for
 * each player while in the same level
 */
int player_set_current_alive(void);

/* sets the player with given index as dead
 * (index returned by player_set_current_alive())
 */
void player_set_dead(int idx);

/* returns in args number of players died and still living, on the current level */
void player_live_count(int *alive, int *dead);

/* clean all player (level specific) state (= set all dead) */
void players_clear(void);

/* called by sprite generator creating players
 * returns object ID for the player AI
 */
obj_id_t player_set_sprite_ai(int player_idx, sprite_t *s);

#endif /* PLAYER_H */
