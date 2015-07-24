#ifndef UI_H
#define UI_H

#include "object.h"

/* User interface interface */

struct ui_message
{
  char *string;
  int x,y;
  int align;  /* ALIGN_CENTER/LEFT/RIGHT */
  font_t *font;
  unsigned int remove_at;
};

void ui_setup(void);
void ui_free(void);
void ui_draw(void);

extern font_t *big_font;
extern font_t *small_font;
extern font_t *tag_font;
extern font_t *menu_font;

/* Write message aligned at (x,y), to be removed in ttl
   milliseconds. There might be a maximum nr of possible simultaneous
   messages. Position is in level coordinates.*/
struct ui_message *
ui_message(const char *message, int x, int y, int align, font_t *font, unsigned int ttl);

#endif
