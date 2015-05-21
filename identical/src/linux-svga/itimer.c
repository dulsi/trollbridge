/*-------------------------------------------------------------------------*\
  <itimer.c> -- Identical Software's Timer Routines (Linux SVGALIB Version)

  Date      Programmer  Description
  02/28/97  Dennis      Created.
\*-------------------------------------------------------------------------*/
#include <itimer.h>
#include <sys/time.h>
#include <unistd.h>

static IULong ITimerTicksIncrement;
static struct timeval ITimerStop;

void ITimerStart(IUShort tps)
{
 struct timezone tz;
 IULong usec;

 gettimeofday(&ITimerStop, &tz);
 usec = ITimerStop.tv_usec;
 ITimerStop.tv_usec += (ITimerTicksIncrement = 1000000 / tps);
 if (ITimerStop.tv_usec < usec)
 {
  ITimerStop.tv_sec++;
 }
}

void ITimerWait()
{
 struct timezone tz;
 struct timeval tv;

 gettimeofday(&tv, &tz);
 if ((tv.tv_sec <= ITimerStop.tv_sec) && (tv.tv_usec < ITimerStop.tv_usec))
 {
  tv.tv_sec = ITimerStop.tv_sec - tv.tv_sec;
  if (tv.tv_usec > ITimerStop.tv_usec)
  {
   tv.tv_sec--;
  }
  tv.tv_usec = ITimerStop.tv_usec - tv.tv_usec;
  select(0, NULL, NULL, NULL, &tv);
 }
 gettimeofday(&ITimerStop, &tz);
 tv.tv_usec = ITimerStop.tv_usec;
 ITimerStop.tv_usec += ITimerTicksIncrement;
 if (ITimerStop.tv_usec < tv.tv_usec)
 {
  ITimerStop.tv_sec++;
 }
}

void ITimerEnd()
{
}

