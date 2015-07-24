#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "xalloc.h"
#include "conf.h"

#define VALID_NAME_CHARS(c) (isalpha(c) || c == '-' || c == '_' || isdigit(c))

enum conf_type 
  {
    CONF_INVALID = -1,
    CONF_SECTION = 0,
    CONF_STRING,
    CONF_NUMBER
  };


struct conf
{
  int len;
  int max_len;
  struct conf_item
  {
    char *name;
    enum conf_type type;
    union
    {
      struct conf *section;
      char *string;
      double number;
    } value;    
  } *items;
};


conf_t *conf_new(void)
{
  conf_t *conf = xalloc(sizeof(*conf));
  conf->max_len = 8;
  conf->len = 0;
  conf->items = xalloc(sizeof(*conf->items)*conf->max_len);
  return conf;
}

static void conf_item_free_value(struct conf_item *item)
{
  if (item->type == CONF_SECTION)
    conf_free(item->value.section);
  else if (item->type == CONF_STRING)
    free(item->value.string);
}

static void conf_item_free_item(struct conf_item *item)
{
  if (item->name)
    free(item->name);
  conf_item_free_value(item);
}

void conf_free(conf_t *conf)
{
  int i;
  for (i=0;i<conf->len;i++)
    conf_item_free_item(&conf->items[i]);
  free(conf->items);
  free(conf);
}

static void conf_write_helper(const conf_t *conf, FILE *dst, int indent)
{
  int i,j;
  for (i=0;i<conf->len;i++)
    {
      for (j=0;j<indent;j++)
	fprintf(dst,"  ");

      if (conf->items[i].name)
	fprintf(dst,"%s ",conf->items[i].name);

      if (conf->items[i].type == CONF_SECTION)
	{
	  fprintf(dst,"{\n");
	  conf_write_helper(conf->items[i].value.section,dst,indent+1);
	  for (j=0;j<indent;j++)
	    fprintf(dst,"  ");
	  fprintf(dst,"}\n");
	}
      else if (conf->items[i].type == CONF_STRING)
	{
	  fprintf(dst,"\"%s\"\n",conf->items[i].value.string);
	}
      else
	{
	  fprintf(dst,"%lg\n",conf->items[i].value.number);
	}
    }
}

void conf_write(const conf_t *conf, FILE *dst)
{
  conf_write_helper(conf,dst,0);
}



static int skip_ws(FILE *src)
{
  int c;
  do {
    while (isspace(c = fgetc(src)));
    if (c == '#') {
       do {
         c = fgetc(src);
         if (c == EOF)
           return c;
       } while (c != '\n');
    }
  } while (isspace(c));
  if (c != EOF)
    ungetc(c,src);
  return c;
}


conf_t *conf_read(FILE *src)
{
  int c, got_name = 0;  
  char namebuf[CONF_BUFSIZE], strbuf[CONF_BUFSIZE];
  conf_t *conf = conf_new();
  while (1)
    {
      c = skip_ws(src);
      if (c == EOF)
	{
	  if (got_name)
	    {
	      fprintf(stderr,"Expected value after name '%s', got end of file.\n",namebuf);
	      conf_free(conf);
	      return NULL;
	    }
	  else
	    {
	      return conf;
	    }
	}
      if (isalpha(c) || c == '_') /* A name is next */
	{
	  int n = 0;
	  c = fgetc(src);
	  while (VALID_NAME_CHARS(c) && n < CONF_BUFSIZE)
	    {
	      namebuf[n++] = c;
	      c = fgetc(src);
	    }
	  if (n >= CONF_BUFSIZE)
	    {
	      namebuf[CONF_BUFSIZE-1] = '\0';
	      fprintf(stderr,"conf_read(): Error, name too long: %s..\n",namebuf);
	      conf_free(conf);
	      return NULL;
	    }
	  namebuf[n] = '\0';
	  if (c != EOF)
	    ungetc(c,src); /* Put back first char of next token */
	  got_name = 1;
	}
      else if (c == '{') /* Read a section */
	{
	  conf_t *section;
	  fgetc(src);
	  section = conf_read(src);
	  if (!section)
	    {
	      conf_free(conf);
	      return NULL;
	    }
	  if (got_name)
	    conf_set_section(conf,namebuf,section);
	  else
	    conf_set_section(conf,NULL,section);
	  got_name = 0;
	}
      else if (c == '}') /* End of section */
	{
	  if (got_name)
	    {
	      fprintf(stderr,"Expected value after name '%s', got '}'.\n",namebuf);
	      conf_free(conf);
	      return NULL;
	    }
	  else
	    {
	      fgetc(src);
	      return conf;
	    }
	}
      else if (c == '"')
	{
	  int n = 0;
	  fgetc(src);
	  c = fgetc(src);
	  while (c != '"' && c != EOF && n < CONF_BUFSIZE)
	    {
	      /* TODO: Put in escape sequences here, in particular \" */
	      strbuf[n++] = c;
	      c = fgetc(src);
	    }
	  if (n >= CONF_BUFSIZE)
	    {
	      strbuf[CONF_BUFSIZE-1] = '\0';
	      fprintf(stderr,"conf_read(): Error, string too long: \"%s..\n",strbuf);
	      conf_free(conf);
	      return NULL;
	    }
	  strbuf[n] = '\0';
	  if (c == EOF)
	    {
	      fprintf(stderr,"conf_read(): Error, unterminated string: \"%s..\"\n",strbuf);
	      conf_free(conf);
	      return NULL;
	    }
	  if (got_name)
	    conf_set_string(conf,namebuf,strbuf);
	  else
	    conf_set_string(conf,NULL,strbuf);
	  got_name = 0;
	}
      else if (isdigit(c) || c == '+' || c == '-' || c == '.')
	{
	  double x;
	  if (fscanf(src,"%lf",&x) != 1)
	    {
	      fprintf(stderr,"conf_read(): Error reading number, first character was '%c'\n",c);
	      conf_free(conf);
	      return NULL;
	    }
	  if (got_name)
	    conf_set_number(conf,namebuf,x);
	  else
	    conf_set_number(conf,NULL,x);
	  got_name = 0;
	}
      else
	{
	  fprintf(stderr,"conf_read(): Error, Read unexpected character '%c'.\n",c);
	  conf_free(conf);
	  return NULL;
	}
    } 
}


