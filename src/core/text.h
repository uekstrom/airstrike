#ifndef TEXT_H
#define TEXT_H
#include <image.h>

enum text_alignment
  {
    ALIGN_CENTER,
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_TOP,
    ALIGN_BOTTOM
  };

typedef struct font
{
  int char_h, char_d;
  image_t *chars[256];
} font_t;

/*
  Airstrike font format: The characters are stored in an image file with alpha transparency.
  The letters are separated by 1 or more columns of clear pixels. Since some charactes have 
  vertical holes (for example the " mark in some fonts) the first like of the image can be 
  used to tie the parts together with some nontransparent pixels. This line is not included
  in the final characters. The space character has to be marked in this way.

  The kerning (font->char_d) and line spacing can be changed from their defaults with 
  the kerning and linesep arguments. These can be both negative and positive.
*/

font_t *font_load(const char *path, const char *chars, int kerning, int linesep);

/* frees whats allocate for the font */
void font_free(font_t *font);

int text_setup(void);

/* Draw a sequence of characters on the screen
 * If boundary is nonzero then it is filled with the
 * bounding rectangle of all the printed chars.
 * The align variables control the text placement with
 * regards to x and y.
 */
void display_text(SDL_Rect *boundary,
		  font_t *font,
		  int x, int y, 
		  enum text_alignment xalign, 
		  enum text_alignment yalign,		 
		  char *string);

#endif
