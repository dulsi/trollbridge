/*-------------------------------------------------------------------------*\
  <bug.C> -- Troll Bridge dynamically linked library - Bugs

  Date      Programmer  Description
  12/12/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"
#include "dllconst.h"
#include "bug.h"

#define TROLL_BUG_WAITING 0
#define TROLL_BUG_RUNSLOW 1
#define TROLL_BUG_RUNFAST 2

TrollThing *TrollBug::createBlueBug(TrollScreen *scr, IShort xStart,
  IShort yStart, IUShort secrt = 0)
{
 return new TrollBug(scr, xStart, yStart, secrt, 0);
}

TrollThing *TrollBug::createRedBug(TrollScreen *scr, IShort xStart,
  IShort yStart, IUShort secrt = 0)
{
 return new TrollBug(scr, xStart, yStart, secrt, 10);
}

void TrollBug::react()
{
 IShort xDiff, yDiff, xSquare, ySquare;
 const Sprite *bugSprite = TrollSpriteHandler.getSprite(sprite);
 if (((state == TROLL_BUG_RUNSLOW) && (time & 1)) ||
   (state == TROLL_BUG_RUNFAST))
/* if (((state == TROLL_BUG_RUNSLOW) && ((time & 3) & 2)) ||
   ((state == TROLL_BUG_RUNFAST) && (time & 1)))*/
 {
  xSquare = (x + xSpeed) / TROLL_SQUARE_X;
  ySquare = (y + ySpeed - TROLL_BUFFER_Y) / TROLL_SQUARE_Y;
  xDiff = x + xSpeed - (xSquare * TROLL_SQUARE_X) + bugSprite->getXSize();
  yDiff = y + ySpeed - (ySquare * TROLL_SQUARE_Y + TROLL_BUFFER_Y) +
    bugSprite->getYSize();
  if ((xSquare < 0) || (xSquare >= TROLL_SCREEN_X) || (ySquare < 0) ||
    (ySquare >= TROLL_SCREEN_Y) ||
    (screen->getPassability(xSquare, ySquare) > TROLL_NOMONSTER) ||
    ((xDiff >= TROLL_SQUARE_X) &&
      (screen->getPassability(xSquare + 1, ySquare) > TROLL_NOMONSTER)) ||
    ((yDiff >= TROLL_SQUARE_Y) &&
      (screen->getPassability(xSquare, ySquare + 1) > TROLL_NOMONSTER)) ||
    ((xDiff >= TROLL_SQUARE_X) && (yDiff >= TROLL_SQUARE_Y) &&
      (screen->getPassability(xSquare + 1, ySquare + 1) > TROLL_NOMONSTER)))
  {
   changeSpeed();
  }
  else
  {
   y += ySpeed;
   x += xSpeed;
   frame++;
   frame %= 2;
  }
 }
 time--;
 if (time == 0)
 {
  switch (state)
  {
   case TROLL_BUG_RUNSLOW:
    if (IRandom(2) == 1)
    {
     state = TROLL_BUG_RUNFAST;
     time = 100 + IRandom(100);
    }
    else
    {
     state = TROLL_BUG_WAITING;
     time = 20 + IRandom(20);
    }
    break;
   case TROLL_BUG_RUNFAST:
   case TROLL_BUG_WAITING:
   default:
    state = TROLL_BUG_RUNSLOW;
    time = 100 + IRandom(50);
    break;
  }
 }
/* // Old movement with full passibility check
 if ((y + ySpeed < TROLL_SQUARE_Y * 2 + TROLL_BUFFER_Y) ||
   (y + ySpeed > 200 - TROLL_SQUARE_Y * 2 -
     TrollSpriteHandler.getSprite(sprite)->getYSize()) ||
   (x + xSpeed < TROLL_SQUARE_X * 2) ||
   (x + xSpeed > 320 - TROLL_SQUARE_X * 2 -
     TrollSpriteHandler.getSprite(sprite)->getXSize()))
 {
  changeSpeed();
 }
 else
 {
  y += ySpeed;
  x += xSpeed;
  frame++;
  frame %= 2;
 }*/
}

TrollBug::TrollBug(TrollScreen *scr, IShort xStart, IShort yStart,
  IUShort secrt, IUShort shft)
 : TrollStandardMonster(scr, secrt, 1)
{
 sprite = TROLL_SPRITE_BUG;
 shift = shft;
 frame = 0;
 hp = 1;
 changeSpeed();
 if ((xStart == TROLL_XYRANDOM) && (yStart == TROLL_XYRANDOM))
 {
  findOpen();
 }
 else
 {
  x = xStart;
  y = yStart;
 }
 state = TROLL_BUG_RUNSLOW;
 time = IRandom(50) + 100;
/* x = IRandom(TROLL_SQUARE_X * (TROLL_SCREEN_X - 5)) + TROLL_SQUARE_X * 2;
 y = IRandom(TROLL_SQUARE_Y * (TROLL_SCREEN_Y - 5)) + TROLL_SQUARE_Y * 2 +
   TROLL_BUFFER_Y;*/
}

void TrollBug::changeSpeed()
{
 ySpeed = IRandom(5);
 xSpeed = 4 - ySpeed;
 if (xSpeed > ySpeed)
 {
  facing = direction = TROLL_RIGHT;
 }
 else
 {
  facing = direction = TROLL_DOWN;
 }
 switch (IRandom(4))
 {
  case 0:
   ySpeed *= -1;
   if (facing == TROLL_DOWN)
   {
    facing = direction = TROLL_UP;
   }
   break;
  case 1:
   ySpeed *= -1;
   if (facing == TROLL_DOWN)
   {
    facing = direction = TROLL_UP;
   }
   // continues to case 2
  case 2:
   xSpeed *= -1;
   if (facing == TROLL_RIGHT)
   {
    facing = direction = TROLL_LEFT;
   }
   break;
  case 3:
  default:
   break;
 }
}

