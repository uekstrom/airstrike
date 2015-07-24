#include "engine.h"

static animation_t *grow;
static animation_t *dissolve;

static int setup(void)
{
  grow = rc_load_animation("ldust-grow");
  dissolve = rc_load_animation("ldust-dissolve");
  animation_last_frame(grow)->next_frame = dissolve;
  animation_last_frame(dissolve)->trigger = sprite_kill;
  return 0;
}

static sprite_t *create(void)
{
  sprite_t *s;
  s = obj_alloc(sizeof(*s),&ldust.object_type);
  s->animation = grow;
  s->anim_p = 0;
  return s;
}

static void collide(struct sprite *this_sprite, 
		    struct sprite *other_sprite, 
		    int x, int y)
{
  if (this_sprite->state == 0)
    {
      this_sprite->state = 1;  
      sprite_set_animation(this_sprite, dissolve);
    }
}

sprite_type_t ldust =
  {
    .object_type = {
      .name = "ldust",
      .base_type = &sprite_type,
    },
    .default_group = &effects_group,
    .setup = setup,
    .create_sprite = create,
    .collide = collide,
  };

REGISTER_SPRITE_TYPE(ldust)

/* ------------------------------------------------------ */

static animation_t *sgrow;
static animation_t *sdissolve;

static int ssetup(void)
{
  sgrow = rc_load_animation("sdust-grow");
  sdissolve = rc_load_animation("sdust-dissolve");
  animation_last_frame(sgrow)->next_frame = sdissolve;
  animation_last_frame(sdissolve)->trigger = sprite_kill;
  return 0;
}

static sprite_t *screate(void)
{
  sprite_t *s;
  s = obj_alloc(sizeof*s,&sdust.object_type);
  s->animation = sgrow;
  s->anim_p = 0;
  return s;
}

static void scollide(struct sprite *this_sprite, 
		    struct sprite *other_sprite, 
		    int x, int y)
{
  if (this_sprite->state == 0)
    {
      this_sprite->state = 1;  
      sprite_set_animation(this_sprite, sdissolve);
    }
}

sprite_type_t sdust =
  {
    .object_type = {
      .name = "sdust",
      .base_type = &sprite_type,
    },
    .default_group = &effects_group,
    .setup = ssetup,
    .create_sprite = screate,
    .collide = scollide,
  };

REGISTER_SPRITE_TYPE(sdust)

/* ------------------------------------------------------ */

static animation_t *dsmokea;

static int dsetup(void)
{
  dsmokea = rc_load_animation("direxpl");
  animation_last_frame(dsmokea)->delay = 100;
  animation_last_frame(dsmokea)->trigger = sprite_kill;
  return 0;
}

static sprite_t *dcreate(void)
{
  sprite_t *s;
  s = obj_alloc(sizeof*s,&dsmoke.object_type);
  s->animation = dsmokea;
  s->anim_p = 0;
  return s;
}

sprite_type_t dsmoke =
  {
    .object_type = {
      .name = "dsmoke",
      .base_type = &sprite_type,
    },
    .default_group = &effects_group,
    .setup = dsetup,
    .create_sprite = dcreate,
  };

REGISTER_SPRITE_TYPE(dsmoke)
