#include "engine.h"

static animation_t *anim;

static int setup(void)
{
  anim = rc_load_animation("fire");
  animation_last_frame(anim)->trigger = sprite_kill;
  return 0;
}

static sprite_t *create(void)
{
  sprite_t *s = obj_alloc(sizeof(*s),&fire.object_type);
  s->animation = anim;
  return s;
}

sprite_type_t fire =
  {
    .object_type = {
      .name = "fire",
      .base_type = &sprite_type,
    },
    .default_group = &effects_group,
    .setup = setup,
    .create_sprite = create,
  };

REGISTER_SPRITE_TYPE(fire)
