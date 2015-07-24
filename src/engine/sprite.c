#include <stdlib.h>
#include <assert.h>
#include "sprite.h"
#include "level.h"
#include "schedule.h"
#include "msg_types.h"

const char *sprite_flags_char = "pamrbi";

sprite_t *sprite_create(sprite_type_t *type)
{
  sprite_t *s;
  if (!type->setup_ok)
    {
      /*      fprintf(stderr,"Loading sprite type '%s'\n",type->object_type.name);*/
      if (type->setup())
        return NULL;
      type->setup_ok = 1;
    }
  s = type->create_sprite();
  sprite_set_pos(s,0,0);
  sprite_update_image(s);
  if (*((sprite_type_t *)obj_type(s))->default_group)
    objlist_insert(*((sprite_type_t *)obj_type(s))->default_group,s);
  else
    BUG("No default group");
  return s;
}

void sprite_kill(void *s)
{
  obj_kill(s);
}

void sprite_set_animation(sprite_t *s, animation_t *anim)
{
  s->animation = anim;
  if (s->animation->trigger)
    s->animation->trigger(s);
}

void sprite_set_pos(struct sprite *s, float x, float y)
{
  s->base.x = x;
  s->base.y = y;
  s->pos.x = x;
  s->pos.y = y;
}

enum msgret sprite_msg(sprite_t *s, msg_t msg)
{
  object_type_t *type = obj_type(s);
  if (type->message)
    return type->message(s,msg);
  else
    return MSG_RET_UNKNOWN;
}

static void send_alarm(msg_t msg, void *sprite)
{
    sprite_msg(sprite,msg);
}

void sprite_alarm(unsigned int delay, sprite_t *target, msg_t msg)
{
  schedule(delay_to_frames(delay),SCHED_NORMAL,send_alarm,msg,target);
}

/* Frame checklist
 * Sprite time propagation dt to new frame:
 * 1) Move all sprites according to velocity
 * 2) Advance animation as of dt, execute callbacks as
 *    frames are entered. Do each sprite completely 
 *    before going to the next.
 * 3) Find collisions, execute callbacks when encountered
 * 4) Execute frame update functions (i.e. react to step 1..3).
 * 5) Clean up dead sprites
 * 6) Draw remaining sprites (may be combined w 5)
 */

static INLINE void sprite_move(sprite_t *s)
{
  s->pos.x += s->vel.x;
  s->pos.y += s->vel.y;
  s->base.x = (int)(s->pos.x + 0.5);
  s->base.y = (int)(s->pos.y + 0.5);
}

int sprite_pgrad(sprite_t *s1, coresprite_t *s2)
{
  int p,a;
  image_t *tmp;
  if (s1->animation->nr_pframes == 1)
    return 0;
  tmp = s1->base.image;
  p = s1->anim_p + 1;
  while (p >= s1->animation->nr_pframes)
    p -= s1->animation->nr_pframes;
  s1->base.image = s1->animation->pframes[p];
  a = coresprite_overlap_area(&s1->base,s2);
  p = s1->anim_p - 2;
  while (p < 0) 
    p += s1->animation->nr_pframes;  
  s1->base.image = s1->animation->pframes[p];
  a -= coresprite_overlap_area(&s1->base,s2);
  s1->base.image = tmp;
  return a;
}

void sprite_update_image(sprite_t *s)
{
  /* Set the correct image on the sprite */
  assert(s->anim_p < s->animation->nr_pframes);
  s->base.image = s->animation->pframes[s->anim_p];
  assert(s->base.image);
}

static void sprite_animate(sprite_t *s, unsigned int ticks)
{
  /* advance animation */
  if (!(s->flags & SPRITE_PAUSED))
    {
      s->t_off += ticks;
      while (s->t_off > s->animation->delay)
	{
	  s->t_off -= s->animation->delay;
	  /* Start anew if there is no next frame */
	  if (s->animation->next_frame)
	    s->animation = s->animation->next_frame;
	  else
	    s->animation = s->animation->base_frame;	 

	  if (s->animation->trigger)
	    s->animation->trigger(s);
	}
      sprite_update_image(s);
    }
}

