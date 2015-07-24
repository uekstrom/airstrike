#include "engine.h"
# define CLOUD_OFFSET 100

static animation_t *drift1, *drift2;

static int setup(void)
{
  drift1 = rc_load_animation("cloud-1");
  drift2 = rc_load_animation("cloud-2");
  return 0;
}

static sprite_t *create(void)
{
  sprite_t *s = obj_alloc(sizeof(*s),&cloud.object_type);
  
  switch(get_random() % 2) {
  case 0:
    s->animation = drift1;
    break;
  case 1:
    s-> animation = drift2;
    break;
  }
  return s;
}

static enum msgret message(void *obj, msg_t msg)
{
  sprite_t *s = obj;
  switch (msg.type)
    {
    case MSG_UPDATE:
      if (s->pos.x + 2*CLOUD_OFFSET < 0 ||
	  s->pos.x - 2*CLOUD_OFFSET > current_level->width) 
	/* sprite is off screen, remove it */
	sprite_kill(s);      
      break;
    default:
      return MSG_RET_UNKNOWN;
      break;
    }
  return MSG_RET_ACK;
}

sprite_type_t cloud =
  {
    .object_type = {
      .name = "cloud",
      .message = message,
      .base_type = &sprite_type
    },
    .default_group = &foreground_group,
    .setup = setup,
    .create_sprite = create,
  };

REGISTER_SPRITE_TYPE(cloud)
