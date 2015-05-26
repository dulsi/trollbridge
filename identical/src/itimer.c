/*-------------------------------------------------------------------------*\
  <itimer.c> -- Identical Software's Timer Routines (Linux SVGALIB Version)

  Date      Programmer  Description
  02/28/97  Dennis      Created.
\*-------------------------------------------------------------------------*/
#include <itimer.h>
#include <SDL_timer.h>

static IULong ITimerTicksIncrement;
static Uint32 ITimerStop;

void ITimerStart(IUShort tps)
{
 ITimerTicksIncrement = 1000 / tps;
 ITimerStop = SDL_GetTicks() + ITimerTicksIncrement;
}

void ITimerWait()
{
 Uint32 millisec;

 millisec = SDL_GetTicks();
 if (millisec < ITimerStop)
 {
  SDL_Delay(ITimerStop - millisec);
 }
 ITimerStop = SDL_GetTicks() + ITimerTicksIncrement;
}

void ITimerEnd()
{
}

