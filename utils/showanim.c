#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <assert.h>
#include <string.h>

int paused = 0;
int frame = 0;
int nr_frames = 0;

void process_events()
{
  SDL_Event event;
  while ( SDL_PollEvent(&event) >= 1 ) 
    {
      switch (event.type) 
	{
	case SDL_QUIT: 
	  exit(EXIT_SUCCESS);
	  break;
	case SDL_KEYDOWN:
	  switch (event.key.keysym.sym)
	    {
	    case SDLK_ESCAPE:
	      exit(EXIT_SUCCESS);
	      break;
	    case SDLK_LEFT:
	      frame--;
	      if (frame < 0)
		frame += nr_frames;
	      break;
	    case SDLK_RIGHT:
	      frame = (frame + 1) % nr_frames;
	      break;
	    case SDLK_SPACE:
	      paused = ~paused;
	      break;
	    default:
	      break;
	    }
	  break;
	default:
	  break;
	}
    }
}

void usage()
{
  puts("Usage: showanim [OPTIONS] IMAGE");
  puts("Chop up IMAGE and show it as a looping animation.");
  puts("");
  puts("Possible OPTIONS are");
  puts("  --help        Display this message.");
  puts("  --bg RGB      Set background to RGB (in hex, like ff00ff).");
  puts("  --frames N    Set number of frames to N.");
  puts("  --delay N     Set frame time to N milliseconds [~10 ms accuracy]");
  puts("");
  puts("If --frames is not given and image width is a multiple of the");
  puts("height then square frames are assumed. Inside the program use");
  puts("ESC to quit and space to pause/unpause. When paused use arrow");
  puts("keys to step forward/backwards. The image will be blitted"); 
  puts("with alpha channel if it exists, else a colorkey of ff00ff is");
  puts("used.");
  puts("");
  puts("Written by Ulf Ekstrom as part of the Airstrike project.");
  puts("This program is released under the GPL. No warranty!");
}


int main(int argc, char *argv[])
{
  SDL_Rect r;
  SDL_Surface *image,*display;
  int i;
  unsigned int red = 128;
  unsigned int green = 128;
  unsigned int blue = 128;
  Uint32 lasttime,frame_time = 30;
  char *filename = 0;

  i = 1;
  while (!filename)
    {
      if (i>=argc)
	{
	  fprintf(stderr,"Invalid arguments, try --help\n");
	  exit(EXIT_FAILURE);
	}
      else if (strcmp(argv[i],"--help") == 0)
	{
	  usage();
	  exit(EXIT_SUCCESS);
	}
      else if (strcmp(argv[i],"--bg") == 0)
	{
	  i++;
	  if (sscanf(argv[i],"%2x%2x%2x",&red,&green,&blue) != 3)
	    {
	      fprintf(stderr,"Error: Expected hex triplet, got '%s'.\n",argv[i]);
	      exit(EXIT_FAILURE);
	    }
	}
      else if (strcmp(argv[i],"--frames") == 0)
	{
	  i++;
	  if (sscanf(argv[i],"%i",&nr_frames) != 1)
	    {
	      fprintf(stderr,"Error: Expected integer, got '%s'.\n",argv[i]);
	      exit(EXIT_FAILURE);
	    }
	}
      else if (strcmp(argv[i],"--delay") == 0)
	{
	  i++;
	  if (sscanf(argv[i],"%ui",&frame_time) != 1)
	    {
	      fprintf(stderr,"Error: Expected integer, got '%s'.\n",argv[i]);
	      exit(EXIT_FAILURE);
	    }
	}
      else
	{
	  filename = argv[i];
	}
      i++;      
    }

  SDL_Init(SDL_INIT_VIDEO);
  SDL_WM_SetCaption(filename,0);
  image = IMG_Load(filename);
  if (!image)
    {
      fprintf(stderr,"Error: Cannot open image '%s', quitting.\n",
	      filename);
      exit(EXIT_FAILURE);
    }
  
  /* Guess nr_frames if not given */
  if (nr_frames == 0)
    {
      if ((image->w % image->h) == 0)
	nr_frames = image->w/image->h; /* assume square frames */
      else
	nr_frames = 1;
    }

  r.w = image->w / nr_frames;
  r.h = image->h;
  r.y = 0;
  assert(display = SDL_SetVideoMode(r.w,r.h,0,SDL_SWSURFACE));

  if (image->format->Amask)
    SDL_SetAlpha(image,SDL_SRCALPHA,0);
  else
    SDL_SetColorKey(image,SDL_SRCCOLORKEY,SDL_MapRGB(image->format,255,0,255));

  while (1)
    {
      lasttime = SDL_GetTicks();
      SDL_FillRect(display,0,SDL_MapRGB(display->format,red,green,blue));
      r.x = r.w * frame;
      SDL_BlitSurface(image,&r,display,0);
      SDL_UpdateRect(display,0,0,0,0);
      process_events();
      /* Assume timer resolution is 10 ms */
      while (SDL_GetTicks() - lasttime < frame_time - 5)
	  SDL_Delay(SDL_GetTicks() - lasttime);      
      if (!paused)
	frame = (frame + 1) % nr_frames;
    }
}
