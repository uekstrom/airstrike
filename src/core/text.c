#include <assert.h>
#include <ctype.h>
#include "xalloc.h"
#include "text.h"
#include "image.h"
#include "display.h"

static int col_free(bitmask_t *mask, int col)
{
  int i;
  assert(col>=0&&col<mask->w);
  for (i=0;i<mask->h;i++)
    if (bitmask_getbit(mask,col,i))
      return 0;
  return 1;
}

font_t *font_load(const char *path, const char *chars, int kerning, int linesep)
{
  font_t *font = xcalloc(1,sizeof(*font));
  SDL_Surface *src;
  SDL_Rect r;
  image_t *tmpimg;

  src = load_surface(path);
  assert(src);
  tmpimg = image_get(src,NULL,0,30,SDL_SWSURFACE,0);
  font->char_h = src->h - 1 + linesep;
  font->char_d = 1 + kerning;
  r.y = 1;
  r.h = src->h - 1;
  r.x = 0;
  r.w = 0;

  while (*chars)	 
    {
      image_t *img;
      /* First skip blank columns */
      while (col_free(tmpimg->mask,r.x))
	{
	  r.x++;
	  if (r.x >= src->w)
	    {
	      int i;
	      fprintf(stderr,"font_load(): Could not find char '%c' in font file %s.\n",*chars,path);
	      fprintf(stderr,"Character widths:\n");
	      for (i=0;i<256;i++)
		{
		  if (font->chars[i])
		    printf("%c: %i\n",(char)i,font->chars[i]->img->w);
		}
	      BUG("Premature end of font image");
	    }
	}
      /* See how wide the next char is */
      r.w = 1;
      while (!col_free(tmpimg->mask,r.x+r.w))
	r.w++;
      if (r.w <= 0)
	fprintf(stderr,"font_load(): Warning, character '%c' is only %i pixels wide.\n",*chars,r.w);
      /* Cut out the image for the current character */
      img = image_get(src,&r,0,0,SDL_RLEACCEL|SDL_SWSURFACE,0);
      img->xoff = 0;
      img->yoff = 0;
      font->chars[(unsigned char)*chars] = img;
      r.x += r.w;
      chars++;
    }

  image_free(tmpimg);
  free_surface(src);
  return font;
}

void font_free(font_t *font)
{
  int i;
  assert(font);
  for (i = 0; i < 256; i++)
    if (font->chars[i])
      image_free(font->chars[i]);
  free(font);
}

void display_text(SDL_Rect *r,
		  font_t *font,
		  int x, int y, 
		  enum text_alignment xalign, 
		  enum text_alignment yalign,
		  char *s)
{
  int w = 0,h = font->char_h; 
  int rw = 0;
  int xstart,ystart, xp, yp;
  char *c = s;

  /* How big rect are we going to cover? */
  while (*c)
    {
      if (*c == '\n')
	{
	  if (rw > w)
	    w = rw;
	  rw = 0;
	  if (*(c+1))
	    h += font->char_h;
	}
      else
	{
	  if (font->chars[(unsigned char)*c])
	    rw += font->chars[(unsigned char)*c]->img->w + font->char_d;
	  else
	    rw += 5 + font->char_d;
	}
      c++;
    }
  if (rw > w)
    w = rw;

  /* Where to start drawing? */
  if (xalign == ALIGN_LEFT)
    xstart = x;
  else if (xalign == ALIGN_RIGHT)
    xstart = x - w;
  else
    xstart = x - w/2;

  if (yalign == ALIGN_TOP)
    ystart = y;
  else if (yalign == ALIGN_BOTTOM)
    ystart = y - h;
  else
    ystart = y - h/2;

  if (r)
    {
      r->x = xstart;
      r->y = ystart;
      r->w = w;
      r->h = h;
    }

  xp = xstart;
  yp = ystart;

  /* Now we can finally render */  
  for (c = s; *c != 0; c++)
    {
      if (*c == '\n')
	{
	  xp = xstart;
	  yp += font->char_h;
	}
      else if (font->chars[(unsigned char)*c])
	{
	  display_image(font->chars[(unsigned char)*c],xp,yp);
	  xp += font->chars[(unsigned char)*c]->img->w + font->char_d;
	}
      else
	{
	  xp += font->char_d + 5;
	}
    }
}

