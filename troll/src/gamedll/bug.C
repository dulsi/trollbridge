/*-------------------------------------------------------------------------*\
  <bug.C> -- Troll Bridge dynamically linked library - Bugs

  Date      Programmer  Description
  12/12/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"
#include "dllconst.h"
#include "bug.h"

TrollThing *TrollBug::createBlueBug(TrollScreen *scr, IUShort secrt = 0)
{
 return new TrollBug(scr, secrt, 0);
}

TrollThing *TrollBug::createRedBug(TrollScreen *scr, IUShort secrt = 0)
{
 return new TrollBug(scr, secrt, 10);
}

void TrollBug::react()
{
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
 }
}

TrollBug::TrollBug(TrollScreen *scr, IUShort secrt, IUShort shft)
 : TrollStandardMonster(scr, secrt, 1)
{
 sprite = TROLL_SPRITE_BUG;
 shift = shft;
 frame = 0;
 hp = 1;
 changeSpeed();
 x = IRandom(TROLL_SQUARE_X * (TROLL_SCREEN_X - 5)) + TROLL_SQUARE_X * 2;
 y = IRandom(TROLL_SQUARE_Y * (TROLL_SCREEN_Y - 5)) + TROLL_SQUARE_Y * 2 +
   TROLL_BUFFER_Y;
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

