#ifndef CORESPRITE_H
#define CORESPRITE_H

#include "image.h"

typedef struct coresprite
{
  int x,y;
  image_t *image;  /* image == 0 is legal */
} coresprite_t;

void display_coresprite(const coresprite_t *sprite);

int coresprite_at_point(const coresprite_t *s1, int x, int y);
int coresprite_overlap(const coresprite_t *s1, const coresprite_t *s2);
int coresprite_overlap_pos(const coresprite_t *s1, const coresprite_t *s2, int *x, int *y);
int coresprite_overlap_area(const coresprite_t *s1, const coresprite_t *s2);
int coresprite_overlap_normal(const coresprite_t *s1, 
			      const coresprite_t *s2, 
			      int *dx, int *dy);

#endif
