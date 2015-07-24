#include <SDL.h>
#include <stdio.h>
#include "joystick.h"
#include "xalloc.h"

static SDL_Joystick **joystick = NULL; /* array to save pointers to joysticks */
static int number_of_joysticks = 0;    /* number of joysticks found by SDL */

/* initialize joystick subsystem */
void joystick_setup(void)
{
  /* initialize joystick subsystem of SDL */
  if(!SDL_WasInit(SDL_INIT_JOYSTICK)) {
    if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0) {
      number_of_joysticks = 0;
      fprintf(stderr, "error initializing SDL joystick subsystem\n");
      return;
    }
  }

  /* disable SDL joystick events if enabled */
  /* caution: there are two SDL functions to change joystick event state:
   * SDL_JoystickEventState() and SDL_EventState() */
  if(SDL_JoystickEventState(SDL_QUERY)) {
#ifdef DEBUG_JOYSTICK
    fprintf(stderr, "joystick_setup(): disabling joystick events\n");
#endif
    if(SDL_JoystickEventState(SDL_IGNORE) != SDL_IGNORE) {
#ifdef DEBUG_JOYSTICK
      fprintf(stderr, "joystick_setup(): could not disable SDL joystick"
                      " events\n");
#endif
    }
  }

  /* allocate memory for joystick-pointers */
  number_of_joysticks = SDL_NumJoysticks();
  if (number_of_joysticks > 0)
    joystick = xcalloc(number_of_joysticks, sizeof(SDL_Joystick *));
  else
    joystick = 0;
#ifdef DEBUG_JOYSTICK
  fprintf(stderr, "joystick_setup(): %d joysticks found\n",number_of_joysticks);
#endif
}

/* open joystick */
/* returns -1 on error, the (unchanged) joystick number (>=0) on success */
int open_joystick(int joy_no)
{
  if((joy_no > -1) && (joy_no < number_of_joysticks)) {
    if(SDL_JoystickOpened(joy_no)) {
#ifdef DEBUG_JOYSTICK
      fprintf(stderr, "open_joystick(): joystick %d already open\n", joy_no);
#endif
      return joy_no;
    } else {
      if((joystick) && !(joystick[joy_no] = SDL_JoystickOpen(joy_no))) {
       fprintf(stderr, "open_joystick(): could not open joystick %d\n", joy_no);
       return -1;
      } else {
#ifdef DEBUG_JOYSTICK
       fprintf(stderr, "open_joystick(): opened joystick %d\n", joy_no);
#endif
       return joy_no;
      }
    }
  } else {
    if(joy_no == -1) {
#ifdef DEBUG_JOYSTICK
      fprintf(stderr, "open_joystick(): joystick disabled");
#endif
    } else {
      fprintf(stderr, "open_joystick(): there is no joystick %d\n", joy_no);
    }
    return -1;
  }
}

/* close joystick */
void close_joystick(int joy_no)
{
  if((joy_no < 0) || (joy_no >= number_of_joysticks)) {
#ifdef DEBUG_JOYSTICK
    fprintf(stderr, "close_joystick(): trying to close non-existing"
                    " joystick %d\n", joy_no);
#endif
    return;
  }

  if(joystick && joystick[joy_no] && SDL_JoystickOpened(joy_no)) {
    SDL_JoystickClose(joystick[joy_no]);
#ifdef DEBUG_JOYSTICK
    fprintf(stderr, "close_joystick(): closed joystick %d\n", joy_no);
#endif
  } else {
#ifdef DEBUG_JOYSTICK
    fprintf(stderr, "close_joystick(): joystick %d not opened\n"
                  , joy_no);
#endif
  }
}

/* close all open joysticks */
void close_joysticks(void)
{
  int joy_no;

  if(!joystick) {
#ifdef DEBUG_JOYSTICK
    fprintf(stderr,"close_joysticks(): no joysticks opened\n");
#endif
    return;
  }

#ifdef DEBUG_JOYSTICK
  fprintf(stderr, "close_joysticks(): closing all joysticks...\n");
#endif
  for(joy_no=0; joy_no<number_of_joysticks; joy_no++) {
    close_joystick(joy_no);
  }
}

/* update joystick states */
void update_joysticks(void)
{
  SDL_JoystickUpdate();
}

