#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include "image.h"

#define FRAMERATE 33
#define DIRTY_TILE_SIZE 64

extern struct display
{
  unsigned int frame_time; /* milliseconds/frame. */
  int use_alpha;
  int hw_accel; /* Automatically set to 1 if target accelerates alpha blits. */
  SDL_Surface *surface;
  SDL_Surface *background;
} display;

int display_open(int w, int h);
void display_close(void);

void display_set_background(SDL_Surface *image);

void display_clear(void); /* Makes sure the background is fresh */
void display_image(const image_t *image, int x, int y);
void display_update(void); /* Makes sure that the screen is updated, keep framerate */
void display_waitframe(void); /* Wait for the next frame to start, but don't draw anything. For benchmarking. */
#endif
