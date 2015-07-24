/*
    bitmask 1.3
    Copyright (C) 2002-2004 Ulf Ekstrom except for the bitcount function which
    is copyright (C) Donald W. Gillies, 1992.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
 
    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdlib.h>
#include <string.h>
#include "bitmask.h"

#define MIN(a,b) ((a) < (b) ? (a) : (b))

bitmask_t *bitmask_create(int w, int h)
{
  bitmask_t *temp = malloc(sizeof(bitmask_t));
  if (!temp) 
    return 0;
  temp->w = w;
  temp->h = h;
  temp->bits = calloc(h*((w - 1)/BITW_LEN + 1),sizeof(BITW));
  if (!temp->bits) 
    {
      free(temp);
      return 0;
    }
  else
    return temp;
}

void bitmask_free(bitmask_t *m)
{
  free(m->bits);
  free(m);
}

void bitmask_clear(bitmask_t *m)
{
  memset(m->bits,0,m->h*((m->w - 1)/BITW_LEN + 1)*sizeof(BITW));
}

void bitmask_fill(bitmask_t *m)
{
  int x,y;
  for (y = 0; y < m->h; y++)
    for (x = 0; x < m->w; x++)
      bitmask_setbit(m, x, y);
}

int bitmask_overlap(const bitmask_t *a, const bitmask_t *b, int xoffset, int yoffset)
{
  BITW *a_entry,*a_end;
  BITW *b_entry;
  BITW *ap,*app,*bp;
  unsigned int shift,rshift,i,astripes,bstripes;
  
  if ((xoffset >= a->w) || (yoffset >= a->h) || (b->h + yoffset <= 0) || (b->w + xoffset <= 0)) 
      return 0;

  if (xoffset >= 0) 
    {
    swapentry:
      if (yoffset >= 0)
	{
	  a_entry = a->bits + a->h*((unsigned int)xoffset/BITW_LEN) + yoffset;
	  a_end = a_entry + MIN(b->h,a->h - yoffset);
	  b_entry = b->bits;
	}
      else
	{
	  a_entry = a->bits + a->h*((unsigned int)xoffset/BITW_LEN);
	  a_end = a_entry + MIN(b->h + yoffset,a->h);
	  b_entry = b->bits - yoffset;
	}
      shift = xoffset & BITW_MASK;
      if (shift)
	{
	  rshift = BITW_LEN - shift;
	  astripes = ((unsigned int)(a->w - 1))/BITW_LEN - (unsigned int)xoffset/BITW_LEN;
	  bstripes = ((unsigned int)(b->w - 1))/BITW_LEN + 1;
	  if (bstripes > astripes) /* zig-zag .. zig*/
	    {
	      for (i=0;i<astripes;i++)
		{
		  for (ap = a_entry, app = ap + a->h, bp = b_entry;ap < a_end;) 
		    if ((*ap++ >> shift) & *bp || (*app++ << rshift) & *bp++) return 1;
		  a_entry += a->h;
		  a_end += a->h;
		  b_entry += b->h;
		}
	      for (ap = a_entry,bp = b_entry;ap < a_end;)
		  if ((*ap++ >> shift) & *bp++) return 1;
	      return 0;
	    }
	  else /* zig-zag */
	    {
	      for (i=0;i<bstripes;i++)
		{
		  for (ap = a_entry, app = ap + a->h, bp = b_entry;ap < a_end;) 
		    if ((*ap++ >> shift) & *bp || (*app++ << rshift) & *bp++) return 1;
		  a_entry += a->h;
		  a_end += a->h;
		  b_entry += b->h;
		}
	      return 0;
	    }
	}
      else /* xoffset is a multiple of the stripe width, and the above routines wont work */
	{
	  astripes = (MIN(b->w,a->w - xoffset) - 1)/BITW_LEN + 1;
	  for (i=0;i<astripes;i++)
	    {
	      for (ap = a_entry,bp = b_entry;ap < a_end;)
		if (*ap++ & *bp++) return 1;
	      a_entry += a->h;
	      a_end += a->h;
	      b_entry += b->h;
	    }
	  return 0;
	}
    }
  else  
    {
      const bitmask_t *c = a;
      a = b;
      b = c;
      xoffset *= -1;
      yoffset *= -1;
      goto swapentry;
    }
}

/* Will hang if there are no bits set in w! */
static INLINE int firstsetbit(BITW w)
{
  int i = 0;
  while ((w & 1) == 0) 
    {
      i++;
      w/=2;
    }
  return i;
}

