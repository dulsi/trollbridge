#ifndef __ITIMER_H
#define __ITIMER_H
/*-------------------------------------------------------------------------*\
  <itimer.h> -- Identical Software's Timing Routines

  Date      Programmer  Description
  11/09/97  Dennis      Created.
\*-------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include <istdlib.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  ITimerStart - Initializes timer.

    Parameters:
      tps          (In)  Number of ticks per second
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void ITimerStart(IUShort tps);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  ITimerWait - Waits for a tick.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void ITimerWait();

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  ITimerEnd - Shutdown timer.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void ITimerEnd();

#ifdef __cplusplus
}
#endif

#endif

