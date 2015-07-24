#include <assert.h>
#include <SDL_image.h>
#include "xalloc.h"
#include "image.h"

/*=================================================
                Image Loading functions 
  =================================================*/

/* minimize rectangle r while not excluding any pixels
   with alpha > threshold */
static void autocrop(SDL_Surface *target, SDL_Rect *r, int threshold)
{
  Uint32 *pixel;
  int x,y,isok;
  Uint32 alpha;

  SDL_LockSurface(target);
  assert(target->format->Amask);
  assert(target->format->BitsPerPixel == 32);

  /* top */
  isok = 1;
  y = r->y;
  while (isok)
    {
      for (x = r->x;x < r->x + r->w; x ++)
	{
	  pixel = (Uint32 *)(target->pixels) + x + y*(target->pitch)/4;
	  alpha = (*pixel & (target->format->Amask)) >> target->format->Ashift;
	  if (alpha > (unsigned int)threshold)
	    {
	      isok = 0;
	      break;
	    }
	}
      y++;
      if (y > r->y + r->h - 2)
	{
	  r->h = 1;
	  goto unlock;
	}
    }
  y--;
  r->h -= y - r->y;
  r->y = y;

  /* left */
  isok = 1;
  x = r->x;
  while (isok)
    {
      for (y = r->y;y < r->y + r->h;y ++)
	{
	  pixel = (Uint32 *)(target->pixels) + x + y*(target->pitch)/4;
	  alpha = (*pixel & (target->format->Amask)) >> target->format->Ashift;
	  if (alpha > (unsigned int)threshold)
	    {
	      isok = 0;
	      break;
	    }
	}
      x++;
      if (x > r->x + r->w - 2)
	{
	  r->w = 1;
	  goto unlock;
	}
    }
  x--;
  r->w -= x - r->x;
  r->x = x;
  /* bottom */
  isok = 1;
  y = r->y + r->h - 1;
  while (isok)
    {
      for (x = r->x; x < r->x + r->w; x++)
	{
	  pixel = (Uint32 *)(target->pixels) + x + y*(target->pitch)/4;
	  alpha = (*pixel & (target->format->Amask)) >> target->format->Ashift;
	  if (alpha > (unsigned int)threshold)
	    {
	      isok = 0;
	      break;
	    }
	}
      y--;
      if (y < r->y + 1)
	{
	  r->h = 1;
	  goto unlock;
	}
    }
  y++;
  r->h = y - r->y + 1;
  /* right */
  isok = 1;
  x = r->x + r->w - 1;
  while (isok)
    {
      for (y = r->y;y < r->y + r->h;y ++)
	{
	  pixel = (Uint32 *)(target->pixels) + x + y*(target->pitch)/4;
	  alpha = (*pixel & (target->format->Amask)) >> target->format->Ashift;
	  if (alpha > (unsigned int)threshold)
	    {
	      isok = 0;
	      break;
	    }
	}
      x--;
      if (x < r->x + 1)
	{
	  r->w = 1;
	  goto unlock;
	}
    }
  x++;
  r->w = x - r->x + 1;
 unlock:
  SDL_UnlockSurface(target);
}

static bitmask_t *mask_from_image_alpha(SDL_Surface *target, int threshold)
{
  Uint32 *pixel;
  int x,y;
  Uint32 alpha;
  bitmask_t *m;

  assert(target->format->Amask);
  assert(target->format->BitsPerPixel == 32);
  m = bitmask_create(target->w,target->h);
  if (!m)
    return 0;
  SDL_LockSurface(target);
  for (x = 0;x < target->w;x ++)
    for (y = 0;y < target->h; y++)
      {
	pixel = (Uint32 *)(target->pixels) + x + y*(target->pitch)/4;
	alpha = (*pixel & (target->format->Amask)) >> target->format->Ashift;
	if (alpha > (unsigned int)threshold)
	  bitmask_setbit(m,x,y);
      }
  SDL_UnlockSurface(target);
  return m;
}

