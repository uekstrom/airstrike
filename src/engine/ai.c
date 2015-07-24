/*
 * AI types and mapping from each "player" to his AI
 * 
 * AI types are:
 * - player1 (set of keys)
 * - player2 (set of keys)
 * - koala
 * - seeker (for missiles, always accelerates and turns toward target)
 * 
 */
#include <string.h>
#include "engine.h"

object_type_t ai_type = 
  {
    .name = "ai",
  };

/* ======= search and destroy ai ======== */

#define COS_NOTURN_ANGLE 0.0	/* don't turn around if target is behind */
#define COS_NOVIEW_ANGLE 0.5	/* lose target when more than 60 deg off */
#define AIM_EPS 0.2		/* fly straight if less than AIM_EPS off */

struct seeker_ai
{
  ai_t ai;
  sprite_t *target;	/* AI is trying to hit this sprite */
};

static enum msgret seeker_message(ai_t *ai, sprite_t *control, msg_t message)
{
  struct seeker_ai *sai = (struct seeker_ai *)ai;
  switch (message.type)
  {
    case MSG_UPDATE:		/* follow target */

      /* always accelerate */
      obj_message(control,msg_ctrl(MSG_ACCELERATE));

      /* turn towards target */
      if(sai->target && obj_alive(sai->target)) {
        vector_t n_to_target, n_vel, adjust={0.0, 0.0}, adj_target;
        float cos_theta;	/* cosine of angle(n_to_target, n_vel) */
	float aim_error;

        /* adjust target position to use warping */
        /* (this does not take relative velocity into account) */
        n_to_target = vnormalize(vsub(sai->target->pos, control->pos));
        n_vel = vnormalize(control->vel);
        cos_theta = vdot(n_to_target, n_vel);

	/* adjust target position if in blind angle but can be reached by warp*/
        if(cos_theta < COS_NOTURN_ANGLE) {
          adjust = vector(((control->vel.x<0) ? -1 : 1)*current_level->width,0);
        }
        adj_target = vadd(adjust, sai->target->pos);

	/* check if missile can see target */
	n_to_target = vnormalize(vsub(adj_target, control->pos));
	cos_theta = vdot(n_to_target, n_vel);

	/* lose lock when out of viewing angle */
	if(cos_theta < COS_NOVIEW_ANGLE) {
	  sai->target = NULL;
	  obj_message(control, msg_set_target(sai->target));
	} else {

	  /* turn towards the (adjusted) target position */
	  aim_error = vcross(n_vel, n_to_target);
          if(aim_error < -AIM_EPS) {
            obj_message(control,msg_ctrl(MSG_UP));
          } else if(aim_error > AIM_EPS) {
            obj_message(control,msg_ctrl(MSG_DOWN));
          }
	}
      }
      return MSG_RET_CONT;
    case MSG_SET_TARGET:	/* get sprite to kill */
      sai->target = msg_get_target(message);
      return MSG_RET_CONT;
    case MSG_CAN_HAVE_AI:
      return MSG_RET_YES;
    default:			/* ai doesn't know how to handle this message */
      return MSG_RET_CONT;	/* maybe something else knows... */
  }
}

static ai_t *seeker_ai(void)
{
  struct seeker_ai *ai = obj_alloc(sizeof(*ai),&ai_type);
  ai->ai.intercept_msg = seeker_message;
  ai->ai.name = "seeker";
  ai->target = NULL;
  return (ai_t *)ai;
}

/* ======= koala ai (which does nothing) ======== */

static enum msgret koala_message(ai_t *ai, sprite_t *target, msg_t message)
{
  switch (message.type)
    {
    case MSG_CAN_HAVE_AI:
      return MSG_RET_YES;
    default:
      return MSG_RET_CONT;
    }
}

static ai_t *koala_ai(void) //Does absolutely nothing
{
  ai_t *ai = obj_alloc(sizeof(*ai),&ai_type);
  ai->intercept_msg = koala_message;
  ai->name = "koala";
  return ai;
}

/* ========= player 1 & 2 "ai" ========= */

struct player_ai
{
  ai_t ai;
  /* Keybindings */
  int key_accelerate;
  int key_break;
  int key_left;
  int key_right;
  int key_fire;
  int key_drop;
  int key_suicide;
  /* Joystick */
  int joy_no;		/* negative for no joystick */
  int joy_threshold;	/* movement below threshold is ignored */
  int joy_accelerate;
  int joy_break;
  int joy_left;
  int joy_right;
  int joy_fire;
  int joy_drop;
  int joy_suicide;
};

