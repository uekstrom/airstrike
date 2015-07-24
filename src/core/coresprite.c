#include "coresprite.h"
#include "display.h"

void display_coresprite(const coresprite_t *sprite)
{
  if (sprite->image)
    display_image(sprite->image,sprite->x,sprite->y);
}

int coresprite_at_point(const coresprite_t *s1, int x, int y)
{
  x = x - s1->x + s1->image->xoff;
  y = y - s1->y + s1->image->yoff;
  if (x < 0 || y < 0 || x >= s1->image->img->w || y >= s1->image->img->h)
    return 0;
  else
    return bitmask_getbit(s1->image->mask,x,y);
}

int coresprite_overlap(const coresprite_t *s1, const coresprite_t *s2)
{
  return bitmask_overlap(s1->image->mask,s2->image->mask,
			 s2->x - s2->image->xoff - s1->x + s1->image->xoff,
			 s2->y - s2->image->yoff - s1->y + s1->image->yoff);
}

int coresprite_overlap_pos(const coresprite_t *s1, const coresprite_t *s2, int *x, int *y)
{
  int c;
  c = bitmask_overlap_pos(s1->image->mask,s2->image->mask,
			  s2->x - s2->image->xoff - s1->x + s1->image->xoff,
			  s2->y - s2->image->yoff - s1->y + s1->image->yoff,
			  x,y);
  if (c)
    {
      *x += s1->x - s1->image->xoff;
      *y += s1->y - s1->image->yoff;
    }
  return c;
}

int coresprite_overlap_area(const coresprite_t *s1, const coresprite_t *s2)
{
  return bitmask_overlap_area(s1->image->mask,s2->image->mask,
			      s2->x - s2->image->xoff - s1->x + s1->image->xoff,
			      s2->y - s2->image->yoff - s1->y + s1->image->yoff);
}

int coresprite_overlap_normal(const coresprite_t *s1, const coresprite_t *s2, int *dx, int *dy)
{
  int ax1,ax2,ay1,ay2;
  ax1 = bitmask_overlap_area(s1->image->mask,s2->image->mask,
			     s2->x - s2->image->xoff - s1->x + s1->image->xoff - 1,
			     s2->y - s2->image->yoff - s1->y + s1->image->yoff);
  ax2 = bitmask_overlap_area(s1->image->mask,s2->image->mask,
			     s2->x - s2->image->xoff - s1->x + s1->image->xoff + 1,
			     s2->y - s2->image->yoff - s1->y + s1->image->yoff);
  ay1 = bitmask_overlap_area(s1->image->mask,s2->image->mask,
			     s2->x - s2->image->xoff - s1->x + s1->image->xoff,
			     s2->y - s2->image->yoff - s1->y + s1->image->yoff - 1);
  ay2 = bitmask_overlap_area(s1->image->mask,s2->image->mask,
			     s2->x - s2->image->xoff - s1->x + s1->image->xoff,
			     s2->y - s2->image->yoff - s1->y + s1->image->yoff + 1);
  *dx = ax2 - ax1;
  *dy = ay2 - ay1;
  return (ax1 + ax2 + ay1 + ay2)/4;
}
