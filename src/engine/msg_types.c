#include "msg_types.h"
#include "string.h"

typedef struct msg_name_s
{
  enum msg_type type;
  const char * const name;
} msg_name_t;

/* every message type should get a name here */
static const msg_name_t msg_names[] =
{
  { MSG_INVALID, "MSG_INVALID" },
  { MSG_IGNORE, "MSG_IGNORE" },
  { MSG_UPDATE, "MSG_UPDATE" },
  { MSG_KILL, "MSG_KILL" },
  { MSG_SELF_DESTRUCT, "MSG_SELF_DESTRUCT" },
  { MSG_DEACTIVATE, "MSG_DEACTIVATE" },
  { MSG_ACTIVATE, "MSG_ACTIVATE" },
  { MSG_CAN_HAVE_AI, "MSG_CAN_HAVE_AI" },
  { MSG_IS_ACTIVE, "MSG_IS_ACTIVE" },
  { MSG_IS_HARMLESS, "MSG_IS_HARMLESS" },
  { MSG_ACCEPTS_BONUS, "MSG_ACCEPTS_BONUS" },
  { MSG_SET_AI, "MSG_SET_AI" },
  { MSG_SET_BONUS, "MSG_SET_BONUS" },
  { MSG_SET_DAMAGE, "MSG_SET_DAMAGE" },
  { MSG_SET_TARGET, "MSG_SET_TARGET" },
  { MSG_SET_VARIANT, "MSG_SET_VARIANT" },
  { MSG_ACCELERATE, "MSG_ACCELERATE" },
  { MSG_BREAK, "MSG_BREAK" },
  { MSG_UP, "MSG_UP" },
  { MSG_DOWN, "MSG_DOWN" },
  { MSG_FIRE, "MSG_FIRE" },
  { MSG_DROP, "MSG_DROP" },
  { MSG_COPY_AI, "MSG_COPY_AI" },
  { MSG_WARP, "MSG_WARP" },
  { MSG_BGSTUCK, "MSG_BGSTUCK" },
  { MSG_OUTOFBOUNDS, "MSG_OUTOFBOUNDS" },
  { MSG_ENDOFTYPES, "MSG_ENDOFTYPES" },
  { 0, NULL }
};

enum msg_type str_to_msg_type(const char *type_name)
{
  int i;

  for(i=0; msg_names[i].name; i++) {
    if(strcmp(msg_names[i].name, type_name) == 0) {
      return msg_names[i].type;
    }
  }
  return MSG_INVALID;
}

const char *msg_type_name(enum msg_type type)
{
  int i;

  for(i=0; msg_names[i].name; i++) {
    if(msg_names[i].type == type) {
      return msg_names[i].name;
    }
  }
  return "unknown message type";
}
