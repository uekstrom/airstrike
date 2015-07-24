#ifndef XALLOC_H
#define XALLOC_H
#include <stdlib.h>

/*
  Memory allocation with error checking.
*/
void *xalloc(size_t size);
void *xcalloc(size_t nmemb, size_t size);
void *xrealloc(void *ptr, size_t size);
char *xstrdup(const char *str);

/* What should be done at OOM (out of memory)?
   Default action is to print an error message and run exit(EXIT_FAILURE).
 */
void xalloc_atoom(void (*function)(void));

#endif
