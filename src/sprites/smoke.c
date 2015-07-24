#include "engine.h"

static animation_t *blacksmokea;

static void killme(void *s)
{
  sprite_kill(s);
}

static int setup(void)
{
  blacksmokea = rc_load_animation("black_smoke");
  animation_last_frame(blacksmokea)->trigger = killme;
  return 0;
}

static sprite_t *create(void)
{
  sprite_t *s;
  s = obj_alloc(sizeof(sprite_t), &blacksmoke.object_type);
  s->animation = blacksmokea;
  s->vel = vector(0,-0.2);
  return s;
}

sprite_type_t blacksmoke =
  {
    .object_type = {
      .name = "blacksmoke",
      .base_type = &sprite_type,
    },
    .default_group = &effects_group,
    .setup = setup,
    .create_sprite = create
  };

REGISTER_SPRITE_TYPE(blacksmoke)
