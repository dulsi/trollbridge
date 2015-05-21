/*-------------------------------------------------------------------------*\
  <itimer.c> -- Identical Software's Timer Routines (ALLEGRO Version)

  Date      Programmer  Description
  12/04/98  Dennis      Created.
\*-------------------------------------------------------------------------*/
#include <itimer.h>
#include <allegro.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Global variables
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static volatile int ITimerState;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Prototypes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void ITimerHandler();

void ITimerStart(IUShort tps)
{
 ITimerState = 1;
 install_int_ex(ITimerHandler, BPS_TO_TIMER(tps));
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
 remove_int(ITimerHandler);
}

static void ITimerHandler()
{
 ITimerState = 1;
}

