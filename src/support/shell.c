#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "dict.h"
#include "shell.h"

static int is_initialized = 0;
static dict_t *functions;

#ifdef READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

/*
  Tokenize a string "in-place", ie change whitespace for
  null chars and make a table (argv) of the tokens. 
  The number of tokens is returned. -1 Is returned on
  too many args for argv, -2 is returned on unterminated string.
  Bugs: "something"anotherthing is considered to be two tokens,
  maybe not optimal.
  argv is null terminated on a successful call.
*/
static int chop_str(char *src, const char *argv[], int max_args)
{
  int argc = 0;
  char *dst = src;
  while (1)
    {
      while (isspace((unsigned char)*src))
	src++;
      if (*src == 0 || *src == '#')
	{
	  if (argc < max_args)
	    {
	      argv[argc] = 0;	  
	      return argc;
	    }
	  else
	    {
	      return -1;
	    }
	}
      else if (argc < max_args)
	{
	  if (*src == '"')
	    {
	      src++;
	      argv[argc++] = dst;
	      while (*src && *src != '"')
	        {
		  if ((*src == '\\') && (*(src+1) == 'n'))
		    {
		      *dst++ = '\n';
		      src += 2;
		    }
		  else
		    {
		      *dst++ = *src++;
		    }
		}
	      if (*src)
		src++;
	      else
		return -2;
	      *dst++ = 0;
	    }
	  else
	    {
	      argv[argc++] = dst;
	      while (*src && !isspace((unsigned char)*src))
		*dst++ = *src++;
	      if (*src)
		src++;
	      *dst++ = 0;
	    }
	}
      else
	{
	  return -1;
	}
    }
}


void shell_print(const char *format, ...)
{
  va_list ap;
  va_start(ap,format);
  vfprintf(stdout,format,ap);
  va_end(ap);
}

void shell_warning(const char *format, ...)
{
  va_list ap;
  fprintf(stderr,"Warning: ");
  va_start(ap,format);
  vfprintf(stderr,format,ap);
  va_end(ap);
}

int shell_error(const char *format, ...)
{
  va_list ap;
  fprintf(stderr,"Error: ");
  va_start(ap,format);
  vfprintf(stderr,format,ap);
  va_end(ap);
  return -1;
}

static int shell_list_fun(int argc, const char *argv[])
{
  int i;
  for (i=0;i<functions->len;i++)
    shell_print("%s\n",functions->def[i].name);
  return 0;
}

static int shell_help_fun(int argc, const char *argv[])
{
  shell_print("Airstrike shell help:\n");
  shell_print("  Functions are called like FUNCTION ARG1 ARG2 ..,\n");
  shell_print("  where arguments can be enclosed in double quotes to include \n");
  shell_print("  whitespace in an argument. To quit the shell type an end-of-file\n");
  shell_print("  character (C-D in Unix, C-Z in Windows). To get a list of the\n");
  shell_print("  currently defined functions type 'fun'. Most functions will\n");
  shell_print("  print a help text if called without arguments.\n");
  return 0;
}

static void shell_init(void)
{
  if (!is_initialized)
    {
      functions = dict_new();
      is_initialized = 1;
      shell_defun("fun",shell_list_fun);
      shell_defun("help",shell_help_fun);
    }
}

void shell_defun(const char *name, shell_fun_t *f)
{
  shell_init();
  dict_define(functions,name,f);
}

int shell_eval(const char *cmdline)
{
  int i,argc;
  char buf[SHELL_LINEBUF];
  const char *argv[SHELL_MAXARGS];
  shell_fun_t *fun;

  shell_init();

  for (i = 0; i < SHELL_LINEBUF && cmdline[i]; i++)
    buf[i] = cmdline[i];
  if (i >= SHELL_LINEBUF)
    {
      fprintf(stderr,"shell_eval(): cmdline too long (> %i chars)\n",SHELL_LINEBUF);
      return SHELL_EBUF;
    }
  else
    {
      buf[i] = 0;
    }
  argc = chop_str(buf,argv,SHELL_MAXARGS);

  if (argc > 0)
    {
      fun = dict_lookup(functions,argv[0]);
      if (fun)
	{
	  return fun(argc-1,argv+1);
	}
      else
	{
	  fprintf(stderr,"shell_eval(): No such function '%s'\n",argv[0]);
	  return SHELL_EFUN;
	}      
    }       
  else if (argc < 0)
    {
      return SHELL_EFMT;
    }
  else
    {
      return 0;
    }
}

void shell_eval_file(FILE *src)
{
  char cbuf[SHELL_LINEBUF];
  char *buf_read;
  int retval, line = 0;
  clearerr(src);
  while (!feof(src))
    {
      if (src == stdin)
        {
#if READLINE
          char *stripped;
          buf_read = readline("Airstrike> ");
          if (!buf_read)
            break;
          line++;

          stripped = buf_read;
	  while (*stripped && isspace((unsigned char)*stripped))
	     stripped++;
          if (*stripped)
            {
              add_history(stripped);
              retval = shell_eval(stripped);
            }
          free(buf_read);
          continue;
#else
          shell_print("Airstrike> ");
#endif
        }
      buf_read = fgets(cbuf,SHELL_LINEBUF,src);
      line++;
      if (buf_read)
	{
	  retval = shell_eval(cbuf);
	}
    }
} 

/* If str start with prefix return rest of str, else
   return 0
*/
static const char *startswith(const char *str, const char *prefix)
{
  while (*prefix)
    {
      if (*str++ != *prefix++)
	return 0;
    }
  return str;
}

int getarg_int(const char *argv[], const char *arg, int *dst)
{
  const char *s;
  if (getarg_str(argv,arg,&s))
    {
      if (sscanf(s,"%d",dst) == 1)
	{
	  return 1;
	}
      else
	{
	  shell_warning("expected %s=int, got %s. Ignoring argument.\n",
		      arg,s);
	  return 0;	
	}
    }
  else
    {
      return 0;
    }
}

int getarg_float(const char *argv[], const char *arg, float *dst)
{
  const char *s;
  if (getarg_str(argv,arg,&s))
    {
      if (sscanf(s,"%f",dst) == 1)
	{
	  return 1;
	}
      else
	{
	  shell_warning("expected %s=float, got %s. Ignoring argument.\n",
		      arg,s);
	  return 0;	
	}
    }
  else
    {
      return 0;
    }
}

/* Put a pointer into argv[..] in dst if arg found */
int getarg_str(const char *argv[], const char *arg, const char **dst)
{
  const char *tmp;
  int i = 0;
  while (argv[i])
    if ((tmp = startswith(argv[i++],arg)) && (*tmp == '='))
      {
	*dst = ++tmp;
	return 1;
      }
  return 0;
}
