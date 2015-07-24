#ifndef ANIMATION_H
#define ANIMATION_H

#include "image.h"

/* 
   Animations are linked lists with a node for each timestep, but can
   also have a parameter at each time, for which an array is used
   (pframes).  For example the angle of the sprite can be the
   parameter
*/
typedef struct animation
{
  image_t **pframes;
  unsigned int delay;
  int nr_pframes;  
  struct animation *next_frame;
  struct animation *base_frame;
  /* The trigger is called with the sprite as argument when this frame
     is _entered_, if trigger is nonzero */
  void (*trigger)(void *s); 
} animation_t;

animation_t *animation_load(const char *path, int nr_pframes, 
			    int nr_tframes, unsigned int delay);
void animation_make_loop(animation_t *anim);
animation_t *animation_last_frame(animation_t *anim);
animation_t *animation_frame_n(animation_t *anim, int frame_nr);

#endif