/* Ugly old function. */
static bitmask_t *mask_from_image(SDL_Surface *surface)
{
  bitmask_t *b;
  int i,j;
  SDL_Rect *r,rr;
  Uint32 colorkey;

  if (surface->format->Amask)
    return mask_from_image_alpha(surface,128);

  colorkey = SDL_MapRGB(surface->format,MAGICPINK);
  assert(surface);

  rr.x = 0;
  rr.y = 0;
  rr.w = surface->w;
  rr.h = surface->h;
  r = &rr;

  b = bitmask_create(r->w,r->h);
  if (!b) 
    return 0;

  SDL_LockSurface(surface);
  switch (surface->format->BytesPerPixel) {
  case 1: 
    { /* Assuming 8-bpp */
      for (j = r->y; j < r->h + r->y; j++)
	{
	  for (i=r->x; i < r->w + r->x; i++)
	    {
	      if (*((Uint8 *)surface->pixels + (j)*surface->pitch + i) 
		  != colorkey)
		bitmask_setbit(b,i-r->x,j-r->y);
	    }
	}
    }
    break;
    
  case 2:  /* Probably 15-bpp or 16-bpp */
    for (j=r->y;j<r->h+r->y;j++)
      {
	for (i=r->x;i<r->w+r->x;i++)
	  {
	    if (*((Uint16 *)surface->pixels + (j)*surface->pitch/2 + i)
		!= colorkey)
	      bitmask_setbit(b,i-r->x,j-r->y);
	  }
      }
    break;
  case 3: 
    { /* Slow 24-bpp mode, usually not used */
      Uint8 *bufp;
      for (j=r->y;j<r->h+r->y;j++)
	{
	  for (i=r->x;i<r->w+r->x;i++)
	    {		
	      bufp = (Uint8 *)surface->pixels + (j)*surface->pitch + (i) * 3;
	      if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
		
		if (!((bufp[0] == (colorkey & 255)) &&
		      (bufp[1] == ((colorkey >> 8) & 255)) &&
		      (bufp[2] == ((colorkey >> 16) & 255))))
		  bitmask_setbit(b,i-r->x,j-r->y);
	      } else 
		{
		  if (!((bufp[2] == (colorkey & 255)) &&
			(bufp[1] == ((colorkey >> 8) & 255)) &&
			(bufp[0] == ((colorkey >> 16) & 255))))
		    bitmask_setbit(b,i-r->x,j-r->y);
		}
	    }
	}
    }
    break;
  case 4:  /* Probably 32-bpp */
    for (j=r->y;j<r->h+r->y;j++)
      {
	for (i=r->x;i<r->w+r->x;i++)
	  {
	    if (*((Uint32 *)surface->pixels + (j)*surface->pitch/4 + i) 
		!= colorkey)
	      {
		bitmask_setbit(b,i-r->x,j-r->y);
	      }
	  }
      }
    break;
  }
  SDL_UnlockSurface(surface);
  return b;
}

static void threshold_alpha(SDL_Surface *target, int threshold)
{
  Uint32 *pixel;
  int x,y;
  Uint32 alpha;
  
  if (!(target->format->Amask)) return;

  assert(target->format->BitsPerPixel == 32);
  SDL_LockSurface(target);
  for (x = 0;x < target->w;x ++)
    for (y = 0;y < target->h; y++)
      {
	pixel = (Uint32 *)(target->pixels) + x + y*(target->pitch)/4;
	alpha = (*pixel & (target->format->Amask)) >> target->format->Ashift;
	if (alpha > (unsigned int )threshold)
	  *pixel |= target->format->Amask;
	else
	  *pixel &= ~target->format->Amask;
      }
  SDL_UnlockSurface(target);
}

/* Converts a section of src to display format, and creates a mask if
 * mask_threshold > 0 and there is either alpha or colorkey
 * information.  
 *
 * If src has an alpha channel and alpha_threshold == 0 then the dest
 * will also have an alpha channel.  
 *
 * Else if alpha_threshold > 0 the dest will have a colorkey (magic
 * pink), where alpha > alpha_threshold will be transparent.  
 * 
 * If the source does not have an alpha channel then if
 * alpha_threshold >= 0 it is assumed to be colorkeyed with magic
 * pink, else it is just flat.  
 *
 * surface_flags will be used when setting colorkey, alpha etc.
 *
 * If crop is used then the target surface will be cropped as long
 * as no {alpha > 0|colorkey opaque} pixels are lost. The corresponding
 * change in center is stored in the returned image.
 */
