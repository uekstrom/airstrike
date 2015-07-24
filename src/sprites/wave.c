#include "engine.h"

static animation_t *anim;

static int setup(void)
{
  anim = rc_load_animation("waves");
  return 0;
}

static sprite_t *create(void)
{
  sprite_t *s;
  s = obj_alloc(sizeof(sprite_t), &wave.object_type);
  s->animation = anim;
  return s;
}

sprite_type_t wave =
  {
    .object_type = {
      .name = "wave",
      .base_type = &sprite_type,
    },
    .default_group = &foreground_group,
    .setup = setup,
    .create_sprite = create
  };

REGISTER_SPRITE_TYPE(wave)
