#include "resources.h"
#include "conf.h"

static conf_t *conf_root;

static int rc_shell_fun(int argc, const char *argv[])
{
  conf_write(conf_root,stdout);
  return 0;
}

int rc_setup(void)
{
  FILE *f;
  conf_t *conf;
  conf_root = conf_new();

  shell_defun("resource",rc_shell_fun);

  /* Add animation data */
  f = fopen("graphics/sprites/animations.conf","r");
  if (f)
    {
      conf = conf_read(f);
      fclose(f);
    }
  else
    {
      conf = NULL;
    }
  
  if (!conf)
    {
      fprintf(stderr,"rc_setup(): Warning, could not load animation definitions from graphics/sprites/animations.conf.\n");      
      return -1;
    }

  conf_set_section(conf_root,"animations",conf);


  /* Load "user" settings from the defaults file */
  f = fopen("defaults.conf","r");
  if (f)
    {
      conf = conf_read(f);
      fclose(f);
    }
  else
    {
      conf = NULL;
    }
  
  if (!conf)
    {
      fprintf(stderr,"rc_setup(): Warning, could not load user defaults from 'defaults.conf'.\n");      
      conf = conf_new();
    }

  conf_set_section(conf_root,"user",conf);

  return 0;
}

animation_t *rc_load_animation(const char *animation_name)
{
  conf_t *anim_conf = conf_lookup_section(conf_root,"animations",NULL); 
  assert(anim_conf);
  anim_conf = conf_lookup_section(anim_conf,animation_name,NULL);
  if (anim_conf)
    {
      animation_t *anim = animation_load(conf_lookup_string(anim_conf,"image",""),
					 conf_lookup_number(anim_conf,"pframes",1),
					 conf_lookup_number(anim_conf,"tframes",1),
					 conf_lookup_number(anim_conf,"delay",100));
      if (anim && conf_lookup_number(anim_conf,"loop",0))
	animation_make_loop(anim);
      return anim;
    }
  else
    {
      fprintf(stderr,"rc_load_animation(): No such animation '%s'.\n",animation_name);
      return NULL;
    }  
}

conf_t *rc_user_conf(void)
{
  return conf_lookup_section(conf_root,"user",NULL);
}
