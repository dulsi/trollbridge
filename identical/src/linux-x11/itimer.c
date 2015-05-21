/*-------------------------------------------------------------------------*\
  <itimer.c> -- Identical Software's Timer Routines (Linux X11 Version)

  Date      Programmer  Description
  02/28/97  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include <istdlib.h>
#include <itimer.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Global variables
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static volatile int ITimerState;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Prototypes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void ITimerAlarm(int signo);

void ITimerStart(IUShort tps)
{
 struct sigaction act, oact;
 struct itimerval tmv, otmv;

 ITimerState = 1;
 act.sa_handler = ITimerAlarm;
 sigemptyset(&act.sa_mask);
 act.sa_flags = 0;
 sigaction(SIGALRM, &act, &oact);
 tmv.it_interval.tv_sec = tmv.it_value.tv_sec = 0;
 tmv.it_interval.tv_usec = tmv.it_value.tv_usec = 1000000 / tps;
 setitimer(ITIMER_REAL, &tmv, &otmv);
}

void ITimerWait()
{
 while (ITimerState == 0)
 {
 }
 ITimerState = 0;
}

void ITimerEnd()
{
}

static void ITimerAlarm(int signo)
{
 ITimerState = 1;
}

