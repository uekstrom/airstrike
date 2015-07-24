/* 
 * Sprite engine Copyright (C) 2002-2004 Ulf Ekström
 * This code is released under the GPL
 */

#ifndef SPRITE_H
#define SPRITE_H

#include "core.h"
#include "maths.h"
#include "list.h"

extern object_type_t sprite_type;

struct sprite;

typedef struct sprite_type 
{
  object_type_t object_type;
  objlist_t **default_group; /* We need a double pointer since the group is created at runtime */
  int (*setup)(void);   /* Returns 0 on success */
  struct sprite *(*create_sprite)(void);
  /* this_sprite is guaranteed to have this type.
     x and y gives a point of collision in global coordinates. */
  void (*collide)(struct sprite *this_sprite, 
		  struct sprite *other_sprite, int x, int y);
  void (*bg_collide)(struct sprite *this_sprite, int x, int y);
  int setup_ok; /* 0: not yet setup, 1: ok. We cannot continue if it
		   fails, so no need to handle that case */
} sprite_type_t;

extern const char *sprite_flags_char;

enum sprite_flags
  {
    SPRITE_PAUSED = 1, /* animation is paused, movement is still allowed */
    SPRITE_ACTIVE = 2, /* For sprites that sometimes become active,
			i.e. after some time.  This bit can then be
			set by an alarm. */
    SPRITE_MECH = 4, /* Sprite is really a mech_sprite */
    SPRITE_CANROTATE = 8, /* Is a mech sprite && can rotate */
    SPRITE_INBG = 16, /* If the sprite can be inside the background */
    SPRITE_IMMOBILE = 32, /* should never be moved due to collision etc */
    SPRITE_AISUPPORT = 64,
  };

typedef struct sprite
{
  object_t object;
  coresprite_t base; /* contains x, y and current image */
  unsigned int state; /* User defined. */
  unsigned int flags; /* set of 'sprite_flags' */
  animation_t *animation;
  int anim_p; /* Animation parameter (angle etc) */
  vector_t pos;
  vector_t vel;

  /* 'private' variables */
  unsigned int t_off;
} sprite_t;

int bg_setup(void);
int bg_load(const char *filename);

/* sprite functions */

/* Setup sprite_type. Must call before creating sprites. */
void sprite_setup(void); 

sprite_t *sprite_create(sprite_type_t *type);
void sprite_kill(void *s);

enum msgret sprite_msg(sprite_t *s, msg_t msg);
/* Sends the message in the first frame after 'delay' 1/10 of a second */
void sprite_alarm(unsigned int delay, sprite_t *target, msg_t msg);
void sprite_set_animation(sprite_t *s, animation_t *anim);
void sprite_set_pos(sprite_t *s, float x, float y);
/* Make sure base.image reflects anim_p */
void sprite_update_image(sprite_t *s);

/* Return the change in overlap area with a change in anim_p
   for s1 (wrt. s2). (A(p+1) - A(p-1))/2
 */
int sprite_pgrad(sprite_t *s1, coresprite_t *s2);

/* Top left corner of the sprite image/mask. */
static INLINE int sprite_ax(sprite_t *s)
{
  return s->base.x - s->animation->pframes[s->anim_p]->xoff;
}

static INLINE int sprite_ay(sprite_t *s)
{
  return s->base.y - s->animation->pframes[s->anim_p]->yoff;
}

/* Moves s so that it (approximately) does not intersect the
  background. The normal is stored in n, and the original number of
  overlapping pixels is returned.  If the gradient is 0 then -nr
  pixels is returned. For most objects the overlap is not completely
  removed, due to object shape. */
int sprite_remove_from_bg(sprite_t *s, float n[]);
/* move sprite (dx,dy) at a time until it collides with background,
   or maxstep is reached. Return 0 is ground was found, else 1 */
int sprite_gravitate(sprite_t *s, int dx, int dy, int maxstep);

/* sprite group functions */

/* 'foreach'-type functions: */ 
void sprite_group_killall(objlist_t *group);
void sprite_group_move(objlist_t *group);
void sprite_group_pos_update(objlist_t *group);
void sprite_group_animate(objlist_t *group, unsigned int ms);
void sprite_group_msg(objlist_t *group, msg_t msg);
void sprite_group_update(objlist_t *group);
/* if f == 0 then the types own functions are used to handle
   the collisions */
void sprite_group_coll(objlist_t *group,
		       void (*f)(struct sprite *this_sprite, 
				 struct sprite *other_sprite, 
				 int x, int y));
void sprite_group_coll2(objlist_t *group1,
			objlist_t *group2,
			void (*f)(struct sprite *this_sprite, 
				  struct sprite *other_sprite, 
				  int x, int y));
void sprite_group_bg_coll(objlist_t *group,
			  void (*f)(struct sprite *sprite,
				    int x, int y));

/* Call f with data for each sprite at (x,y) in group */
void sprite_group_point_coll(objlist_t *group,
			     int point_x,int point_y,
			     void (*f)(struct sprite *sprite, 
				       int x, int y, void *data),
			     void *data);

/* Call trigger for each sprite colliding with mask at (x,y) */
int sprite_group_mask_coll(objlist_t *group, bitmask_t *mask,
			   int x, int y,
			   void (*f)(sprite_t *s, int x, int y, void *data),
			   void *data);

void sprite_group_draw(objlist_t *group);

/* other global sprite functions */

void sprite_start_frame(void); /* Call at start of frame */
unsigned int sprite_end_frame(void); /* Returns the number of ms slept */


#endif
