#ifndef LEVEL_H
#define LEVEL_H

#include "engine.h"

enum {
  LEVEL_STATE_INPROGRESS,
  LEVEL_STATE_DONE,
  LEVEL_STATE_FAIL,
  LEVEL_STATE_SKIP,
  LEVEL_STATE_RESTART
};

enum {
  LEVEL_FLAG_LOVE = 1	/* AIs should be non-aggressive */
};

struct level
{
  const char *name;
  const char *description;
  const char *path; /* Path of the level data directory */

  /* set by other parts of the engine */
  unsigned int flags;
  unsigned int state;
  char *reason;		/* why level has that state */

  float gravity;
  float bounce;
  float ground_friction; /* number >= 0, telling how slippery the ground is */
  float soft_impact_level;
  float soft_impact_damage;
  float hard_impact_level;
  float hard_impact_damage;

  /* These members are set automatically when the level is loaded */
  int width, height; /* Set from the image size */ 
  struct image *bg_image; /* the mask of this image is loaded from mask_path */
  coresprite_t bg_sprite; /* dummy sprite to use in collision detection with bg */

  /* Called after the images have been loaded, just before the game
     starts. Return 0 on success. */
  int (*init)(struct level *level); 
  /* Called each frame. Returns 0 as long as the game should go on. */
  int (*update)(struct level *level);
  /* Called when leaving this level. bg_image etc does not need to be 
   free'd here, it's done automatically. */
  int (*free)(struct level *level);
};

extern struct level *current_level;

void level_setup(void);
void level_load(const char *path); /* path from under levels/ */
void level_unload(void); /* Unloads current level, if there is one */

void level_state_set(unsigned int state, const char *reason);
unsigned int level_state_get(const char **reason);

#endif
