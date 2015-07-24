#include <stdio.h>
#include <string.h>
#include "xalloc.h"

static void warnandquit(void)
{
  fprintf(stderr,"Out of memory, quitting.\n");
  exit(EXIT_FAILURE);
}

static void (*atoom)(void) = warnandquit;

void xalloc_atoom(void (*function)(void))
{
  atoom = function;
}

void *xalloc(size_t size)
{
  void *p = malloc(size);
  if (!p)
    atoom();
  return p;
}

void *xcalloc(size_t nmemb, size_t size)
{
  void *p = calloc(nmemb,size);
  if (!p)
    atoom();
  return p;
}

void *xrealloc(void *ptr, size_t size)
{
  void *p = realloc(ptr,size);
  if (!p)
    atoom();
  return p;
}

char *xstrdup(const char *str)
{
  char *ds = strdup(str);
  if (!ds)
    atoom();
  return ds;
}
