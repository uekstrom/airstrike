#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <assert.h>

float *frombmp(const char *filename,int *w, int *h)
{
  SDL_Surface *image,*source;
  float *field;
  Uint32 *srcpix;
  int x,y;
  Uint8 r,g,b;

  image = IMG_Load(filename);
  if (!image)
    {
      fprintf(stderr,"Error: Cannot open image '%s', quitting.\n",
	      filename);
      exit(EXIT_FAILURE);
    }
  
  source = SDL_CreateRGBSurface(SDL_SWSURFACE,image->w,image->h,
				32, 255, 255 << 8, 255 << 16, 0);
  assert(source);
  SDL_BlitSurface(image,0,source,0);  
  srcpix = source->pixels;

  *w = image->w;
  *h = image->h;
  field = malloc(sizeof(*field)*(*w)*(*h));
  assert(field);
  
  for (y=0;y<image->h;y++)
    for (x=0;x<image->w;x++)
      {
	SDL_GetRGB(srcpix[x + y*source->pitch/4],
		   source->format,&r,&g,&b);
	field[x + y*(*w)] = (r*256 + g)/((float)(1 << 17) - 1);
      }
  SDL_FreeSurface(source);
  SDL_FreeSurface(image);
  return field;
}

void tobmp(const char *filename,float *field,int w, int h)
{
  SDL_Surface *source;
  Uint32 *srcpix;
  int x,y,z;
  
  source = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,
				32, 255, 255 << 8, 255 << 16, 0);
  assert(source);
  srcpix = source->pixels;  
  
  for (y=0;y<h;y++)
    for (x=0;x<w;x++)
      {
	z = (int)(field[x + y*w]*((1 << 17) - 1));
	srcpix[x + y*source->pitch/4] = 
	  SDL_MapRGB(source->format,z / 256, z & 255,0);
      }
  SDL_SaveBMP(source,filename);
}

int main(int argc, char *argv[])
{
  int w,h,x,y,i;
  float *field;
  if (argc != 3) {
    fprintf(stderr, "usage: %s <src image> <dest image>\n", *argv);
    return -1;
  }
  field = frombmp(argv[1],&w,&h);
  SDL_Init(SDL_INIT_VIDEO);
  for (i=0;i<8;i++)
    for (y=1;y<h-1;y++)
      for (x=1;x<w-1;x++)
	{
	  field[x + w*y] = 
	    0.25*(field[x + 1 + w*y] + field[x + w*(y - 1)] +
		  field[x - 1 + w*y] + field[x + w*(y + 1)]);
	}
  tobmp(argv[2],field,w,h);
  return EXIT_SUCCESS;
}
