#include "engine.h"

objlist_t *ui_group;  /* TODO: only mark object is using this, is this really needed? */
objlist_t *mech_group;
objlist_t *bullet_group;
objlist_t *effects_group;
objlist_t *foreground_group;
objlist_t *generator_group;
objlist_t *trigger_group;

objdict_t *sprite_tags;
objdict_t *trigger_tags;

static int display_shell_fun(int argc, const char *argv[])
{
  int w,h;
  if (argc == 2 && sscanf(argv[0],"%i",&w) + sscanf(argv[1],"%i",&h) == 2)
    {
      display_open(w,h);
      return 0;
    }
  else
    {
      shell_print("Usage: display WIDTH HEIGHT\n");
      shell_print("Sets display resolution.\n");
      if (argc == 0)
	return 0;
      else
	return -1;
    }
}

static int gcstats_shell_fun(int argc, const char *argv[])
{
  obj_print_stats();
  return 0;
}

static int groups_shell_fun(int argc, const char *argv[])
{
  shell_print("ui_group: %i\n",ui_group->size);
  shell_print("mech_group: %i\n",mech_group->size);
  shell_print("bullet_group: %i\n",bullet_group->size);
  shell_print("effects_group: %i\n",effects_group->size);
  shell_print("foreground_group: %i\n",foreground_group->size);
  shell_print("generator_group: %i\n",generator_group->size);
  shell_print("trigger_group: %i\n",trigger_group->size);
  return 0;
}

static int objects_shell_fun(int argc, const char *argv[])
{
  obj_print_living();
  return 0;
}

void engine_setup(const char *defaults_path)
{
  FILE *f;

  SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO);

  /* don't insert events that will only be ignored into event queue
   * SDL_EventState() cannot use ORed event types :(
   * ignore all events except SDL_ACTIVEEVENT, SDL_QUIT, SDL_SYSWMEVENT,
   * SDL_VIDEORESIZE and SDL_USEREVENT
   * SDL_SYSWMEVENT is ignored by default (at least on my system...) */
  SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);
  SDL_EventState(SDL_KEYUP, SDL_IGNORE);
  SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
  SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
  SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
  SDL_EventState(SDL_JOYAXISMOTION, SDL_IGNORE);
  SDL_EventState(SDL_JOYBALLMOTION, SDL_IGNORE);
  SDL_EventState(SDL_JOYHATMOTION, SDL_IGNORE);
  SDL_EventState(SDL_JOYBUTTONDOWN, SDL_IGNORE);
  SDL_EventState(SDL_JOYBUTTONUP, SDL_IGNORE);

  rc_setup();
  
  maths_setup();

  sprite_setup();
   
  sprite_types_setup();

  joystick_setup();
   
  ai_setup();

  player_setup();
   
  bonuses_setup();
   
  generators_setup();
  triggers_setup();
   
  shell_defun("gcstats",gcstats_shell_fun);
  shell_defun("display",display_shell_fun);
  shell_defun("groups",groups_shell_fun);
  shell_defun("objects",objects_shell_fun);

  if (defaults_path)
    {
      f = fopen(defaults_path,"r");
      if (!f)
	{
	  fprintf(stderr,"engine_setup(): Error, default settings file not found at '%s', quitting\n",defaults_path);
	  exit(EXIT_FAILURE);
	}
      shell_eval_file(f);
      fclose(f);
    }

  /* Now the display should be open so we can use
     SDL display functions */

  ui_setup();
  sound_setup();
  level_setup();

  ui_group = objlist_new();
  mech_group = objlist_new();
  bullet_group = objlist_new();
  effects_group = objlist_new();
  foreground_group = objlist_new();
  generator_group = objlist_new();
  trigger_group = objlist_new();

  sprite_tags = objdict_new();
  trigger_tags = objdict_new();
}

void engine_shutdown(void)
{
  fprintf(stderr,"Shutting down engine...\n");
  objlist_killall(ui_group);
  objlist_delete(ui_group);
  objlist_killall(mech_group);
  objlist_delete(mech_group);
  objlist_killall(bullet_group);
  objlist_delete(bullet_group);
  objlist_killall(effects_group);
  objlist_delete(effects_group);  
  objlist_killall(foreground_group);
  objlist_delete(foreground_group);
  objlist_killall(generator_group);
  objlist_delete(generator_group);
  objlist_killall(trigger_group);
  objlist_delete(trigger_group);

  objdict_delete(trigger_tags);
  objdict_delete(sprite_tags);

  close_joysticks();
  joystick_stop();

  ui_free();
}

void draw_frame(void) /* Draw background, sprites and ui */
{
  display_clear();
  //display_image(current_level->bg_image,0,0);
  sprite_group_draw(mech_group);
  sprite_group_draw(bullet_group);
  sprite_group_draw(effects_group);
  sprite_group_draw(foreground_group);
  sprite_group_draw(ui_group);
  ui_draw();
  display_update();
}

void game_frame(void) /* Advance the game one tick */
{
  schedule_advance(0,SCHED_EARLY);

  objlist_clean(trigger_group);
  triggers_update(trigger_group);

  objlist_clean(generator_group);
  generators_update(generator_group);

  sprite_group_move(mech_group);
  sprite_group_move(bullet_group);
  sprite_group_move(effects_group);
  sprite_group_move(foreground_group);
  sprite_group_move(ui_group);

  sprite_group_animate(mech_group, display.frame_time);
  sprite_group_animate(bullet_group, display.frame_time);
  sprite_group_animate(effects_group, display.frame_time);
  sprite_group_animate(foreground_group, display.frame_time);
  sprite_group_animate(ui_group, display.frame_time);
  
  schedule_advance(0,SCHED_NORMAL);

  sprite_group_update(mech_group);
  sprite_group_update(bullet_group);
  sprite_group_update(effects_group);
  sprite_group_update(foreground_group);
  sprite_group_update(ui_group);

  objlist_clean(bullet_group);
  objlist_clean(mech_group);
  objlist_clean(effects_group);
  objlist_clean(foreground_group);
  objlist_clean(ui_group);

  objdict_clean(sprite_tags);

  sprite_group_pos_update(mech_group);
  sprite_group_pos_update(bullet_group);

  sprite_group_coll(mech_group,mech_sprite_collide);
  sprite_group_pos_update(mech_group);
  sprite_group_bg_coll(mech_group,mech_sprite_bg_collide);
  sprite_group_pos_update(mech_group);
  sprite_group_coll2(bullet_group,mech_group,0);
  sprite_group_bg_coll(bullet_group,0);

  objlist_clean(bullet_group);
  objlist_clean(mech_group);
  objlist_clean(effects_group);
  objlist_clean(foreground_group);
  objlist_clean(ui_group);

  schedule_advance(1,SCHED_FIRST);
}

