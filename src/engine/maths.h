#ifndef MATHS_H
#define MATHS_H

#include <math.h>
#include "compat.h"

typedef struct vector
{
  float x,y;
} vector_t;

static INLINE vector_t vector(float x, float y)
{
  vector_t v;
  v.x = x;
  v.y = y;
  return v;
}

/*
 * Defines some 2d vector utility functions.
 *
 * 256 <-> 360 degrees 
 * trigtable[i][0] is the cosine, and trig[i][1] is the sine,
 * so trigtable[i] is the unit vector in the i direction
 */
extern vector_t trigtable[256]; 

static INLINE vector_t trig(int angle)
{
  return trigtable[angle & 255];
}

#ifndef MAX
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#endif

static INLINE float sq(float x)
{
  return x*x;
}

/* This is a little unconventional at x=0 */
static INLINE float sgn(float x)
{
  if (x > 0)
    return 1;
  else 
    return -1;
}

static INLINE float hmean(float x,float y) /*harmonic mean of x and y */
{
  return x*y/(x+y);
}

void maths_setup(void);

static INLINE float vdot(vector_t v1, vector_t v2)
{
  return v1.x*v2.x + v1.y*v2.y;
}

static INLINE float vcross(vector_t v1, vector_t v2)
{
  return v1.x*v2.y - v1.y*v2.x;
}

float vabs(vector_t v);
float vabs2(vector_t v);
vector_t vnormalize(vector_t v);
vector_t vnormalize2(vector_t v);

static INLINE vector_t vadd(vector_t v1, vector_t v2)
{
  vector_t v;
  v.x = v1.x + v2.x;
  v.y = v1.y + v2.y;
  return v;
}

static INLINE float vdist2(vector_t v1, vector_t v2)
{
  return sq(v1.x - v2.x) + sq(v1.y - v2.y);
}

static INLINE double vdist(vector_t v1, vector_t v2)
{
  return sqrt(sq(v1.x - v2.x) + sq(v1.y - v2.y));
}

static INLINE vector_t vsub(vector_t v1, vector_t v2)
{
  vector_t v;
  v.x = v1.x - v2.x;
  v.y = v1.y - v2.y;
  return v;
}

static INLINE vector_t vmul(vector_t v1, float x)
{
  vector_t v;
  v.x = v1.x*x;
  v.y = v1.y*x;
  return v;
}

/* return v1 + x*v2 */
static INLINE vector_t vmadd(vector_t v1, vector_t v2, float x)
{
  vector_t v;
  v.x = v1.x + v2.x*x;
  v.y = v1.y + v2.y*x;
  return v;
}



vector_t vrot(vector_t v, int angle);

float frand(void); /* return a random float from [0..1] */
float frand_uniform(float min, float max); /* Return a random number from [min..max] */

#endif
