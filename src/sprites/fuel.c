/* 
 * (w) 2005 by Eero Tamminen
 */
#include "engine.h"

static animation_t *anim;


static int setup(void)
{
  anim = rc_load_animation("fuel");
  animation_make_loop(anim);
  return 0;
}

static sprite_t *create(void)
{
  sprite_t *s;
  s = obj_alloc(sizeof(sprite_t), &fuel.object_type);
  s->animation = anim;
  return s;
}

static void collide(struct sprite *this_sprite, 
		    struct sprite *other_sprite, 
		    int x, int y)
{
  /* ask catcher to fill up on fuel */
  if (sprite_msg(other_sprite, msg_set_bonus(BONUS_FUEL)) == MSG_RET_ACK) {
    /* and if it does, kill myself after that */
    sprite_kill(this_sprite);
  }
}

sprite_type_t fuel =
  {
    .object_type = {
      .name = "fuel",
      .base_type = &sprite_type,
    },
    .default_group = &bullet_group,
    .setup = setup,
    .create_sprite = create,
    .collide = collide
  };

REGISTER_SPRITE_TYPE(fuel)