void sprite_group_coll(objlist_t *group,
		       void (*f)(struct sprite *this_sprite, 
				 struct sprite *other_sprite, 
				 int x, int y))
{
  int i,j,x,y;
  sprite_t *s1,*s2;
  sprite_type_t *s1type,*s2type;

  if (f)
    {
      for (i = 0; i < group->size - 1; i++)
	{
	  s1 = group->items[i];
	  for (j = i + 1; j < group->size; j++)
	    {
	      s2 = group->items[j];
	      if (coresprite_overlap_pos(&s1->base, &s2->base, &x, &y))
		f(s1,s2,x,y);
	    }
	}
    }
  else
    {
      for (i = 0; i < group->size - 1; i++)
	{
	  s1 = group->items[i];
	  s1type = (sprite_type_t *)obj_type(s1);
	  for (j = i + 1; j < group->size; j++)
	    {
	      s2 = group->items[j];
	      if (coresprite_overlap_pos(&s1->base, &s2->base, &x, &y))
		{
		  s2type = (sprite_type_t *)obj_type(s2);
		  if (s1type->collide)
		    s1type->collide(s1,s2,x,y);
		  if (s2type->collide)
		    s2type->collide(s2,s1,x,y);
		}
	    }
	}
    }
}

void sprite_group_coll2(objlist_t *group1, 
			objlist_t *group2,
			void (*f)(struct sprite *this_sprite, 
				  struct sprite *other_sprite, 
				  int x, int y))
{
  int i,j,x,y;
  sprite_t *s1,*s2;
  sprite_type_t *s1type,*s2type;
  bitmask_t *b1;
  if (f)
    {
      for (i = 0; i < group1->size; i++)
	{
	  s1 = group1->items[i];
	  b1 = s1->animation->pframes[s1->anim_p]->mask;
	  for (j = 0; j < group2->size; j++)
	    {
	      s2 = group2->items[j];
	      if (bitmask_overlap_pos(b1,s2->animation->pframes[s2->anim_p]->mask,
				      sprite_ax(s2) - sprite_ax(s1),
				      sprite_ay(s2) - sprite_ay(s1),
				      &x, &y))
		{
		  x += sprite_ax(s1);
		  y += sprite_ay(s1);
		  f(s1,s2,x,y);
		}
	    }
	}
    }
  else
    {
      for (i = 0; i < group1->size; i++)
	{
	  s1 = group1->items[i];
	  s1type = (sprite_type_t *)obj_type(s1);
	  b1 = s1->animation->pframes[s1->anim_p]->mask;
	  for (j = 0; j < group2->size; j++)
	    {
	      s2 = group2->items[j];
	      if (bitmask_overlap_pos(b1,s2->animation->pframes[s2->anim_p]->mask,
				      sprite_ax(s2) - sprite_ax(s1),
				      sprite_ay(s2) - sprite_ay(s1),
				      &x, &y))
		{
		  x += sprite_ax(s1);
		  y += sprite_ay(s1);
		  s2type = (sprite_type_t *)obj_type(s2);
		  if (s1type->collide)
		    s1type->collide(s1,s2,x,y);
		  if (s2type->collide)
		    s2type->collide(s2,s1,x,y);
		}
	    }
	}
    }
}

void sprite_group_point_coll(objlist_t *group,
			     int point_x, int point_y,
			     void (*f)(struct sprite *sprite, 
				       int x, int y, void *data),
			     void *data)
{
  int i;
  for (i = 0; i < group->size; i++)
    if (coresprite_at_point(&(((sprite_t *)group->items[i])->base),point_x,point_y))
      f(group->items[i],point_x,point_y,data);
}


