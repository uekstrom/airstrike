#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "display.h"

struct display display = 
  {
    .frame_time = 1000/FRAMERATE, /* duration of one frame in ms */
    .use_alpha = 1
  };

static int drawto = 0;
static int all_dirty[2] = {1,1};
static unsigned long int dirty_mask[2][32];
static SDL_Rect tile_rects[512]; /* 512 is the maximum number of rects
				    in a 32x32 tile array, as they are
				    merged in the x direction */
static int nr_tile_rects = 0;
static int bg_changed = 1;


int display_open(int w, int h)
{  
  const SDL_VideoInfo *info;
  atexit(SDL_Quit);
  info = SDL_GetVideoInfo();
  
  if (info) /* info is sometimes NULL due to some bug in SDL. */
    {
      if (display.use_alpha)
	{
	  if (info->blit_hw_A)
	    {
	      fprintf(stderr,"Hardware alpha blits available!\n");
	      display.hw_accel = 1;
	    }
	  else if (info->blit_hw_CC)
	    {
	      fprintf(stderr,"No hardware alpha blits, but the game can run accelerated if you turn off transparency.\n");
	      display.hw_accel = 0;
	    }
	  else
	    {
	      fprintf(stderr,"No hardware acceleration found, using software rendering.\n");
	      display.hw_accel = 0;
	    }      
	}
      else
	{
	  if (info->blit_hw_CC)
	    {
	      fprintf(stderr,"Hardware colorkey blits available!\n");
	      display.hw_accel = 1;
	    }
	  else	
	    {
	      fprintf(stderr,"No hardware acceleration found, using software rendering.\n");
	      display.hw_accel = 0;
	    }      
	}  
    }
  else
    {
      fprintf(stderr,"Can't determine if hardware accelerated graphics is available, will run using software rendering.\n");
      display.hw_accel = 0;
    }

  if (display.hw_accel)
    display.surface = SDL_SetVideoMode(w, h, 0, SDL_DOUBLEBUF);
  else
    display.surface = SDL_SetVideoMode(w, h, 0, SDL_SWSURFACE);

  bg_changed = 1;

  if (display.surface)
    {
      char cbuf[200];
      info = SDL_GetVideoInfo();
      SDL_VideoDriverName(cbuf,200);
      fprintf(stderr,"Video driver: %s\n",cbuf);
      return 0;
    }
  else
    {
      fprintf(stderr,"Error: could not open display, quitting.\n");
      exit(EXIT_FAILURE);
      return -1;
    }
}

void display_close(void)
{
  
}

void display_set_background(SDL_Surface *image)
{
  display.background = image;
  bg_changed = 1;
}

/* Mark rect r (screen coords) as dirty */
static void display_dirty_rect(SDL_Rect *r)
{
  if ((!all_dirty[drawto]) && (r->w > 0) && (r->h > 0) 
      && (r->x < display.surface->w)
      && (r->y < display.surface->h))
    {
      int xs,xe,ys,ye,i,j;
      xs = r->x / DIRTY_TILE_SIZE;
      ys = r->y / DIRTY_TILE_SIZE;  
      xe = (r->x + r->w - 1) / DIRTY_TILE_SIZE;
      ye = (r->y + r->h - 1) / DIRTY_TILE_SIZE;
      for (i=ys;i<=ye;i++)
	for (j = xs; j <= xe; j++)
	  dirty_mask[drawto][i] |= (unsigned long)1 << j;
    }
}

static INLINE void update_row(SDL_Surface *target, int row)
{
  SDL_Rect r;
  unsigned long int mask = dirty_mask[1 - drawto][row];

  r.y = row*DIRTY_TILE_SIZE;  
  if ((row + 1)*DIRTY_TILE_SIZE <= target->h)
    r.h = DIRTY_TILE_SIZE;
  else
    r.h = DIRTY_TILE_SIZE - ((row + 1)*DIRTY_TILE_SIZE - target->h);
  r.x = 0;
  r.w = 0;
  
  while (mask)
    {
      while (mask && ((mask & 1) == 0))
	{
	  r.x += DIRTY_TILE_SIZE;
	  mask /= 2;
	}
      while (mask && (mask & 1))
	{
	  r.w += DIRTY_TILE_SIZE;
	  mask /= 2;
	}
      tile_rects[nr_tile_rects++] = r;
      r.x += r.w;
      r.w = 0;
    }
  if (nr_tile_rects)
    {
      if (tile_rects[nr_tile_rects - 1].x + 
	  tile_rects[nr_tile_rects - 1].w > target->w)	  
      {
	tile_rects[nr_tile_rects - 1].w -= 
	  tile_rects[nr_tile_rects - 1].x + 
	  tile_rects[nr_tile_rects - 1].w - target->w;
      }
    }
}

