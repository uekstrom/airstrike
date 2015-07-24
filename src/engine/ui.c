#include <string.h>
#include "engine.h"
#include "list.h"

/* User interface interface */

font_t *big_font;
font_t *small_font;
font_t *tag_font;
font_t *menu_font;
list_t *messages; 


void ui_setup(void)
{
  static const char *ascii = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{|}~ ";
  /* Use a smaller character set to save some memory on the big font. */
  static const char *small_set = "!0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
  big_font = font_load("graphics/fonts/bigfont.png",ascii,0,0);
  small_font = font_load("graphics/fonts/smallfont-green.png",ascii,0,0);
  tag_font = font_load("graphics/fonts/smallfont-red.png",ascii,0,0);
  menu_font = font_load("graphics/fonts/titlefont.png",small_set,-10,0);
  messages = list_new();
}

void ui_free(void)
{
  font_free(menu_font);
  font_free(tag_font);
  font_free(small_font);
  font_free(big_font);
}

void ui_draw(void)
{
  int i, removed = 0;
  unsigned int now; 
  struct ui_message *m;
  now = SDL_GetTicks(); /* Remove all too old messages */
  for (i=0;i<messages->size;i++)
    {
      m = messages->items[i];
      if (now > m->remove_at)
	{
	  free(m->string);
	  free(m);
	  messages->items[i] = 0;
	  removed = 1;
	}
      else
	{
	  display_text(0,m->font,m->x,m->y,
		       m->align,ALIGN_TOP,
		       m->string);
	}
    }
  if (removed)
    list_clean(messages);
}

struct ui_message *
ui_message(const char *message, int x, int y, int align, font_t *font, unsigned int ttl)
{
  struct ui_message *m = xalloc(sizeof(*m));
  m->string = strdup(message);
  m->x = x;
  m->y = y;
  m->align = align;
  m->font = font;
  /* convert ttl to 1/10 secs like sprite ALARMs */
  m->remove_at = SDL_GetTicks() + ttl*100;
  list_insert(messages,m);  
  return m;
}

