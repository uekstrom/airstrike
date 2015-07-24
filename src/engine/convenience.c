#include <string.h>
#include "xalloc.h"
#include "convenience.h"

/* Allocates new memory and returns s1 joined with s2 */
char *strjoin(const char *s1, const char *s2)
{
  int l1 = strlen(s1);
  char *s = xalloc(l1+strlen(s2)+1);
  strcpy(s,s1);
  strcpy(s+l1,s2);
  return s;
}


char *strjoin3(const char *s1, const char *s2, const char *s3)
{
  int l1 = strlen(s1);
  int l2 = strlen(s2);
  char *s = xalloc(l1+l2+strlen(s3)+1);
  strcpy(s,s1);
  strcpy(s+l1,s2);
  strcpy(s+l1+l2,s3);
  return s;
}
