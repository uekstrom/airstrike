#include <stdlib.h>
#include <assert.h>
#include "xalloc.h"
#include "animation.h"
#include "display.h"

void animation_make_loop(animation_t *anim)
{
  animation_last_frame(anim)->next_frame = anim;
}

animation_t *animation_last_frame(animation_t *anim)
{
  animation_t *a = anim;
  while ((a->next_frame != 0) && (a->next_frame != anim)) 
    a = a->next_frame;
  return a;
}

animation_t *animation_frame_n(animation_t *anim, int frame_nr)
{
  while (frame_nr--)
    {
      if (!anim) return 0;
      anim = anim->next_frame;
    }
  return anim;
}

animation_t *animation_load(const char *path, int nr_pframes, 
			    int nr_tframes, unsigned int delay)
{
  int i,j;
  animation_t *first = 0,*f,*prev = 0,*base = 0;
  SDL_Surface *source;
  SDL_Rect r,cropr;
  Uint32 flags;

  if (display.hw_accel)
    flags = SDL_HWSURFACE;
  else
    flags = SDL_SWSURFACE|SDL_RLEACCEL;

  assert(nr_pframes > 0);
  assert(nr_tframes > 0);
  assert(delay > 0);
  source = load_surface(path);
  if (!source)
    {
      fprintf(stderr, "animation_load(): Error opening image file '%s'. Quitting!\n",path);
      exit(EXIT_FAILURE);
    }
  r.h = source->h;
  r.w = source->w/(nr_pframes*nr_tframes);
  r.y = 0;
  for (i=0;i<nr_tframes;i++)
    {
      f = xalloc(sizeof(animation_t));
      f->pframes = xalloc(sizeof(*(f->pframes))*nr_pframes);
      if (i==0)
	base = f;
      f->base_frame = base;
      f->trigger = 0;
      f->next_frame = 0;
      f->nr_pframes = nr_pframes;
      f->delay = delay;
      if (!first)
	first = f;
      if (prev)
	prev->next_frame = f;
      /* Convert to display format and create the individual frames*/
      for (j=0;j<nr_pframes;j++)
	{
	  r.x = (i + j*nr_tframes) * r.w;
	  cropr = r;
	  if (!display.use_alpha)
	    f->pframes[j] = image_get(source,&cropr,128,128,flags,1);
	  else
	    f->pframes[j] = image_get(source,&cropr,0,128,flags,1);
	}
      prev = f;
    }
  free_surface(source);
  return first;
}
