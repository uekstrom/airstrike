#include "sprite_types.h"

static animation_t *anim;

static int setup(void)
{
  anim = rc_load_animation("mark");
  return 0;
}

static sprite_t *create(void)
{
  sprite_t *s = obj_alloc(sizeof(*s),&mark.object_type);
  s->animation = anim;
  s->anim_p = 0;
  return s;
}

sprite_type_t mark =
  {
    .object_type = {
      .name = "mark",
      .base_type = &sprite_type,
    },
    .default_group = &ui_group,
    .setup = setup,
    .create_sprite = create,
  };

REGISTER_SPRITE_TYPE(mark)
