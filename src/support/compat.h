#ifndef COMPAT_H
#define COMPAT_H

/*
  Platform specific solutions to some common problems.

*/

/* Define INLINE for different compilers.  If your compiler does not
   support inlining then there might be a performance hit in
   bitmask_overlap_area() used for collision checking.
*/
#ifndef INLINE
# ifdef __GNUC__
#  define INLINE inline
# else
#  ifdef _MSC_VER
#   define INLINE __inline
#  else
#warning No INLINE definition, collision bitmasking performance may suffer.
#   define INLINE
#  endif
# endif
#endif

#include <math.h>
#ifndef M_PI
# define M_PI 3.1415926535897931
#endif

/* MINGW supplies a unistd.h, so we can use it even when compiling in windows */
#if defined(WIN32) && !defined(__MINGW32__)
static INLINE int chdir(const char *path)
{
	fprintf(stderr, "Reading files from path '%s'...\n", path);
	return 1;
}
#else
/* POSIX and related systems */
# include <unistd.h>
#endif

#include <stdio.h>

#define BUG(msg) do { fprintf(stderr,"BUG(%s) in %s:%d\n",msg,__FILE__,__LINE__); *(int *)0 = 1; } while (0);


#endif
