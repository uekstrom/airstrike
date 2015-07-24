#ifndef CONF_H
#define CONF_H

#include <stdio.h>

/*
  Configuration file/database library for Airstrike.
  The conf_t datatype is a tree where the leaf nodes are
  either numbers (double precision), or strings. Nodes can
  be named or anonymous.

  The trees can be written out, and read in, in the format (example)

  this_is_a_name "this is a string"
  x 12
  this_is_a_section {
    "item 1 (anonymous string)"
    2
    6.7
    { "This string is inside an anonymous subtree (section)" }
    a_number 17
  }  

  To get the value 6.7 here we could use the function

  double default_value = 2.1;
  double x;
  x = conf_get_number(conf,"this_is_a_section[2]",default_value);

  If "this_is_a_section[2]" is not found x will get the value 2.1.

  conf_get_number(conf,"this_is_a_section.a_number",default_value)
  would return 17 in this example.

  ---

  Whitespace is completely optional, except to separate names and numbers
  in some cases.

  Names must start with a-Z_ and can contain those characters and
  digits, but no other characters.

  Strings are always enclosed in double quotes.

  Numbers must start with 0-9+-. and are parsed with sscanf.

  Comments are not allowed, as they are difficult to preserve
  when saving a conf object after reading and modifying it, the 
  tree structure should make the thing more or less self-documenting.

*/


/* Maximum length of names and string values. */
#define CONF_BUFSIZE 512

typedef struct conf conf_t;

conf_t *conf_new(void);
void conf_free(conf_t *conf);

void conf_write(const conf_t *conf, FILE *dst);
conf_t *conf_read(FILE *src);

/* Try to find the item by path, will create the item if it does not
   exists.  The number and string functions create new items with the
   default value, while the section function create an empty
   section. Any sections leading up to the final item will also be
   created, possibly deleting any items of other types in the way.
*/
double conf_number(conf_t *conf, const char *path, double default_value);
const char *conf_string(conf_t *conf, const char *path, const char *default_value);
conf_t *conf_section(conf_t *conf, const char *path);

/* More specialized function below. */


/* The conf_get_* functions return default_value if the path is
   invalid, or if the item is of the wrong type. */ 
double conf_lookup_number(const conf_t *conf, const char *path, double default_value);
/* conf_get_string returns a pointer to the string still owned by the
   conf, this pointer should not be stored between calls to the
   conf_* functions. */
const char *conf_lookup_string(const conf_t *conf, const char *path, const char *default_value);
/* conf_get_section returns a pointer to the section still owned by
   the conf, this pointer should not be stored between calls to the
   conf_* functions. */
conf_t *conf_lookup_section(conf_t *conf, const char *path, conf_t *default_value);

int conf_nr_items(const conf_t *conf);

int conf_item_is_number(const conf_t *conf, int item);
int conf_item_is_string(const conf_t *conf, int item);
int conf_item_is_section(const conf_t *conf, int item);

/* Return items by index */
double conf_get_number(const conf_t *conf, int item, double default_value);
const char *conf_get_string(const conf_t *conf, int item, const char *default_value);
conf_t *conf_get_section(conf_t *conf, int item, conf_t *default_value);
/* Return the name of item, or NULL if the item is anonymous or if the index is
   out of range. */
const char *conf_get_name(const conf_t *conf, int item);

/* The set functions only work if the path up to the last item exists,
   and if the last name is valid. The old value (if any) will be
   deleted.  Path can be given as NULL to simply append the value
   anonymously last in the configuration tree.
*/
void conf_set_number(conf_t *conf, const char *path, double value);
/* conf_set_string takes a copy of value. */
void conf_set_string(conf_t *conf, const char *path, const char *value);
/* conf_set_section takes ownership of section, no other pointers
   to section should be stored anywhere. If a section already exists
   at path the old section and all its contents will be deleted.
   If section is passed as NULL a new empty section is inserted at path. */
void conf_set_section(conf_t *conf, const char *path, conf_t *section);


#endif
