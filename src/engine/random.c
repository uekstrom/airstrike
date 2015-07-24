/*
 * Random functions for networked Airstrike (the functions have
 * to be identical on each end)
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms specified by the BSD Licence.
 *
 * Copyright (c) 1983 Regents of the University of California.
 *
 * NOTES
 * - These functions are from Amiga NetBSD ca. 1995 and modified
 *   for the Airstrike game by Eero Tamminen
 */
#include "random.h"

static long randtbl[] = {
  0x9a319039, 0x32d9c024, 0x9b663182, 0x5da1f342, 0xde3b81e0, 0xdf0a6fb5,
  0xf103bc02, 0x48f340fb, 0x7449e56b, 0xbeb1dbb0, 0xab5c5918, 0x946554fd,
  0x8c2e680f, 0xeb3d799f, 0xb11ee0b7, 0x2d436b86, 0xda672e2a, 0x1588ca88,
  0xe369735d, 0x904f35f7, 0xd7158fd6, 0x6fa6f051, 0x616e6b96, 0xac94efdc,
  0x36413f93, 0xc622c298, 0xf5a42ab8, 0x8a88d77b, 0xf5ad9d0e, 0x8999220b,
  0x27fb47b9,
};
static long *fptr = &randtbl[3];
static long *rptr = &randtbl[0];
static long *state = &randtbl[0];
static long *end_ptr = &randtbl[31];

long get_random(void)
{
	long i;
	*fptr += *rptr;
	i = (*fptr >> 1) & 0x7fffffff;
	if (++fptr >= end_ptr) {
		fptr = state;
		++rptr;
	} else {
		if (++rptr >= end_ptr) {
			rptr = state;
		}
	}
	return i;
}

void seed_random(unsigned int x)
{
	register int i;

	state[0] = x;
	for (i = 1; i < 31; i++) {
		state[i] = 1103515245 * state[i - 1] + 12345;
	}
	fptr = &state[3];
	rptr = &state[0];
	for (i = 0; i < 310; i++) {
		(void)get_random();
	}
}
