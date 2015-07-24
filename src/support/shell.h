#ifndef GETARGS_H
#define GETARGS_H
#include <stdio.h>

#define SHELL_MAXARGS 32
#define SHELL_LINEBUF 1024

#define SHELL_EBUF -127
#define SHELL_EFMT -126
#define SHELL_EFUN -125

/* argc is the number of actual argument, argv is null terminated. */
typedef int shell_fun_t(int argc, const char *argv[]);

void shell_defun(const char *name, shell_fun_t *f);
int shell_eval(const char *cmdline);
void shell_print(const char *format, ...);
void shell_warning(const char *format, ...);
int shell_error(const char *format, ...);
void shell_eval_file(FILE *src);

/*
  If there is a string arg=VALUE in argv, try to put VALUE
  into dst. Return 1 on success, else 0. If arg exists, but
  VALUE is invalid then print an error messages with shell_warning
  and return 0.
*/
int getarg_int(const char *argv[], const char *arg, int *dst);
int getarg_float(const char *argv[], const char *arg, float *dst);
/* Put a pointer into argv[..] in dst if arg found, otherwise, leave dst as it is */
int getarg_str(const char *argv[], const char *arg, const char **dst);

#endif
