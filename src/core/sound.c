#include "sound.h"

#ifndef USE_SOUND

#include <stdio.h>

int sound_setup(void) { return 0; }
void sound_move_ear(int x, int y)
{
  fprintf(stderr, "SOUND: set ear at (%d,%d)\n", x, y);
}
void sound_effect(sound_t *sound, int x, int y)
{
  /* visual audio ;-) */
  fprintf(stderr, "SOUND: play %s at (%d,%d)\n", sound->filename, x, y);
}
void sound_source_set_sound(sound_source_t *source, sound_t *sound)
{
  fprintf(stderr, "SOUND: set sound %s to channel %d\n",
	  sound->filename, source->channel);
}
void sound_source_set_pos(sound_source_t *source, int x, int y)
{
  fprintf(stderr, "SOUND: position channel %d (%d,%d)\n",
	  source->channel, x, y);
}


#else /* SOUND */

#include <stdlib.h>

static int sound_ok = 0;
static int sound_on = 0; /* = 1 if sound should be played */
static int ear_x = 0;
static int ear_y = 0;

static void channel_set_pos(int channel, int x, int y)
{
  int panning = (x - ear_x) / 4 + 128;
  int d;
  if (panning < 0)
    panning = 0;
  else if (panning > 255)
    panning = 255;
  Mix_SetPanning(channel, 255 - panning, panning);
  d = (600 - y) / 3;
  Mix_SetDistance(channel,d);
}

void sound_move_ear(int x, int y)
{
  ear_x = x;
  ear_y = y;
}

/* Some of this code is cleverly stolen from the
   playwav test program of SDL_mixer -- ulf*/
int sound_setup(void)
{
  int audio_rate = MIX_DEFAULT_FREQUENCY;
  Uint16 audio_format = MIX_DEFAULT_FORMAT;
  int audio_channels = 1;
  int buflen;
  buflen = 2048;
  if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, buflen) < 0)
    {
      fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
      return -1; 
    } 
  else 
    {
      Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
      printf("Opened audio at %d Hz %d bit %s\n", audio_rate,
	     (audio_format&0xFF),
	     (audio_channels > 1) ? "stereo" : "mono");
    }
  sound_ok = 1;
  sound_on = 1;
  return 0;
}

void sound_effect(sound_t *s, int x, int y)
{
  int channel;
  if (sound_on)
    {
      if (s->init_ok == 0) /* lazy loading */
	{
	  /*fprintf(stderr,"loading sound '%s'\n",s->filename);*/
	  s->wave = Mix_LoadWAV(s->filename);
	  if (s->wave)
	    {
	      s->init_ok = 1;
	    }
	  else
	    {
	      fprintf(stderr,"Could not open sound %s\n",s->filename);
	      s->init_ok = -1;
	    }
	}
      if (s->init_ok == 1)
	{
	  channel = Mix_PlayChannel(-1, s->wave, s->nr_loops);
	  channel_set_pos(channel, x, y);
	}
    }
}

void sound_source_set_pos(sound_source_t *source, int x, int y)
{
  if (source->sound)
    channel_set_pos(source->channel, x, y);
}

void sound_source_set_sound(sound_source_t *source, sound_t *s)
{
  int channel;
  if ((source->sound == s) || (!sound_on))
    return;
  if (s)
    {
      if (s->init_ok == 0) /* lazy loading */
	{
	  fprintf(stderr,"loading sound '%s'\n",s->filename);
	  s->wave = Mix_LoadWAV(s->filename);
	  if (s->wave)
	    {
	      s->init_ok = 1;
	    }
	  else
	    {
	      fprintf(stderr,"Could not open sound %s\n",s->filename);
	      s->init_ok = -1;
	    }
	}
      if (s->init_ok == 1)
	{
	  if (source->sound)
	    channel = source->channel;
	  else
	    channel = -1;
	  channel = Mix_PlayChannel(channel, s->wave, s->nr_loops);
	  source->channel = channel;
	  source->sound = s;
	}
    }
  else
    {
      if (source->sound)
	{
	  Mix_HaltChannel(source->channel);
	  source->sound = 0;
	}
    }
}

#endif /* SOUND */