/* x and y are given in the coordinates of mask a, and are untouched if there is no overlap */
int bitmask_overlap_pos(const bitmask_t *a, const bitmask_t *b, int xoffset, int yoffset, int *x, int *y)
{
  BITW *a_entry,*a_end;
  BITW *b_entry;
  BITW *ap,*bp;
  unsigned int shift,rshift,i,astripes,bstripes,xbase;
  
  if ((xoffset >= a->w) || (yoffset >= a->h) || (yoffset <= - b->h)) 
      return 0;
  
  if (xoffset >= 0) 
    {
      xbase = xoffset/BITW_LEN; /* first stripe from mask a */
      if (yoffset >= 0)
	{
	  a_entry = a->bits + a->h*xbase + yoffset;
	  a_end = a_entry + MIN(b->h,a->h - yoffset);
	  b_entry = b->bits;
	}
      else
	{
	  a_entry = a->bits + a->h*xbase;
	  a_end = a_entry + MIN(b->h + yoffset,a->h);
	  b_entry = b->bits - yoffset;
	  yoffset = 0; /* relied on below */
	}
      shift = xoffset & BITW_MASK;
      if (shift)
	{
	  rshift = BITW_LEN - shift;
	  astripes = (a->w - 1)/BITW_LEN - xoffset/BITW_LEN;
	  bstripes = (b->w - 1)/BITW_LEN + 1;
	  if (bstripes > astripes) /* zig-zag .. zig*/
	    {
	      for (i=0;i<astripes;i++)
		{
		  for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		      if (*ap & (*bp << shift)) 
			{
			  *y = ap - a_entry + yoffset;
			  *x = (xbase + i)*BITW_LEN + firstsetbit(*ap & (*bp << shift));
			  return 1;
			}
		  a_entry += a->h;
		  a_end += a->h;
		  for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		      if (*ap & (*bp >> rshift)) 
			{
			  *y = ap - a_entry + yoffset;
			  *x = (xbase + i + 1)*BITW_LEN + firstsetbit(*ap & (*bp >> rshift));
			  return 1;
			}
		  b_entry += b->h;
		}
	      for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		if (*ap & (*bp << shift)) 
		  {
		    *y = ap - a_entry + yoffset;
		    *x = (xbase + astripes)*BITW_LEN + firstsetbit(*ap & (*bp << shift));
		    return 1;
		  }
	      return 0;
	    }
	  else /* zig-zag */
	    {
	      for (i=0;i<bstripes;i++)
		{
		  for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		      if (*ap & (*bp << shift)) 
			{
			  *y = ap - a_entry + yoffset;
			  *x = (xbase + i)*BITW_LEN + firstsetbit(*ap & (*bp << shift));
			  return 1;
			}
		  a_entry += a->h;
		  a_end += a->h;
		  for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		      if (*ap & (*bp >> rshift)) 
			{
			  *y = ap - a_entry + yoffset;
			  *x = (xbase + i + 1)*BITW_LEN + firstsetbit(*ap & (*bp >> rshift));
			  return 1;
			}
		  b_entry += b->h;
		}
	      return 0;
	    }
	}
      else 
/* xoffset is a multiple of the stripe width, and the above routines
   won't work. This way is also slightly faster. */
	{
	  astripes = (MIN(b->w,a->w - xoffset) - 1)/BITW_LEN + 1;
	  for (i=0;i<astripes;i++)
	    {
	      for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		{
		  if (*ap & *bp)
		    {
		      *y = ap - a_entry + yoffset;
		      *x = (xbase + i)*BITW_LEN + firstsetbit(*ap & *bp); 
		      return 1;
		    }
		}
	      a_entry += a->h;
	      a_end += a->h;
	      b_entry += b->h;
	    }
	  return 0;
	}
    }
  else  
    {
      if (bitmask_overlap_pos(b,a,-xoffset,-yoffset,x,y))
	{
	  *x += xoffset;
	  *y += yoffset;
	  return 1;
	}
      else
	return 0;
    }
}


