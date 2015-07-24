#include <SDL.h>
#include <assert.h>
#include "input.h"

int key_pressed(int key)
{
  int n;
  Uint8 *keys = SDL_GetKeyState(&n);
  assert(key < n && "Invalid key");
  return keys[key];
}

int key_grab(int *key) /* Return 0 if exactly one key is pressed, else -1 */
{
  int n;
  Uint8 *keys = SDL_GetKeyState(&n);
  *key = -1;
  for (n--;n>=0;n--)
    {
      if (keys[n])
	{
	  if (*key > -1)
	    return -1;
	  else
	    *key = n;
	}
    }
  return 0;
}

 