static void process_events(void)
{
  SDL_Event event;
  while ( SDL_PollEvent(&event) >= 1 ) 
    {
      switch (event.type) 
	{
	case SDL_QUIT: 
	  exit(EXIT_SUCCESS);
	  break;
	default:
	  break;
	}
    }
}

void display_waitframe(void)
{
  int delay;
  Uint32 now;
  static Uint32 last_update = 0;  
  /* Keep framerate */
  now = SDL_GetTicks();
  delay = (int)(display.frame_time + last_update) - (int)now;
  if (delay > 0)
    {
      SDL_Delay((Uint32)delay);
      now = SDL_GetTicks();
      /*    printf("%i\n",display.frame_time + last_update - now);*/
    }
  last_update = now;
}

void display_update(void)
{
  display_waitframe();
  if (display.hw_accel)
    {
      SDL_Flip(display.surface);
    }
  else
    {
      int i,row;
     
      all_dirty[1 - drawto] |= all_dirty[drawto];
      
      if (all_dirty[1 - drawto])
	{
	  SDL_UpdateRect(display.surface,0,0,0,0);
	  all_dirty[1 - drawto] = 0;
	}
      else
	{
	  for (row = 0; row < 32; row++)
	    dirty_mask[1 - drawto][row] |= dirty_mask[drawto][row];
	  nr_tile_rects = 0;
	  for (i=0;i<=display.surface->h/DIRTY_TILE_SIZE;i++)
	    update_row(display.surface,i);
	  /* clip lower parts of bottom rects */
	  i = nr_tile_rects - 1;      
	  SDL_UpdateRects(display.surface,nr_tile_rects,tile_rects);
	}
      
      for (row = 0; row < 32; row++)
	dirty_mask[1 - drawto][row] = 0;
      
      drawto = 1 - drawto;
    }
  process_events();
}


void display_clear(void)
{
  if (display.hw_accel)
    {
      SDL_BlitSurface(display.background,0,display.surface,0);
    }
  else
    {
      if (bg_changed) /* Do we need to repaint the bg buffer? */
	{
	  SDL_BlitSurface(display.background,0,display.surface,0);
	  all_dirty[1 - drawto] = 1;
	  bg_changed = 0;
	}
      else if (all_dirty[1 - drawto]) /* Should we restore the whole bg, or just parts */
	{
	  SDL_BlitSurface(display.background,0,display.surface,0);
	}
      else /* Just the dirty rects then.. */
	{
	  SDL_Rect r,rb;
	  unsigned long int mask;
	  int row;
	  for (row = 0; row < 32; row++)
	    {
	      r.y = row*DIRTY_TILE_SIZE;
	      r.h = DIRTY_TILE_SIZE;
	      r.x = 0;
	      r.w = 0;
	      
	      mask = dirty_mask[1 - drawto][row];
	      
	      while (mask)
		{
		  while (mask && ((mask & 1) == 0))
		    {
		      r.x += DIRTY_TILE_SIZE;
		      mask /= 2;
		    }
		  while (mask && (mask & 1))
		    {
		      r.w += DIRTY_TILE_SIZE;
		      mask /= 2;
		    }
		  rb.x = r.x;
		  rb.y = r.y;
		  rb.w = r.w;
		  rb.h = r.h;
		  SDL_BlitSurface(display.background,&rb, display.surface,&r);
		  r.x += r.w;
		  r.w = 0;
		}
	    }
	}
    }
}

void display_image(const image_t *image, int x, int y)
{
  SDL_Rect r;
  r.x = x - image->xoff;
  r.y = y - image->yoff;
  r.w = image->img->w;
  r.h = image->img->h;
  SDL_BlitSurface(image->img,0,display.surface,&r);
  if (!display.hw_accel)
    display_dirty_rect(&r);
}