static INLINE unsigned int bitcount(BITW n)
{
  if (BITW_LEN == 32)
    {
/* (C) Donald W. Gillies, 1992.  All rights reserved.  You may reuse
   this bitcount() function anywhere you please as long as you retain
   this Copyright Notice. */
      register unsigned long tmp;
      return (tmp = (n) - (((n) >> 1) & 033333333333) - 
	      (((n) >> 2) & 011111111111),
	      tmp = ((tmp + (tmp >> 3)) & 030707070707),
	      tmp =  (tmp + (tmp >> 6)),
	      tmp = (tmp + (tmp >> 12) + (tmp >> 24)) & 077);
/* End of Donald W. Gillies bitcount code */
    }
  else
    {
      /* Handle non-32 bit case the slow way */
      unsigned int nbits = 0;
      while (n)
	{
	  if (n & 1)
	    nbits++;
	  n = n >> 1;
	}
      return nbits;
    }
}

int bitmask_overlap_area(const bitmask_t *a, const bitmask_t *b, int xoffset, int yoffset)
{
  BITW *a_entry,*a_end;
  BITW *b_entry;
  BITW *ap,*bp;
  unsigned int shift,rshift,i,astripes,bstripes;
  unsigned int count = 0;

  if ((xoffset >= a->w) || (yoffset >= a->h) || (yoffset <= - b->h)) 
      return 0;
  
  if (xoffset >= 0) 
    {
      if (yoffset >= 0)
	{
	  a_entry = a->bits + a->h*(xoffset/BITW_LEN) + yoffset;
	  a_end = a_entry + MIN(b->h,a->h - yoffset);
	  b_entry = b->bits;
	}
      else
	{
	  a_entry = a->bits + a->h*(xoffset/BITW_LEN);
	  a_end = a_entry + MIN(b->h + yoffset,a->h);
	  b_entry = b->bits - yoffset;
	}
      shift = xoffset & BITW_MASK;
      if (shift)
	{
	  rshift = BITW_LEN - shift;
	  astripes = (a->w - 1)/BITW_LEN - xoffset/BITW_LEN;
	  bstripes = (b->w - 1)/BITW_LEN + 1;
	  if (bstripes > astripes) /* zig-zag .. zig*/
	    {
	      for (i=0;i<astripes;i++)
		{
		  for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		    count += bitcount(((*ap >> shift) | (*(ap + a->h) << rshift)) & *bp);
		  a_entry += a->h;
		  a_end += a->h;
		  b_entry += b->h;
		}
	      for (ap = a_entry,bp = b_entry;ap < a_end;)
		count += bitcount((*ap++ >> shift) & *bp++);
	      return count;
	    }
	  else /* zig-zag */
	    {
	      for (i=0;i<bstripes;i++)
		{
		  for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		    count += bitcount(((*ap >> shift) | (*(ap + a->h) << rshift)) & *bp);
		  a_entry += a->h;
		  a_end += a->h;
		  b_entry += b->h;
		}
	      return count;
	    }
	}
      else /* xoffset is a multiple of the stripe width, and the above routines wont work */
	{
	  astripes = (MIN(b->w,a->w - xoffset) - 1)/BITW_LEN + 1;
	  for (i=0;i<astripes;i++)
	    {
	      for (ap = a_entry,bp = b_entry;ap < a_end;)
		count += bitcount(*ap++ & *bp++);

	      a_entry += a->h;
	      a_end += a->h;
	      b_entry += b->h;
	    }
	  return count;
	}
    }
  else  
      return bitmask_overlap_area(b,a,-xoffset,-yoffset);
}

