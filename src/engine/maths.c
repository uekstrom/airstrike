#include <math.h>
#include "random.h"
#include "maths.h"

#ifndef M_PI
# define M_PI 3.1415926535897931
#endif

vector_t trigtable[256]; 

void maths_setup(void)
{
  int i;
  for (i=0; i<256; i++)
    {
      trigtable[i].x = cos(M_PI*i/128.0);
      trigtable[i].y = sin(M_PI*i/128.0);
    }
}

float vabs(vector_t v)
{
  return sqrt(vdot(v,v));
}

float vabs2(vector_t v)
{
  return vdot(v,v);
}

vector_t vnormalize(vector_t v)
{
  float d = vdot(v,v);
  if (d < 1e-6)
    return vector(1,0);
  else
    return vmul(v,1/sqrt(d));
}

vector_t vnormalize2(vector_t v) // legacy code uses this
{
  float d = vdot(v,v);
  if (d < 1e-6)
    return vector(1,0);
  else
    return vmul(v,1/d);
}

vector_t vrot(vector_t v, int angle)
{
  vector_t r;
  angle &= 255;
  r.x = v.x*trigtable[angle].x - v.y*trigtable[angle].y;
  r.y = v.x*trigtable[angle].y + v.y*trigtable[angle].x;
  return r;
}

float frand(void) /* return a random float from [0..1] */
{
  return (get_random()/((float)RANDOM_MAX));
}

float frand_uniform(float min, float max) /* Return a random number from [min..max] */
{
  return min + (max - min)*(get_random()/((float)RANDOM_MAX));
}
