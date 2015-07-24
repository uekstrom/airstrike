#ifndef SCHEDULE_H
#define SCHEDULE_H
#include "core.h"
/*
  This is for scheduling game logic events. delay is in game frames,
  low priority events are run first.
*/

#define SCHED_NORMAL 0
#define SCHED_EARLY -1000
#define SCHED_LATE 1000
#define SCHED_FIRST -2000000
#define SCHED_LAST 2000000

/* 'delay' in frames */
void schedule(unsigned int delay, int priority, void (*f)(msg_t msg, void *target), msg_t msg, void *target);
void schedule_advance(unsigned int step, int priority);
int frame_nr(void);

/* Return 1 and set the timer if more than delay passed since last time,
   else return 0 (meaning "try later"). 
   'delay' is in 100 milliseconds. 
*/
int rate_limit(unsigned int delay, unsigned int *timer);
/* convert delay (in 100 milliseconds) to game frames */
static INLINE unsigned int delay_to_frames(unsigned int delay)
{
  return (delay * FRAMERATE) / 10;
}
/* send a message to the object after a delay (in 100ms) */
void obj_alarm(unsigned int delay, void *obj,  msg_t msg);

#endif
