/*-------------------------------------------------------------------------*\
  <corba.C> -- Troll Bridge dynamically linked library - Cobras

  Date      Programmer  Description
  01/23/99  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"
#include "dllconst.h"
#include "items.h"
#include "cobra.h"
#include "blast.h"

#define TROLL_COBRA_SPEED 2
#define TROLL_COBRA_STATE_WAITFULL 0
#define TROLL_COBRA_STATE_PREFIRE  14
#define TROLL_COBRA_STATE_FIRE1    15
#define TROLL_COBRA_STATE_FIRE2    17
#define TROLL_COBRA_STATE_FIRE3    20
#define TROLL_COBRA_STATE_REPEAT   21

TrollThing *TrollCobra::createGreenCobra(TrollScreen *scr, IUShort xStart,
  IUShort yStart, IUShort secrt = 0)
{
 return new TrollCobra(scr, xStart, yStart, secrt, 5);
}

void TrollCobra::die()
{
 IUShort num;
 TrollThing *item;
 TrollCharacter *troll = const_cast<TrollCharacter *>(screen->getCharacter());

 TrollStandardMonster::die();
 troll->addTotalHp(2);
 if (IRandom(3) == 0)
 {
  num = IRandom(10);
  if (num > 8)
  {
   item = TrollGold::create3GoldCoins(screen, x, y);
  }
  else if (num > 3)
  {
   item = TrollGold::createGoldCoin(screen, x, y);
  }
  else
  {
   item = TrollHitPoint::createHitPoint(screen, x, y);
  }
  screen->addItem(item);
 }
}

void TrollCobra::react()
{
 int i;
 bool pass = true;
 IUShort xCur, yCur;

 if (invincible)
 {
  invincible--;
 }
 time++;
 if (time == TROLL_COBRA_SPEED)
 {
  frame++;
  frame %= 2;
  time = 0;
  state++;
  if ((state == TROLL_COBRA_STATE_FIRE1) ||
    (state == TROLL_COBRA_STATE_FIRE2) ||
    (state == TROLL_COBRA_STATE_FIRE3))
  {
   const TrollCharacter *trll = screen->getCharacter();
   IShort xDiff = abs(trll->getX() - x - 6);
   IShort yDiff = abs(trll->getY() - y - 4);
   IShort xSpeed, ySpeed;
   if (xDiff < yDiff)
   {
    xSpeed =
      (IShort)(((float)xDiff) / yDiff * ((x + 6 < trll->getX()) ? 4 : -4));
    ySpeed = (4 - abs(xSpeed)) * ((y + 4 < trll->getY()) ? 1 : -1);
   }
   else
   {
    ySpeed =
      (IShort)(((float)yDiff) / xDiff * ((y + 4 < trll->getY()) ? 4 : -4));
    xSpeed = (4 - abs(ySpeed)) * ((x + 6 < trll->getX()) ? 1 : -1);
   }
   screen->addMonsterProjectile(new TrollBlast(screen, x + 6, y + 4, 1,
     xSpeed, ySpeed, 10));
   facing = 1;
  }
  if (state == TROLL_COBRA_STATE_REPEAT)
  {
   facing = 0;
   state = 0;
  }
 }
}

void TrollCobra::takeHit(TrollThing *hitBy)
{
 TrollStandardMonster::takeHit(hitBy);
 if (invincible == TROLL_INVINCIBLE_TIME)
 {
  state = TROLL_COBRA_STATE_PREFIRE;
 }
}

TrollCobra::TrollCobra(TrollScreen *scr, IUShort xStart, IUShort yStart,
  IUShort secrt, IUShort shft)
 : TrollStandardMonster(scr, secrt, 2)
{
 if ((xStart == TROLL_XYRANDOM) && (yStart == TROLL_XYRANDOM))
 {
  x = TROLL_CALCULATE_X_POS(7);
  y = TROLL_CALCULATE_Y_POS(2);
 }
 else
 {
  x = xStart;
  y = yStart;
 }
 sprite = TROLL_SPRITE_COBRA;
 shift = shft;
 direction = TROLL_DOWN;
 facing = 0;
 frame = 0;
 hp = 4;
 time = 0;
 state = TROLL_COBRA_STATE_WAITFULL;
}

