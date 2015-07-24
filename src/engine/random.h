#ifndef RANDOM_H
#define RANDOM_H

#define RANDOM_MAX 0x7fffffff

/* return pseudo-random value (0 <= x < max) */
long get_random(void);

/* initialize random generator */
void seed_random(unsigned int x);

#endif /* RANDOM_H */
