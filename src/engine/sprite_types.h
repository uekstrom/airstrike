#ifndef SPRITE_TYPES
#define SPRITE_TYPES

#include <assert.h>
#include "engine.h"
#include "sprites_autogen.h"

/* 
 * This macro should be used at file scope to register each sprite type
 * The argument is the name of the struct itself, not a pointer. No 
 * semicolon required
 */
#define REGISTER_SPRITE_TYPE(t)

/* Call the setup function of each registered type */
int sprite_types_setup(void);

/* return sprite type with given name */
sprite_type_t *get_sprite_type(const char *type_name);

/* Call to clear sprite statistics etc. before new level is loaded */
void sprite_types_info_clear(void);

/* Utility functions used by the sprite types: */

sprite_t *create_effect(sprite_type_t *type, vector_t pos);

/* output given message centered to the sprite */
void sprite_say(sprite_t *s, const char *message);

enum msgret minimal_message(sprite_t *s, msg_t msg);

enum sprite_states /* Different states the sprite might be in */
  {
    SPRITE_ACCELERATING = 1,
    SPRITE_CRASHING = 2,
    SPRITE_UP = 4,
    SPRITE_DOWN = 8,
  };


#endif