int conf_nr_items(const conf_t *conf)
{
  return conf->len;
}

int conf_item_is_number(const conf_t *conf, int idx)
{
  return idx >= 0 && idx < conf->len && conf->items[idx].type == CONF_NUMBER;
}

int conf_item_is_string(const conf_t *conf, int idx)
{
  return idx >= 0 && idx < conf->len && conf->items[idx].type == CONF_STRING;
}

int conf_item_is_section(const conf_t *conf, int idx)
{
  return idx >= 0 && idx < conf->len && conf->items[idx].type == CONF_SECTION;
}

double conf_get_number(const conf_t *conf, int idx, double default_value)
{
  if (conf_item_is_number(conf,idx))
    return conf->items[idx].value.number;
  else
    return default_value;
}

const char *conf_get_string(const conf_t *conf, int idx, const char *default_value)
{
  if (conf_item_is_string(conf,idx))
    return conf->items[idx].value.string;
  else
    return default_value;
}

conf_t *conf_get_section(conf_t *conf, int idx, conf_t *default_value)
{
  if (conf_item_is_section(conf,idx))
    return conf->items[idx].value.section;
  else
    return default_value;
}


/* If path and head matches until the end of head, and if then the
   next character of path is in ".[\0", return a pointer to this next
   character, else return NULL.
*/
static const char *dotmatch(const char *path, const char *head)
{
  if (!head)
    return NULL;
  while (*head)
    if (*head++ != *path++)
      return NULL;
  if (*path == '.' || *path == '[' || *path == '\0')
    return path;
  else
    return NULL;
}

static const char *path_next(const char *path)
{
  if (*path == '[')
    {
      while (*path && *path != ']')
	path++;
      if (*path == ']')
	path++; 
    }
  else
    {
      while (*path && *path != '.' && *path != '[')
	path++;
    }
  if (*path == '.')
    path++;
  return path;
}

/* Count the depth of the path.
   "" would have the depth 0,
   "name" would have the depth 1,
   "[23]" has the depth 1
   "a.b.c" has the depth 3.
   "a[23].x" also has the depth 3
*/
static int path_length(const char *path)
{
  int n = 0;
  while (*path)
    {
      path = path_next(path);
      n++;
    }
  return n;
}

/* Return the item corresponding to the head of path, or
 NULL if no such item exists. */
static struct conf_item *conf_get_first_item(conf_t *conf, const char *path)
{
  int i;
  if (path[0] == '[')
    {
      if (sscanf(path,"[%i",&i) == 1 && i>=0 && i < conf->len)
	return &conf->items[i];
    }
  else
    {
      for (i=0;i<conf->len;i++)
	if (dotmatch(path,conf->items[i].name))
	  return &conf->items[i];
    }
  return NULL;
}

