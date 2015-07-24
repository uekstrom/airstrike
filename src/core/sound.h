#ifndef SOUND_H
#define SOUND_H

#ifdef USE_SOUND
#include <SDL.h>
#include <SDL_mixer.h>
#else
typedef void Mix_Chunk;
#endif

typedef struct sound
{
  const char *filename;
  int nr_loops;
  Mix_Chunk *wave;
  int init_ok; /* 0: not loaded, 1: loaded ok, -1: failed */ 
} sound_t;

/* Intended for continous sounds, does not report sound ending.  This
   is sort of a convenience device; it can be destroyed without
   needing to 'unregister' it. It should however be stopped (using
   set_sound with sound = 0) before freeing it (It is usually part of
   a sprite) */
typedef struct sound_source
{
  sound_t *sound; /* These are private, don't touch */
  int channel;
} sound_source_t;

int sound_setup(void);
void sound_update(void); /* moves sounds etc */
void sound_move_ear(int x, int y);

/* One time sound effects. Should be pretty short lived. */
void sound_effect(sound_t *sound, int x, int y);

/* Set sound = 0 to stop playing. Setting the same sound twice does
   not cause a reset of the sound. */
void sound_source_set_sound(sound_source_t *source, sound_t *sound);
void sound_source_set_pos(sound_source_t *source, int x, int y);

#endif