static enum msgret player_message(ai_t *ai, sprite_t *target, msg_t message)
{
  struct player_ai *pai = (struct player_ai *)ai;
  switch (message.type)
    {
    case MSG_UPDATE:
      /* update joystick state just once per update message */
      if(pai->joy_no > -1) {
	update_joysticks();
      }
      if (key_pressed(pai->key_accelerate) ||
	      joy_active(pai->joy_no, pai->joy_accelerate, pai->joy_threshold))
	obj_message(target,msg_ctrl(MSG_ACCELERATE));
      else if (key_pressed(pai->key_break) ||
	      joy_active(pai->joy_no, pai->joy_break, pai->joy_threshold))
	obj_message(target,msg_ctrl(MSG_BREAK));
      
      if (key_pressed(pai->key_right) ||
	      joy_active(pai->joy_no, pai->joy_right, pai->joy_threshold))
	obj_message(target,msg_ctrl(MSG_DOWN));
      else if (key_pressed(pai->key_left) ||
	      joy_active(pai->joy_no, pai->joy_left, pai->joy_threshold))
	obj_message(target,msg_ctrl(MSG_UP));
      
      if (key_pressed(pai->key_fire) ||
	      joy_active(pai->joy_no, pai->joy_fire, pai->joy_threshold))
	obj_message(target,msg_ctrl(MSG_FIRE));
      else if (key_pressed(pai->key_drop) ||
	      joy_active(pai->joy_no, pai->joy_drop, pai->joy_threshold))
	obj_message(target,msg_ctrl(MSG_DROP));

      if (key_pressed(pai->key_suicide) ||
	      joy_active(pai->joy_no, pai->joy_suicide, pai->joy_threshold))
	obj_message(target,msg_short(MSG_SELF_DESTRUCT));

      return MSG_RET_CONT;
    case MSG_CAN_HAVE_AI:
      return MSG_RET_YES;
    default:
      return MSG_RET_CONT;
    }
 }

static ai_t *player_ai(int player_nr)
{
  struct player_ai *ai = obj_alloc(sizeof(*ai),&ai_type);
  conf_t *key_conf; 
  ai->ai.intercept_msg = player_message;
  if (player_nr == 1)
    {
      ai->ai.name = "player1";
      key_conf = conf_section(rc_user_conf(),"player1_control");
      ai->key_accelerate = conf_number(key_conf,"accelerate",SDLK_UP);
      ai->key_break = conf_number(key_conf,"break",SDLK_DOWN);
      ai->key_left = conf_number(key_conf,"left",SDLK_LEFT);
      ai->key_right = conf_number(key_conf,"right",SDLK_RIGHT);
      ai->key_fire = conf_number(key_conf,"fire",SDLK_RSHIFT);
      ai->key_drop = conf_number(key_conf,"drop",SDLK_RCTRL);
      ai->key_suicide = conf_number(key_conf,"suicide",SDLK_BACKSPACE);
    }
  else if (player_nr == 2)
    {
      ai->ai.name = "player2";
      key_conf = conf_section(rc_user_conf(),"player2_control");
      ai->key_accelerate = conf_number(key_conf,"accelerate",SDLK_q);
      ai->key_break = conf_number(key_conf,"break",SDLK_w);
      ai->key_left = conf_number(key_conf,"left",SDLK_x);
      ai->key_right = conf_number(key_conf,"right",SDLK_c);
      ai->key_fire = conf_number(key_conf,"fire",SDLK_LALT);
      ai->key_drop = conf_number(key_conf,"drop",SDLK_TAB);
      ai->key_suicide = conf_number(key_conf,"suicide",SDLK_s);
    }
  else
    {
      key_conf = NULL;
      BUG("Invalid player nr");
    }
  /* use first joystick for player one, second for player two as default */
  ai->joy_no = conf_number(key_conf,"joy_no",player_nr-1);
  /* default joystick configuration is the same for every player */
  ai->joy_threshold = conf_number(key_conf,"joy_threshold",JOY_THRESHOLD);
  ai->joy_accelerate = conf_number(key_conf,"joy_accelerate",JOY_AXIS1_NEG);
  ai->joy_break = conf_number(key_conf,"joy_break",JOY_AXIS1_POS);
  ai->joy_left = conf_number(key_conf,"joy_left",JOY_AXIS0_NEG);
  ai->joy_right = conf_number(key_conf,"joy_right",JOY_AXIS0_POS);
  ai->joy_fire = conf_number(key_conf,"joy_fire",JOY_BUTTON0);
  ai->joy_drop = conf_number(key_conf,"joy_drop",JOY_BUTTON1);
  ai->joy_suicide = conf_number(key_conf,"joy_suicide",JOY_BUTTON3);

  /* try to open joystick and set joy_no to -1 on error */
  ai->joy_no = open_joystick(ai->joy_no);
  return (ai_t *)ai;
}

