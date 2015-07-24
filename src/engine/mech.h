#ifndef MECHANICS_H
#define MECHANICS_H
#include "sprite.h"
#include "maths.h"

/* The angular resolution of the sprites,
 * in parts of 256 of a whole circle. 4 implies
 * 64 steps.
 */
#define MECH_ANG_DIV 4

extern object_type_t mech_sprite_type;

typedef struct mech_sprite
{
  sprite_t sprite;
  float rmass; /* 1/mass. Used in collisions*/
  float gravity; /*How much it is affected by gravity(for balloons etc)*/
  float rinmom; /* 1/I, rotational inertial moment */

  float bounce; /* Number between 0 and 1 */

  float air_resistance;
  float air_resistance_ang;

  float angle;
  float ang_vel;
  float ang_impulse;
  vector_t lin_impulse; /* dv/timestep */
  unsigned int damage;
} mech_sprite_t;

/* Returns a vector pointing in the heading of sprite s */
static INLINE vector_t mech_heading(mech_sprite_t *s)
{
  return trig((int)s->angle);
}

static INLINE vector_t mech_right(mech_sprite_t *s)
{
  return trig((int)s->angle + 64);
}

/* Calculate the global position of a the local point. Object x axis
   is along mech_heading(), y axix is mech_right() */
vector_t mech_global_pos(mech_sprite_t *s, vector_t local);

/* Set the physical constants to default sane values.
   Note that the appropriate flags should be set, specially
   SPRITE_CANROTATE.
 */
void mech_defaults(mech_sprite_t *s);

void mech_turn_to(mech_sprite_t *s, const float *n, float strength);
/* Returns the velocity of point r on sprite s */
vector_t mech_point_vel(mech_sprite_t *s, const float *p);
void mech_apply_impulse(mech_sprite_t *s, const float *p, vector_t i);
void mech_sprite_collide(sprite_t *this_sprite,
			 sprite_t *other_sprite,
			 int x, int y);
void mech_sprite_bg_collide(sprite_t *s, int x, int y);
void mech_update(mech_sprite_t *s);
void mech_boundary(mech_sprite_t *ms);
/* Make s follow "it" at a distance dist, using a maximum force to accelerate
   TODO: Implement better than proportional control.
*/
void mech_follow(mech_sprite_t *s, sprite_t *it, float dist, float max_force);

/* Make sure that the sprite image reflects s->angle */
void mech_refresh_image(mech_sprite_t *s);

#endif
