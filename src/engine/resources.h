#ifndef RESOURCES_H
#define RESOURCES_H

#include "engine.h"

int rc_setup(void);
animation_t *rc_load_animation(const char *animation_name);
conf_t *rc_user_conf(void);

#endif
