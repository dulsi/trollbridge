/*-------------------------------------------------------------------------*\
  <itimer.c> -- Identical Software's Timer Routines (DOS Version)

  Date      Programmer  Description
  02/28/97  Dennis      Created.
\*-------------------------------------------------------------------------*/
#include <itimer.h>
#include <dos.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Defines
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define ITIMER_INT 0x08
#define ITIMER_COUNT 0x1234DDL

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Global variables
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static IUShort ITimerTicksIncrement;
static long ITimerTicks;
static volatile int ITimerState;
static void interrupt (*ITimerOldHandler)();

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Prototypes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static void interrupt ITimerHandler();

void ITimerStart(IUShort tps)
{
 ITimerTicksIncrement = ITIMER_COUNT / tps;
 ITimerTicks = 0;
 ITimerState = 1;
 ITimerOldHandler = getvect(ITIMER_INT);
 setvect(ITIMER_INT, ITimerHandler);
 asm {
  mov al, 34h
  out 43h, al
  mov ax, ITimerTicksIncrement
  out 40h, al
  mov al, ah
  out 40h, al
 }
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
 asm {
  mov al, 34h
  out 43, al
  mov al, 0
  out 40h, al
  out 40h, al
 }
 setvect(ITIMER_INT, ITimerOldHandler);
}

static void interrupt ITimerHandler()
{
 ITimerState = 1;
 ITimerTicks += ITimerTicksIncrement;
 /* Ticks per second of less than 19 will cause the timer to be off */
 if (ITimerTicks > 0x10000L)
 {
  ITimerTicks -= 0x10000L;
  ITimerOldHandler();
 }
 else
 {
  asm {
   mov al, 20h
   out 20h, al
  }
 }
}