image_t *image_get(SDL_Surface *src, SDL_Rect *r,
		   int alpha_threshold, int mask_threshold,
		   Uint32 surface_flags, int crop)
{
  image_t *image;
  int hasalpha;
  SDL_Surface *tmp;
  SDL_Rect rorig,rfull;

  hasalpha = src->format->Amask != 0;

  if (r == 0)
    {
      r = &rfull;
      r->x = 0;
      r->y = 0;
      r->w = src->w;
      r->h = src->h;
    }

  rorig = *r;

  if (hasalpha && crop)
    autocrop(src,r,1); /* last arg is threshold for cropping */

  image = xcalloc(1, sizeof(*image));

  /* ugly hack to get a proper target surface */
  tmp = SDL_CreateRGBSurface(SDL_SWSURFACE,
			      r->w,r->h,8,0,0,0,0);
  assert(tmp);

  if (!hasalpha)
    image->img = SDL_DisplayFormat(tmp);
  else
    image->img = SDL_DisplayFormatAlpha(tmp);
  assert(image->img);
  SDL_FreeSurface(tmp);

  if (!hasalpha)
    {
      SDL_BlitSurface(src,r,image->img,0);
      if (alpha_threshold >= 0)
	{
	  SDL_SetColorKey(image->img,SDL_SRCCOLORKEY|surface_flags,
			  SDL_MapRGB((image->img)->format,255,0,255));
	}
      if (mask_threshold > 0)
	{
	  image->mask = mask_from_image(image->img);
	  assert(image->mask);
	}      
    }
  else if (alpha_threshold == 0) /* We want alpha in the target also */
    {
      SDL_SetAlpha(src,0,0);
      SDL_BlitSurface(src,r,image->img,0);
      SDL_SetAlpha(image->img,SDL_SRCALPHA|surface_flags,0);
      if (mask_threshold > 0)
	{
	  image->mask = mask_from_image_alpha(image->img,mask_threshold);
	  assert(image->mask);
	}
    }
  else /* alpha source, non-alpha target */
    {
      SDL_SetAlpha(src,0,0);
      SDL_BlitSurface(src,r,image->img,0);
      threshold_alpha(image->img,alpha_threshold);
      tmp = SDL_DisplayFormat(image->img);
      SDL_FillRect(tmp,0,SDL_MapRGB(tmp->format,255,0,255));
      SDL_SetAlpha(image->img,SDL_SRCALPHA,0);
      SDL_BlitSurface(image->img,0,tmp,0);
      SDL_FreeSurface(image->img);
      image->img = tmp;
      SDL_SetColorKey(image->img,SDL_SRCCOLORKEY|surface_flags,
		      SDL_MapRGB((image->img)->format,255,0,255));
      if (mask_threshold > 0)
	{
	  image->mask = mask_from_image(image->img);
	  assert(image->mask);
	}
    }
  image->xoff = rorig.w/2 - (r->x - rorig.x);
  image->yoff = rorig.h/2 - (r->y - rorig.y);
  return image;
}

void image_free(image_t *image)
{
  assert(image && image->img);
  free(image->img);
  if (image->mask)
    free(image->mask);
  free(image);
}

SDL_Surface *load_surface(const char *path)
{
  SDL_Surface *s;
  s = IMG_Load(path);
  if (s == 0)
    {
      fprintf(stderr,"load_surface(): Couldn't load image '%s'.\n",path);
      exit(EXIT_FAILURE);
    }
  return s;
}

void free_surface(SDL_Surface *s)
{
  assert(s);
  SDL_FreeSurface(s);
}

image_t *image_load(const char *imagepath, const char *maskpath)
{
  SDL_Surface *s;
  image_t *image,*maskimage;
  s = IMG_Load(imagepath);
  if (s == 0)
    {
      fprintf(stderr,"image_load(): Couldn't load image '%s'.\n",imagepath);
      exit(EXIT_FAILURE);
    }
  image = image_get(s,0,0,0,SDL_SWSURFACE,0);
  SDL_FreeSurface(s);
  if (maskpath)
    {
      if (image->mask)
	bitmask_free(image->mask);
      s = IMG_Load(maskpath);
      if (s == 0)
	{
	  fprintf(stderr,"image_load(): Couldn't load mask image '%s'.\n",imagepath);
	  exit(EXIT_FAILURE);
	}
      /* Expensive way to get the mask, but that shouldn't matter */ 
      maskimage = image_get(s,0,128,128,SDL_SWSURFACE,0);
      SDL_FreeSurface(s);
      image->mask = maskimage->mask;
      SDL_FreeSurface(maskimage->img);
      free(maskimage);
    }
  image->xoff = 0;
  image->yoff = 0;
  return image;
}
