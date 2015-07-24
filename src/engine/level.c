#include "engine.h"

struct level *current_level = 0;

static int level_name_shell_fun(int argc, const char *argv[])
{
  if (argc != 1)
    {
      shell_print("Usage: level_name NAME\n");
      shell_print("Set level name.\n");
    }
  else
    {
      current_level->name = xstrdup(argv[0]);
      SDL_WM_SetCaption(current_level->name,0);
    }
  return 0;
}

static int level_description_shell_fun(int argc, const char *argv[])
{
  if (argc != 1)
    {
      shell_print("Usage: level_description DESCRIPTION\n");
      shell_print("Set level description.\n");
    }
  else
    {
      current_level->description = xstrdup(argv[0]);
    }
  return 0;
}

static int gravity_shell_fun(int argc, const char *argv[])
{
  float g;
  if (argc != 1 || sscanf(argv[0],"%f",&g) != 1)
    {
      shell_print("Usage: gravity NUM\n");
      shell_print("Set gravity of current level. Current value: %f\n",
		  current_level->gravity);
    }
  else
    {
      current_level->gravity = g;
    }
  return 0;
}

static int bounce_shell_fun(int argc, const char *argv[])
{
  float g;
  if (argc != 1 || sscanf(argv[0],"%f",&g) != 1)
    {
      shell_print("Usage: bounce NUM\n");
      shell_print("Set bounce of current level. Current value: %f\n",
		  current_level->bounce);
    }
  else
    {
      current_level->bounce = g;
    }
  return 0;
}

static int ground_friction_shell_fun(int argc, const char *argv[])
{
  float g;
  if (argc != 1 || sscanf(argv[0],"%f",&g) != 1)
    {
      shell_print("Usage: ground_friction NUM\n");
      shell_print("Set ground_friction of current level. Current value: %f\n",
		  current_level->ground_friction);
    }
  else
    {
      current_level->ground_friction = g;
    }
  return 0;
}

static int tags_shell_fun(int argc, const char *argv[])
{
  if (argc > 0)
    {
      shell_print("Usage: tags\n");
      shell_print("List currently defined tags and sprite types\n");
    }
  else
    {
      int i;
      shell_print("Currently defined sprite tags:\n");
      for (i=0;i<sprite_tags->len;i++)
	{
	  sprite_t *s = obj_lock(sprite_tags->def[i].value,&sprite_type);
	  if (s)
	    {
	      shell_print("%s\t (%s)\n",sprite_tags->def[i].name,obj_type(s)->name);
	      obj_unlock(s);
	    }
	}
    }
  return 0;
}

unsigned int level_state_get(const char **reason)
{
  assert(current_level);
  *reason = current_level->reason;
  return current_level->state;
}

void level_state_set(unsigned int state, const char *reason)
{
  assert(current_level);
  current_level->state = state;
  if (reason)
  {
    if (current_level->reason)
      free(current_level->reason);
    current_level->reason = xstrdup(reason);
  }
}


void level_load(const char *path)
{
  struct level *level;
  char *bg_path,*mask_path,*settings_path;
  FILE *settings;

  if (current_level)
    level_unload();

  obj_type_clear_counts();

  level = xcalloc(1, sizeof(*level));
  level->path = path;
  bg_path = strjoin3("levels/",level->path,"/bg.jpg");
  mask_path = strjoin3("levels/",level->path,"/mask.png");
  settings_path = strjoin3("levels/",level->path,"/settings");
  level->name = "Untitled Level";
  level->gravity = 0.05;
  level->bounce = 0;
  level->ground_friction = 0.03;

  current_level = level;

  settings = fopen(settings_path,"r");
  if (!settings) {
    fprintf(stderr,"Error: level (file '%s') not found!\n", settings_path);
    exit(EXIT_FAILURE);
  }
  shell_eval_file(settings);
  fclose(settings);
  
  fprintf(stderr,"Loading level %s\n",level->name);

  level->bg_image = image_load(bg_path,mask_path); 
  level->bg_sprite.image = level->bg_image;
  level->bg_sprite.x = 0;
  level->bg_sprite.y = 0;
  level->width = level->bg_image->img->w;
  level->height = level->bg_image->img->h;

  free(bg_path);
  free(mask_path);    
  free(settings_path);

  level->state = LEVEL_STATE_INPROGRESS;
  
  if (level->init)
    level->init(level);
  display_set_background(level->bg_image->img);
}

void level_unload(void) /* Unloads current level, if there is one */
{
  if (current_level)
    {
      /* Kill all sprites */
      objlist_killall(mech_group);
      objlist_killall(bullet_group);
      objlist_killall(effects_group);
      objlist_killall(foreground_group);
      objlist_killall(ui_group);

      /* Kill all triggers/generators */
      objlist_killall(generator_group);
      objlist_killall(trigger_group);

      if (current_level->reason)
	free(current_level->reason);
      image_free(current_level->bg_image);
      if (current_level->free)
	current_level->free(current_level);
      else
	free(current_level);
      current_level = NULL;

      /* clear other level specific options */
      bonuses_clear();
      players_clear();
      
      objdict_clear(sprite_tags);
      objdict_clear(trigger_tags);
    }
}

void level_setup(void)
{
  shell_defun("level_name",level_name_shell_fun);
  shell_defun("level_description",level_description_shell_fun);
  shell_defun("gravity",gravity_shell_fun);
  shell_defun("bounce",bounce_shell_fun);
  shell_defun("tags",tags_shell_fun);
  shell_defun("ground_friction",ground_friction_shell_fun);
}