/* Draws mask b onto mask a (bitwise OR) */
void bitmask_draw(bitmask_t *a, bitmask_t *b, int xoffset, int yoffset)
{
  BITW *a_entry,*a_end;
  BITW *b_entry;
  BITW *ap,*bp;
  bitmask_t *swap;
  int shift,rshift,i,astripes,bstripes;
  
  if ((xoffset >= a->w) || (yoffset >= a->h) || (yoffset <= - b->h)) 
      return;
  
  if (xoffset >= 0) 
    {
      if (yoffset >= 0)
	{
	  a_entry = a->bits + a->h*(xoffset/BITW_LEN) + yoffset;
	  a_end = a_entry + MIN(b->h,a->h - yoffset);
	  b_entry = b->bits;
	}
      else
	{
	  a_entry = a->bits + a->h*(xoffset/BITW_LEN);
	  a_end = a_entry + MIN(b->h + yoffset,a->h);
	  b_entry = b->bits - yoffset;
	}
      shift = xoffset & BITW_MASK;
      if (shift)
	{
	  rshift = BITW_LEN - shift;
	  astripes = (a->w - 1)/BITW_LEN - xoffset/BITW_LEN;
	  bstripes = (b->w - 1)/BITW_LEN + 1;
	  if (bstripes > astripes) /* zig-zag .. zig*/
	    {
	      for (i=0;i<astripes;i++)
		{
		  for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		    *ap |= (*bp << shift);
		  a_entry += a->h;
		  a_end += a->h;
		  for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		    *ap |= (*bp >> rshift);
		  b_entry += b->h;
		}
	      for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		*ap |= (*bp << shift);
	      return;
	    }
	  else /* zig-zag */
	    {
	      for (i=0;i<bstripes;i++)
		{
		  for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		    *ap |= (*bp << shift);
		  a_entry += a->h;
		  a_end += a->h;
		  for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		    *ap |= (*bp >> rshift);
		  b_entry += b->h;
		}
	      return;
	    }
	}
      else /* xoffset is a multiple of the stripe width, 
	      and the above routines won't work. */
	{
	  astripes = (MIN(b->w,a->w - xoffset) - 1)/BITW_LEN + 1;
	  for (i=0;i<astripes;i++)
	    {
	      for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		{
		  *ap |= *bp;
		}
	      a_entry += a->h;
	      a_end += a->h;
	      b_entry += b->h;
	    }
	  return;
	}
    }
  else  
    {
      /* 'Swapping' arguments to be able to almost reuse the code above,
       should be taken care of by the compiler efficiently. */
      swap = a;
      a = b;
      b = swap;
      xoffset *= -1;
      yoffset *= -1;

      if (yoffset >= 0)
	{
	  a_entry = a->bits + a->h*(xoffset/BITW_LEN) + yoffset;
	  a_end = a_entry + MIN(b->h,a->h - yoffset);
	  b_entry = b->bits;
	}
      else
	{
	  a_entry = a->bits + a->h*(xoffset/BITW_LEN);
	  a_end = a_entry + MIN(b->h + yoffset,a->h);
	  b_entry = b->bits - yoffset;
	}
      shift = xoffset & BITW_MASK;
      if (shift)
	{
	  rshift = BITW_LEN - shift;
	  astripes = (a->w - 1)/BITW_LEN - xoffset/BITW_LEN;
	  bstripes = (b->w - 1)/BITW_LEN + 1;
	  if (bstripes > astripes) /* zig-zag .. zig*/
	    {
	      for (i=0;i<astripes;i++)
		{
		  for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		    *bp |= (*ap >> shift);
		  a_entry += a->h;
		  a_end += a->h;
		  for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		    *bp |= (*ap <<rshift); 
		  b_entry += b->h;
		}
	      for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		*bp |= (*ap >> shift);
	      return;
	    }
	  else /* zig-zag */
	    {
	      for (i=0;i<bstripes;i++)
		{
		  for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		    *bp |= (*ap >> shift);
		  a_entry += a->h;
		  a_end += a->h;
		  for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		    *bp |= (*ap << rshift);
		  b_entry += b->h;
		}
	      return;
	    }
	}
      else /* xoffset is a multiple of the stripe width, and the above routines won't work. */
	{
	  astripes = (MIN(b->w,a->w - xoffset) - 1)/BITW_LEN + 1;
	  for (i=0;i<astripes;i++)
	    {
	      for (ap = a_entry,bp = b_entry;ap < a_end;ap++,bp++)
		{
		  *bp |= *ap;
		}
	      a_entry += a->h;
	      a_end += a->h;
	      b_entry += b->h;
	    }
	  return;
	}
    }	
}

bitmask_t *bitmask_scale(bitmask_t *m, int w, int h)
{
  bitmask_t *nm;
  int x,y,nx,ny,dx,dy,dnx,dny;

  if (w*h == 0)
    {
      nm = bitmask_create(1,1);
      return nm;
    }

  nm = bitmask_create(w,h);
  ny = dny = 0;
  for (y=0,dy=h; y<m->h; y++,dy+=h)
    {      
      while (dny < dy)
	{
	  nx = dnx = 0;
	  for (x=0,dx=w; x < m->w; x++, dx+=w)
	    {
	      if (bitmask_getbit(m,x,y))
		{
		  while (dnx < dx)
		    {
		      bitmask_setbit(nm,nx,ny);
		      nx++;
		      dnx += m->w;
		    }
		}
	      else
		{
		  while (dnx < dx)
		    {
		      nx++;
		      dnx += m->w;
		    }
		}
	    }
	  ny++;
	  dny+=m->h;
	}
    }
  return nm;
}
