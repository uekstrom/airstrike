#ifndef IMAGE_H
#define IMAGE_H

#include <SDL.h>
#include "bitmask.h"

/* the masks are B/W where black indicates void... */
#define MAGICPINK 0,0,0

typedef struct image
{
  SDL_Surface *img;
  bitmask_t *mask;
  int xoff,yoff;
} image_t;

/* Create an image from src surface, which must be in 32bpp format if
   it contains an alpha channel.  Will place (xoff,yoff) in center of
   image (before crop). If r == 0 the whole surface is used.
   The SDL surface will be created with surface_flags.
*/
image_t *image_get(SDL_Surface *src, SDL_Rect *r,
		   int alpha_threshold, int mask_threshold,
		   Uint32 surface_flags, int crop);

void image_free(image_t *image);

SDL_Surface *load_surface(const char *path);
void free_surface(SDL_Surface *s);
/* Load an image from imagepath, with an optional mask from maskpath */
image_t *image_load(const char *imagepath, const char *maskpath);

#endif
