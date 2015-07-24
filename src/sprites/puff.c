#include "engine.h"

static animation_t *grow;

static void frame_trigger(void *s)
{
  sprite_kill(s);
}

static int setup(void)
{
  grow = rc_load_animation("white_smoke");
  animation_last_frame(grow)->trigger = frame_trigger;
  return 0;
}

static sprite_t *create(void)
{
  sprite_t *s;
  s = obj_alloc(sizeof(sprite_t), &puff.object_type);
  s->animation = grow;
  s->vel = vector(0,-0.3);
  return s;
}

static void collide(struct sprite *this_sprite, 
		    struct sprite *other_sprite, 
		    int x, int y)
{
  if (this_sprite->state == 0)
    {
      this_sprite->state = 1;  
      /*      sprite_set_animation(this_sprite, dissolve); */
    }
}

sprite_type_t puff =
  {
    .object_type = {
      .name = "puff",
      .base_type = &sprite_type,
    },
    .default_group = &effects_group,
    .setup = setup,
    .create_sprite = create,
    .collide = collide
  };

REGISTER_SPRITE_TYPE(puff)