static const struct conf_item *conf_const_get_first_item(const conf_t *conf, const char *path)
{
  int i;
  if (path[0] == '[')
    {
      if (sscanf(path,"[%i",&i) == 1 && i>=0 && i < conf->len)
	return &conf->items[i];
    }
  else
    {
      for (i=0;i<conf->len;i++)
	if (dotmatch(path,conf->items[i].name))
	  return &conf->items[i];
    }
  return NULL;
}



double conf_lookup_number(const conf_t *conf, const char *path, double default_value)
{
  const struct conf_item *item; 
  item = conf_const_get_first_item(conf,path);
  if (!item)
    return default_value;
  path = path_next(path);
  while (*path)
    {
      if (item->type == CONF_SECTION)
	conf = item->value.section;
      else
	return default_value;
      item = conf_const_get_first_item(conf,path);
      if (!item)
	return default_value;
      path = path_next(path);
    }
  if (item->type == CONF_NUMBER)
    return item->value.number;
  else
    return default_value;
}

const char *conf_lookup_string(const conf_t *conf, const char *path, const char *default_value)
{
  const struct conf_item *item; 
  item = conf_const_get_first_item(conf,path);
  if (!item)
    return default_value;
  path = path_next(path);
  while (*path)
    {
      if (item->type == CONF_SECTION)
	conf = item->value.section;
      else
	return default_value;
      item = conf_const_get_first_item(conf,path);
      if (!item)
	return default_value;
      path = path_next(path);
    }
  if (item->type == CONF_STRING)
    return item->value.string;
  else
    return default_value;
}

conf_t *conf_lookup_section(conf_t *conf, const char *path, conf_t *default_value)
{
  const struct conf_item *item; 
  
  if (path[0] == '.' && path[1] == 0) /* Allow '.' as a path, refers to conf itself. */
    return conf;

  item = conf_const_get_first_item(conf,path);
  if (!item)
    return default_value;
  path = path_next(path);
  while (*path)
    {
      if (item->type == CONF_SECTION)
	conf = item->value.section;
      else
	return default_value;
      item = conf_const_get_first_item(conf,path);
      if (!item)
	return default_value;
      path = path_next(path);
    }
  if (item->type == CONF_SECTION)
    return item->value.section;
  else
    return default_value;
}

/* Check if a name (not a full path) is valid. */
static int name_isvalid(const char *name)
{
  if (isalpha(*name) || *name == '_')
    {
      do
	name++;
      while (VALID_NAME_CHARS(*name));
      if (*name == '\0')
	return 1;
      else
	return 0;
    }
  else
    {
      return 0;
    }
}

static struct conf_item *conf_append(conf_t *conf, const char *name)
{
  while (conf->len >= conf->max_len)
    {
      conf->max_len*=2;
      conf->items = realloc(conf->items,sizeof(*conf->items)*conf->max_len);
    }
  if (name)
    conf->items[conf->len].name = xstrdup(name);
  else
    conf->items[conf->len].name = NULL;
  conf->items[conf->len].type = CONF_INVALID;
  return &conf->items[conf->len++];
}

void conf_set_number(conf_t *conf, const char *path, double value)
{
  struct conf_item *item; 

  if (!path)
    {
      item = conf_append(conf,path);
      item->type = CONF_NUMBER;
      item->value.number = value;	
    }
  else
    {
      int n = path_length(path);
      if (n < 1)
	goto badpath;
      while (n > 1)
	{
	  item = conf_get_first_item(conf,path);
	  if (item && item->type == CONF_SECTION)
	    {
	      conf = item->value.section;
	      path = path_next(path);
	      n--;
	    }
	  else
	    {
	      goto badpath;
	    }      
	}
      item = conf_get_first_item(conf,path);
      if (item)
	conf_item_free_value(item);
      else if (name_isvalid(path))
	item = conf_append(conf,path);
      else
	goto badpath;
      item->type = CONF_NUMBER;
      item->value.number = value;	
      return;
    badpath:
      fprintf(stderr,"conf_set_num(): Error, invalid path.\n");
      
    }
}

