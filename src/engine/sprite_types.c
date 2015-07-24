#include <stdio.h>
#include "sprite_types.h"

static dict_t *sprite_types;

#include "sprites_autogen.inc"

static int sprite_shell_fun(int argc, const char *argv[])
{
  float x,y;
  if (argc == 0)
    {
      int i;
      shell_print("Usage: sprite TYPE X Y [tag=TAG]\n");
      shell_print("Create a new sprite at (X,Y) of TYPE [with TAG]\n");
      shell_print("Possible sprite types are:\n");
      for (i=0;i<sprite_types->len;i++)
	shell_print("  %s\n",sprite_types->def[i].name);
      return 0;
    }
  else if (argc >= 3 && sscanf(argv[1],"%f",&x) + sscanf(argv[2],"%f",&y) == 2)
    {
      sprite_t *s;
      sprite_type_t *type = dict_lookup(sprite_types,argv[0]);
      float v;
      const char *tag;
      if (!type)
	{
	  shell_error("unknown sprite type %s\n",argv[0]);
	  return -1;
	}
      s = sprite_create(type);
      s->pos.x = x;
      s->pos.y = y;      
      if (getarg_str(argv,"tag",&tag))
	{
	  if (objdict_lookup(sprite_tags,tag))
	    shell_warning("Sprite tag '%s' already defined.\n",tag);
	  else	    
	    objdict_define(sprite_tags,tag,obj_id(s));
	}
      if (getarg_float(argv,"vx",&v))
	s->vel.x = v;
      if (getarg_float(argv,"vy",&v))
	s->vel.y = v;
      return 0;
    }
  else
    {
      return -1;
    }
}

int sprite_types_setup(void)
{
  shell_defun("sprite",sprite_shell_fun);
  sprite_types = dict_new();
  sprite_types_register();  
  return 0;
}

sprite_type_t *get_sprite_type(const char *type_name)
{
  return dict_lookup(sprite_types, type_name);
}

void sprite_types_info_clear(void)
{
	/* TODO: add sprite type stats */
}

sprite_t *create_effect(sprite_type_t *type, vector_t pos)
{
  sprite_t *s = sprite_create(type);
  s->pos = pos;
  return s;
}

void sprite_say(sprite_t *s, const char *message)
{
  ui_message(message, s->pos.x, s->pos.y, ALIGN_CENTER, tag_font, 25);
}

enum msgret minimal_message(sprite_t *s, msg_t msg)
{
  switch(msg.type)
    {
    case MSG_OUTOFBOUNDS:
    case MSG_KILL:
      obj_kill(s);
      break;
    default:
      return MSG_RET_UNKNOWN;
      break;
    }
  return MSG_RET_ACK;
}

