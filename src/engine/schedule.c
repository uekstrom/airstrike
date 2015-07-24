#include "schedule.h"
#include "xalloc.h"
#include "compat.h"

static unsigned int now = 0;

#define TABSIZE 1024

struct task
{
  unsigned int when;
  int priority;
  void (*f)(msg_t msg, void *target);
  msg_t msg;
  obj_id_t target;
  struct task *next;
} *pending[TABSIZE];

/* Return 1 if t1 if before or at the same time as t2 */
static INLINE int before(struct task *t1, struct task *t2)
{
  if ((t1->when < t2->when) || 
      ((t1->when == t2->when) && 
       (t1->priority <= t2->priority)))
    return 1;
  else
    return 0;
}

#include <stdio.h> 

void schedule(unsigned int delay, int priority, void (*f)(msg_t, void *), 
	      msg_t msg, void *target)
{
  unsigned int i;
  struct task *task,*t;

  task = xalloc(sizeof(*task));
  task->when = now + delay;
  task->priority = priority;
  task->msg = msg;
  task->target = obj_id(target);
  task->f = f;

  i = task->when % TABSIZE;

  if (!pending[i] || before(task,pending[i]))
    {
      task->next = pending[i];
      pending[i] = task;
    }
  else
    {
      t = pending[i];
      while (t->next && before(t->next,task))
	t = t->next;
      task->next = t->next;
      t->next = task;
    }
}

void schedule_advance(unsigned int step, int priority)
{
  struct task target,*tp;
  unsigned int i,j;
  target.when = now + step;
  target.priority = priority;
  for (i=0;i<=step;i++)
    {
      j = (now + i) % TABSIZE;
      while (pending[j] && before(pending[j],&target))
	{
	  void *obj = obj_lock(pending[j]->target,NULL);
	  if (obj)
	    {
	      pending[j]->f(pending[j]->msg,obj);
	      obj_unlock(obj);
	    }
	  tp = pending[j];
	  pending[j] = tp->next;
	  free(tp);
	}
    }
  now += step;
}

int frame_nr(void)
{
  return now;
}

int rate_limit(unsigned int delay, unsigned int *timer)
{
  if (now >= *timer)
    {
      *timer = now + delay_to_frames(delay);
      return 1;
    }
  else
    {
      return 0;
    }
}

static void send_obj_alarm(msg_t msg, void *obj)
{
  obj_message(obj, msg);
}

void obj_alarm(unsigned int delay, void *obj,  msg_t msg)
{
  schedule(delay_to_frames(delay), SCHED_NORMAL, send_obj_alarm, msg, obj);
}