void conf_set_string(conf_t *conf, const char *path, const char *value)
{
  struct conf_item *item; 

  if (!path)
    {
      item = conf_append(conf,path);
      item->type = CONF_STRING;
      item->value.string = strdup(value);	
    }
  else
    {
      int n = path_length(path);
      if (n < 1)
	goto badpath;
      while (n > 1)
	{
	  item = conf_get_first_item(conf,path);
	  if (item && item->type == CONF_SECTION)
	    {
	      conf = item->value.section;
	      path = path_next(path);
	      n--;
	    }
	  else
	    {
	      goto badpath;
	    }      
	}
      item = conf_get_first_item(conf,path);
      if (item)
	conf_item_free_value(item);
      else if (name_isvalid(path))
	item = conf_append(conf,path);
      else
	goto badpath;
      
      item->type = CONF_STRING;
      item->value.string = strdup(value);
      return;
    badpath:
      fprintf(stderr,"conf_set_string(): Error, invalid path.\n");
    }
}

void conf_set_section(conf_t *conf, const char *path, conf_t *section)
{
  struct conf_item *item; 
  if (!path)
    {
      item = conf_append(conf,path);
      item->type = CONF_SECTION;
      item->value.section = section;	
    }
  else
    {
      int n = path_length(path);
      if (n < 1)
	goto badpath;
      
      while (n > 1)
	{
	  item = conf_get_first_item(conf,path);
	  if (item && item->type == CONF_SECTION)
	    {
	      conf = item->value.section;
	      path = path_next(path);
	      n--;
	    }
	  else
	    {
	      goto badpath;
	    }      
	}
      
      item = conf_get_first_item(conf,path);
      if (item)
	conf_item_free_value(item);
      else if (name_isvalid(path))
	item = conf_append(conf,path);
      else
	goto badpath;
      
      item->type = CONF_SECTION;
      if (section)
	item->value.section = section;
      else
	item->value.section = conf_new();
      return;
    badpath:
      fprintf(stderr,"conf_set_section(): Error, invalid path.\n");
    }
}

/* Make sure that all the sections leading up to the item given by
   path exists. Any existing items with the same name as a section
   that is given in path will be deleted, new sections will be
   created. Return the new item, which has to be filled in by the caller */
static struct conf_item *conf_probe(conf_t *conf, const char *path)
{
  struct conf_item *item; 
  int n = path_length(path);
  while (n > 1)
    {
      item = conf_get_first_item(conf,path);
      if (item) 
	{
	  if (item->type != CONF_SECTION)
	    {
	      fprintf(stderr,"WARNING: Deleting conf item to make way for implicit section.\n");
	      conf_item_free_value(item);
	      item->type = CONF_SECTION;
	      item->value.section = conf_new();
	    }
	}
      else
	{
	  /* Ugly hack to get the name of the section to create. */
	  int i,nlen = 0;
	  char *name;
	  while (path[nlen] && path[nlen] != '.')
	    nlen++;
	  name = xalloc(nlen+1);
	  for (i=0;i<nlen;i++)
	    name[i] = path[i];
	  name[nlen] = 0;
	  printf("creating section %s\n",name);
	  item = conf_append(conf,name);
	  free(name);
	  item->type = CONF_SECTION;
	  item->value.section = conf_new();
	}    
      conf = item->value.section;
      path = path_next(path);
      n--;  
    }
  if ((item = conf_get_first_item(conf,path)))
    return item;
  else
    return conf_append(conf,path);
}

double conf_number(conf_t *conf, const char *path, double default_value)
{
  struct conf_item *item = conf_probe(conf,path);
  if (item->type != CONF_NUMBER)
    {
      conf_item_free_value(item);
      item->type = CONF_NUMBER;
      item->value.number = default_value;
    }
  return item->value.number;
}

const char *conf_string(conf_t *conf, const char *path, const char *default_value)
{
  struct conf_item *item = conf_probe(conf,path);
  if (item->type != CONF_STRING)
    {
      conf_item_free_value(item);
      item->type = CONF_STRING;
      item->value.string = xstrdup(default_value);
    }
  return item->value.string;
}

conf_t *conf_section(conf_t *conf, const char *path)
{
  struct conf_item *item = conf_probe(conf,path);
  if (item->type != CONF_SECTION)
    {
      conf_item_free_value(item);
      item->type = CONF_SECTION;
      item->value.section = conf_new();	
    }
  return item->value.section;
}

#ifdef TEST

int main(int argc, char *argv[])
{
  conf_t *conf1 = conf_read(stdin);
  if (conf1)
    {
      printf("Read the following configuration:\n");
      conf_write(conf1,stdout);
      printf("probing number\n");
      conf_string(conf1,"a.b.c","plopp");
      printf("...\n");
      conf_number(conf1,"a.b.c.x",12);
      conf_write(conf1,stdout);
      conf_free(conf1);
    }
  else
    {
      fprintf(stderr,"Error, could not read configuration.\n");
    }
  return EXIT_SUCCESS;
}

#endif