#if TODO /* what's this for? */
/* Call trigger for each sprite colliding with mask at (x,y) */
static int sprite_group_coresprite_coll(objlist_t *group,
				 coresprite_t *coresprite,
				 void (*f)(sprite_t *s, int x, int y, void *data),
				 void *data)
{
  int i,n = 0,xp,yp;
  sprite_t *s1;
  for (i = 0; i < group->size; i++)
    {
      s1 = group->items[i];
      if (coresprite_overlap_pos(&s1->base,coresprite,&xp, &yp))
	{
	  f(s1,xp,yp,data);
	  n++;
	}
    }
  return n;
}
#endif


void sprite_group_bg_coll(objlist_t *group,
			  void (*f)(struct sprite *sprite,
				    int x, int y))
{
  int i,n = 0,xp,yp;
  sprite_t *s1;
  sprite_type_t *s1type;
  coresprite_t *coresprite = &current_level->bg_sprite;
  for (i = 0; i < group->size; i++)
    {
      s1 = group->items[i];
      s1type = (sprite_type_t *)obj_type(s1);
      if (coresprite_overlap_pos(&s1->base,coresprite,&xp, &yp))
	{
	  if (f)
	    f(s1,xp,yp);
	  else if (s1type->bg_collide)
	      s1type->bg_collide(s1,xp,yp);
	  n++;
	}
    }
}

void sprite_group_move(objlist_t *group)
{
  int i;
  for (i=0; i < group->size; i++)
    sprite_move(group->items[i]);
}

void sprite_group_pos_update(objlist_t *group)
{
  int i;
  for (i=0; i < group->size; i++)
    {
      ((sprite_t *)group->items[i])->base.x = ((sprite_t *)group->items[i])->pos.x + 0.5;
      ((sprite_t *)group->items[i])->base.y = ((sprite_t *)group->items[i])->pos.y + 0.5;
    }
}

void sprite_group_animate(objlist_t *group, unsigned int delta)
{
  int i;
  for (i=0; i < group->size; i++)
    sprite_animate(group->items[i],delta);
}

void sprite_group_update(objlist_t *group)
{
  int i;
  for (i=0; i < group->size; i++)
    obj_message(group->items[i],msg_update());
}

void sprite_group_draw(objlist_t *group)
{
  int i;
  for (i=0; i < group->size; i++)
    display_coresprite(&(((sprite_t *)group->items[i])->base));
}

object_type_t sprite_type = 
  {
    .name = "sprite",    
  };

static int spriteinfo_shell_fun(int argc, const char *argv[])
{
  if (argc == 1)
    {
      sprite_t *s = obj_lock(objdict_lookup(sprite_tags,argv[0]),&sprite_type);
      if (s)
	{
	  int i;
	  shell_print("  Type = %s\n",obj_type(s)->name);
	  shell_print("  x = %g, y = %g\n",s->pos.x,s->pos.y);
	  shell_print("  v_x = %g, v_y = %g\n",s->vel.x,s->vel.y);
	  shell_print("  anim_p = %i\n",s->anim_p);
	  shell_print("  flags = ");
	  for (i=0;sprite_flags_char[i];i++)
	    {
	      if (s->flags & (1 << i))
		shell_print("%c",sprite_flags_char[i]);
	      else
		shell_print("-");	  
	    }
	  shell_print("\n");
	  obj_unlock(s);
	  return 0;
	}
      else
	{
	  shell_error("No such tag '%s'.\n",argv[0]);
	  return -1;
	}      
    }
  else
    {
      shell_print("Usage: spriteinfo TAG\n");
      shell_print("  Print info about sprite with TAG.\n");
      if (argc == 0)
	return 0;
      else
	return -1;
    }
}

void sprite_setup(void) 
{
  shell_defun("spriteinfo",spriteinfo_shell_fun);
  obj_type_init(&sprite_type);
}