/* query status of joystick */
int joy_active(int joy_no, int actuator, int threshold)
{
  /* only open joysticks need to be queried */
  if((joy_no < 0) || (joy_no >= number_of_joysticks) ||
     (!joystick)  || (!joystick[joy_no])) {
    return 0;
  }

  /* check the joystick component crresponding to actuator */
  switch(actuator) {
    case JOY_AXIS0_POS:
      return ((SDL_JoystickGetAxis(joystick[joy_no], 0) / threshold) > 0);
    case JOY_AXIS0_NEG:
      return (-(SDL_JoystickGetAxis(joystick[joy_no], 0) / threshold) > 0);
    case JOY_AXIS1_POS:
      return ((SDL_JoystickGetAxis(joystick[joy_no], 1) / threshold) > 0);
    case JOY_AXIS1_NEG:
      return (-(SDL_JoystickGetAxis(joystick[joy_no], 1) / threshold) > 0);
    case JOY_AXIS2_POS:
      return ((SDL_JoystickGetAxis(joystick[joy_no], 2) / threshold) > 0);
    case JOY_AXIS2_NEG:
      return (-(SDL_JoystickGetAxis(joystick[joy_no], 2) / threshold) > 0);
    case JOY_AXIS3_POS:
      return ((SDL_JoystickGetAxis(joystick[joy_no], 3) / threshold) > 0);
    case JOY_AXIS3_NEG:
      return (-(SDL_JoystickGetAxis(joystick[joy_no], 3) / threshold) > 0);
    case JOY_AXIS4_POS:
      return ((SDL_JoystickGetAxis(joystick[joy_no], 4) / threshold) > 0);
    case JOY_AXIS4_NEG:
      return (-(SDL_JoystickGetAxis(joystick[joy_no], 4) / threshold) > 0);
    case JOY_AXIS5_POS:
      return ((SDL_JoystickGetAxis(joystick[joy_no], 5) / threshold) > 0);
    case JOY_AXIS5_NEG:
      return (-(SDL_JoystickGetAxis(joystick[joy_no], 5) / threshold) > 0);
    case JOY_BUTTON0:
      return SDL_JoystickGetButton(joystick[joy_no], 0);
    case JOY_BUTTON1:
      return SDL_JoystickGetButton(joystick[joy_no], 1);
    case JOY_BUTTON2:
      return SDL_JoystickGetButton(joystick[joy_no], 2);
    case JOY_BUTTON3:
      return SDL_JoystickGetButton(joystick[joy_no], 3);
    case JOY_BUTTON4:
      return SDL_JoystickGetButton(joystick[joy_no], 4);
    case JOY_BUTTON5:
      return SDL_JoystickGetButton(joystick[joy_no], 5);
    case JOY_BUTTON6:
      return SDL_JoystickGetButton(joystick[joy_no], 6);
    case JOY_BUTTON7:
      return SDL_JoystickGetButton(joystick[joy_no], 7);
    case JOY_BUTTON8:
      return SDL_JoystickGetButton(joystick[joy_no], 8);
    case JOY_BUTTON9:
      return SDL_JoystickGetButton(joystick[joy_no], 9);
    case JOY_BUTTON10:
      return SDL_JoystickGetButton(joystick[joy_no], 10);
    case JOY_BUTTON11:
      return SDL_JoystickGetButton(joystick[joy_no], 11);
    case JOY_BUTTON12:
      return SDL_JoystickGetButton(joystick[joy_no], 12);
    case JOY_BUTTON13:
      return SDL_JoystickGetButton(joystick[joy_no], 13);
    case JOY_BUTTON14:
      return SDL_JoystickGetButton(joystick[joy_no], 14);
    case JOY_BUTTON15:
      return SDL_JoystickGetButton(joystick[joy_no], 15);

    /* if an unknown actuator is queried it is assumed to be not active */
    default:
      return 0;
  }
}

/* stop joystick subsystem */
void joystick_stop(void)
{
  if(SDL_WasInit(SDL_INIT_JOYSTICK)) {
    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
  }
  if(joystick) {
    free(joystick);
  }
#ifdef DEBUG_JOYSTICK
  fprintf(stderr, "joystick_stop(): SDL joystick subsystem stopped\n");
#endif
}
