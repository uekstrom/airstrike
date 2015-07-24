#ifdef _MSC_VER
/* Microsoft Visual Studio stuff */
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_mixer.lib")
#pragma comment(lib, "SDL_image.lib")
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compat.h"  /* unistd.h stuff: chdir() etc */
#include "engine.h"
#include "shell.h"

#define DEFAULT_ROOT "../share"


static void process_events(void)
{
  SDL_Event event;
  while (SDL_PollEvent(&event) >= 1) 
    {
      switch (event.type) 
	{
	case SDL_QUIT: 
	  exit(EXIT_SUCCESS);
	  break;
	default:
	  break;
	}
    }
}

static void level_end(void)
{
  const char *msg, *reason;
  unsigned int idx;

  /* needs to be checked before level is unloaded */
  switch (level_state_get(&reason)) {
  case LEVEL_STATE_SKIP:
	msg = "LEVEL SKIPPED...";
	break;
  case LEVEL_STATE_DONE:
	msg = "LEVEL DONE!";
	break;
  case LEVEL_STATE_FAIL:
	msg = "*LEVEL FAILED*";
	break;
  case LEVEL_STATE_RESTART:
	msg = "LEVEL RESTARTED";
	break;
  default:
	msg = "LEVEL STATE ERROR";
  }
  ui_message(msg,400,200,ALIGN_CENTER,menu_font,20);
  if (reason) {
    ui_message(reason,400,300,ALIGN_CENTER,big_font,20); 
  }
  /* wait 2s */
  for (idx = 0; idx < delay_to_frames(20); idx++) {
    /* update screen while waiting */
    draw_frame();
    game_frame();      
    obj_check_refs();
  }
  level_unload();
  obj_print_stats();
}

static void level_start(const char *levelname)
{
  level_load(levelname);
  ui_message(current_level->name,10,10,ALIGN_LEFT,big_font,20);
  if (current_level->description) {
    ui_message(current_level->description,400,300,ALIGN_CENTER,big_font,20); 
  }
}

static void usage(const char *bin, const char *opt, const char *error)
{
  if (error)
    fprintf(stderr,"Error with option '%s': %s\n", opt, error);

  fprintf(stderr,"\nUsage: %s [options]\n", bin);

  fprintf(stderr,"\nOptions:\n");
  fprintf(stderr,"-level <#level>\t\tStart on given level (number, 0-9)\n");
  fprintf(stderr,"-ai <type> <name>\tSet player AI type and name\n");

  fprintf(stderr,"\nExamples:\n");
  fprintf(stderr,"  %s\n", bin);
  fprintf(stderr,"  %s -level 1 -ai player1 'Human' -ai koala 'Computer'\n", bin);
  exit(EXIT_FAILURE);
}

int main(int argc, const char *argv[])
{
  int shutdown_game, draw, arg = 0, levelstate;
  const char *dummy, *dir, *root = DEFAULT_ROOT;
  char *levelname = strdup("0");

  if (chdir(root) != 0) {
    /* try the directory without the preceding path */
    dir = root + strlen(root);
    while (dir >= root && *dir != '/')
       dir--;
    dir++;
    if (dir == root || chdir(dir) != 0) {
      fprintf(stderr,"Can't chdir() to '%s' or '%s', quitting.\n", root, dir);
      return EXIT_FAILURE;
    }
  }
  engine_setup("defaults");

  while (++arg < argc) {
      if (argv[arg][0] == '-') {
	  if (strcmp(argv[arg], "-level") == 0) {
	      if (arg+1 < argc) {
		  levelname = strdup(argv[++arg]);
		  if (levelname[1] || *levelname < '0' || *levelname > '9')
		    usage(argv[0], argv[arg], "Invalid level number");
		  continue;
	      }
	      usage(argv[0], argv[arg], "Level argument missing");
	  }
	  if (strcmp(argv[arg], "-ai") == 0) {
	      if (arg+2 < argc) {
                  if (player_set_new(argv[arg+1], argv[arg+2]) == 0) {
                      arg += 2;
                      continue;
                  } else {
			  usage(argv[0], argv[arg], NULL);
		  }
	      }
	      usage(argv[0], argv[arg], "AI arguments missing");
	  }
      }
      /* TODO: handle rest of arguments (see doc/airstrike.6) */
      usage(argv[0], argv[arg], "Unknown option");
  }

  printf("\nIn-game keyboard shortcuts:\n");
  printf("  F5  -- Restart level\n");
  printf("  F4  -- Next level (for testing purposes)\n");
  printf("  F1  -- Internal game console\n");
  printf("  ESC -- Exit game\n\n");

  printf("Player 1:\n");
  printf("  Up            -- Left arrow\n");
  printf("  Down          -- Right arrow\n");
  printf("  Accelerate    -- Up arrow\n");
  printf("  Fire bullet   -- Right Shift\n");
  printf("  Drop bomb     -- Right Ctrl\n");
  printf("  Self-destruct -- Backspace\n\n");

  printf("Player 2:\n");
  printf("  Up            -- x\n");
  printf("  Down          -- c\n");
  printf("  Accelerate    -- q\n");
  printf("  Fire bullet   -- Left Alt\n");
  printf("  Drop bomb     -- TAB\n");
  printf("  Self-destruct -- s\n\n");

  printf("Note unless you give two -ai options, the game starts in single\n"
	 "player mode. I.e. only the first plane is user controllable.\n\n");
	
  /* Clean up at exit */
  atexit(display_close);

  level_start(levelname);

  /* Main loop */
  printf("entering main loop\n");

  ui_message("WELCOME TO AIRSTRIKE",400,200,ALIGN_CENTER,menu_font,50);

  shutdown_game = 0;
  draw = 1;
  while (!shutdown_game)
    {
      if (draw)
	draw_frame();
      else {
	display_waitframe();
	process_events();
      }
      game_frame();      
      obj_check_refs();

      if (key_pressed(SDLK_ESCAPE)) {
	shutdown_game = 1;
        continue;
      }
      if (key_pressed(SDLK_F1)) {
	shell_print("Entering shell..\n");
	shell_eval_file(stdin);
	shell_print("Returning to game..\n");
        continue;
      }
      if (key_pressed(SDLK_F5)) {
	/* level restarted */
	level_state_set(LEVEL_STATE_RESTART, "User restarted...");
	level_end();
	level_start(levelname);
        continue;
      }
      levelstate = level_state_get(&dummy);
      if (levelstate == LEVEL_STATE_RESTART) {
	level_end();
	level_start(levelname);
        continue;
      }
      if (levelname[0] < '9') {
	  /* advance to next level? */
	if (key_pressed(SDLK_F4)) {
	  level_state_set(LEVEL_STATE_SKIP, "User skipped...");
	  level_end();
	  levelname[0]++;
	  level_start(levelname);
          continue;
	} else if (levelstate != LEVEL_STATE_INPROGRESS) {
	  level_end();
	  levelname[0]++;
	  level_start(levelname);
	  continue;
	}
      }
      if (key_pressed(SDLK_F8)) {
	draw *= 0;
        continue;
      }
    }  
  
  engine_shutdown();
  obj_print_stats();

  return EXIT_SUCCESS;
}