static ai_t *player1_ai(void)
{
  return player_ai(1);
}

static ai_t *player2_ai(void)
{
  return player_ai(2);
}

/* ========= "ai" name mapping ========= */

/* mapping from AI name to creation function */
typedef struct {
  const char *name;
  ai_create_func_t *create;
} ai_map_t;

static ai_map_t ai_map[] = {
  { "koala", koala_ai },
  { "player1", player1_ai },
  { "player2", player2_ai },
  { "seeker", seeker_ai },
  { NULL, NULL }
};

static void ai_print_types(void)
{
  int i;
  for (i = 0; ai_map[i].name; i++) {
    shell_print("- %s\n", ai_map[i].name);
  }
}

ai_create_func_t *ai_get_func(const char *ai_type_name)
{
  int i;
  for (i = 0; ai_map[i].name; i++) {
    if (strcmp(ai_type_name, ai_map[i].name) == 0) {
      return ai_map[i].create;
    }
  }
  shell_error("Unknown AI name.  Valid names are:\n");
  ai_print_types();
  return NULL;
}


obj_id_t ai_set_sprite_ai(sprite_t *s, const char *ai_type_name)
{
  ai_create_func_t *ai_create;
  ai_t *ai;
  
  assert(ai_type_name && s);

  if (!sprite_ask(s, MSG_CAN_HAVE_AI))
    {
      shell_error("Assigning AI to sprite (%s) without AI support.\n",
                  obj_type(s)->name);
      return 0;
    }

  ai_create = ai_get_func(ai_type_name);
  if (!ai_create)
    {
      return shell_error("No such AI '%s'.\n",ai_type_name);
      return 0;
    }

  ai = obj_ref(ai_create(),&ai_type);
  sprite_msg(s,msg_set_ai(ai));
  obj_deref(ai);

  return obj_id(ai);
}

/* ========= "ai" shell functions and setup ========= */

static int ai_shell_fun(int argc, const char *argv[])
{
  if (argc == 0)
    {      
      shell_print("Usage: ai AI SPRITE_TAG\n");
      shell_print("Set the ai of a sprite. Not all sprite types\n");      
      shell_print("support ai's. Possible ai's are:\n");
      ai_print_types();
      return 0;
    }

  if (argc == 2)
    {
      sprite_t *s = obj_lock(objdict_lookup(sprite_tags,argv[1]),&sprite_type);
      if (s)
	{
	  obj_id_t res = ai_set_sprite_ai(s, argv[0]);
	  obj_unlock(s);
	  if (res)
            return 0;
          else
            return -1;
	}
      return shell_error("No such sprite tag '%s'.\n",argv[1]);
    }
  return shell_error("Invalid arguments");
}

static int ai_target_shell_fun(int argc, const char *argv[])
{
  if (argc == 0)
    {      
      shell_print("Usage: ai_target SPRITE_TAG TARGET_TAG\n");
      shell_print("Set the target of ai in SPRITE_TAG to TARGET_TAG.\n");
      return 0;
    }

  if (argc == 2)
    {
      sprite_t *s = obj_lock(objdict_lookup(sprite_tags,argv[0]),&sprite_type);
      sprite_t *target = obj_lock(objdict_lookup(sprite_tags,argv[1]),
                                  &sprite_type);
      if (s)
	{
	  if (target)	    
	    {
	      if (s != target)
		{
		  sprite_msg(s,msg_set_target(target));
		  obj_unlock(s);
		  obj_unlock(target);
		  return 0;
		}
	      obj_unlock(s);
	      obj_unlock(target);
	      return shell_error("Can't set target to sprite itself.\n");
	    }
	  obj_unlock(s);
	  return shell_error("No such sprite tag '%s'.\n",argv[1]);
	}
      return shell_error("No such sprite tag '%s'.\n",argv[0]);
    }
  return shell_error("Invalid arguments");
}

void ai_setup(void)
{
  obj_type_init(&ai_type);
  shell_defun("ai",ai_shell_fun);
  shell_defun("ai_target",ai_target_shell_fun);
}
