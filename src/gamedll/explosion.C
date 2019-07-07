/*-------------------------------------------------------------------------*\
  <explosion.C> -- Troll Bridge dynamically linked library - Explosion

  Date      Programmer  Description
  02/14/99  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"
#include "dllconst.h"
#include "items.h"
#include "explosion.h"

TrollExplosion::TrollExplosion(TrollScreen *scr, IShort xStart,
  IShort yStart, IUShort shft, int chance /*= 3*/)
 : TrollMonster(scr)
{
 x = xStart;
 y = yStart;
 time = 0;
 sprite = TROLL_SPRITE_EXPLOSION;
 shift = shft;
 facing = direction = 0;
 frame = 0;
 time = 0;
 rewardChance = chance;
}

void TrollExplosion::die()
{
 IUShort num;
 TrollThing *item;

 TrollMonster::die();
 if ((rewardChance) && (IRandom(rewardChance) == 0))
 {
  num = IRandom(10);
  if (num > 8)
  {
   item = TrollGold::create3GoldCoins(screen, x, y, 0);
  }
  else if (num > 3)
  {
   item = TrollGold::createGoldCoin(screen, x, y, 0);
  }
  else
  {
   item = TrollHitPoint::createHitPoint(screen, x, y, 0);
  }
  screen->addItem(item);
 }
}

void TrollExplosion::react()
{
 int i;
 bool pass = true;
 IUShort xCur, yCur;

 time++;
 if (time == 4)
 {
  if (facing == 0)
  {
   facing = 1;
  }
  else
  {
   dead = 1;
  }
  time = 0;
 }
}

