#include "engine.h"

#define BULLET_DAMAGE 10


static animation_t *anim;

static void killme(void *s)
{
  sprite_kill(s);
}

static int setup(void)
{
  anim = rc_load_animation("bullet");
  animation_last_frame(anim)->trigger = killme;
  return 0;
}

static sprite_t *create(void)
{
  sprite_t *s = obj_alloc(sizeof(*s),&bullet.object_type);
  s->animation = anim;
  s->anim_p = 0;
  return s;
}

static void collide(struct sprite *this_sprite, 
		    struct sprite *other_sprite, 
		    int x, int y)
{
#ifdef TODO
  sprite_t *s;
  float n[2];
  /* Create a smoke effect in the opposite direction*/  
  s = sprite_create(&dsmoke);
  sprite_set_pos(s,x,y);
  sprite_get_vel(this_sprite, n);
  s->anim_p = ((vangle(n) + 128) & 255)/8 ;
  sprite_get_vel(other_sprite,n);
  sprite_set_vel(s,n);
  sprite_group_insert(effects_group,s);
#endif
  sprite_msg(other_sprite,msg_set_damage(BULLET_DAMAGE));
  sprite_kill(this_sprite);
  /* Disable the sound since we need different sounds for different objects */
#ifdef TODO
  sound_effect(&sound_bullethit, this_sprite->pos.x, this_sprite->pos.y);
#endif
}

static void collide_world(struct sprite *this_sprite, int x, int y)
{
#ifdef TODO
  sprite_t *s;
  float n[2];
  /* Create a smoke effect in the opposite direction*/
  s = sprite_create(&dsmoke);
  sprite_set_pos(s,x,y);
  sprite_get_vel(this_sprite, n);
  s->anim_p = ((vangle(n) + 128) & 255)/8 ;
  sprite_group_insert(effects_group,s);  
#endif
  sprite_kill(this_sprite);
}

static enum msgret message(void *s, msg_t msg)
{  
  switch(msg.type)
    {      
    case MSG_OUTOFBOUNDS:
      sprite_kill(s);
      break;
    default:
      return MSG_RET_UNKNOWN;
      break;
    }
  return MSG_RET_ACK;
}

sprite_type_t bullet =
  {
    .object_type = {
      .name = "bullet",
      .base_type = &sprite_type,
      .message = message      
    },
    .default_group = &bullet_group,
    .setup = setup,
    .create_sprite = create,
    .collide = collide,
    .bg_collide = collide_world,
  };

REGISTER_SPRITE_TYPE(bullet)
