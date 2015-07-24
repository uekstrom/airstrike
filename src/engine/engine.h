#ifndef H_ENGINE
#define H_ENGINE

#include "core.h"
#include "list.h"
#include "dict.h"
#include "sprite.h"
#include "mech.h"
#include "level.h"
#include "sprite_types.h"
#include "schedule.h"
#include "ui.h"
#include "convenience.h"
#include "generator.h"
#include "trigger.h"
#include "player.h"
#include "ai.h"
#include "bonus.h"
#include "msg_types.h"
#include "maths.h"
#include "random.h"
#include "sound_fx.h"
#include "shell.h"
#include "conf.h"
#include "resources.h"

extern objlist_t *ui_group;
extern objlist_t *mech_group;
extern objlist_t *bullet_group;
extern objlist_t *effects_group;
extern objlist_t *foreground_group;
extern objlist_t *generator_group;
extern objlist_t *trigger_group;

extern objdict_t *sprite_tags;
extern objdict_t *trigger_tags;

void engine_setup(const char *defaults_path);
void engine_shutdown(void);
void draw_frame(void); /* Draw background, sprites and ui */
void game_frame(void); /* Advance the game one tick */

#endif
