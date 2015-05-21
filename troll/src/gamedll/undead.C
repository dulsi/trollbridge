/*-------------------------------------------------------------------------*\
  <undead.C> -- Troll Bridge dynamically linked library - Undead

  Date      Programmer  Description
  12/25/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"
#include "dllconst.h"
#include "items.h"
#include "undead.h"
#include "explosion.h"

#define TROLL_SKELETALTROLL_SPEED 2

#define TROLL_SKELETALTROLL_ONESTEP  3
#define TROLL_SKELETALTROLL_SETDIR   4
#define TROLL_SKELETALTROLL_WAIT     5
#define TROLL_SKELETALTROLL_SICKNESS 6

void TrollUndead::react()
{
 if (TrollAnhkItem::checkActiveAnhk(screen))
 {
  runAway();
 }
 else
 {
  attack();
 }
}

TrollUndead::TrollUndead(TrollScreen *scr, IUShort secrt, IUShort dmg)
 : TrollStandardMonster(scr, secrt, dmg)
{
 // Nothing
}

TrollThing *TrollSkeletalTroll::createSkeletalTroll(TrollScreen *scr,
  IUShort secrt = 0)
{
 return new TrollSkeletalTroll(scr, secrt);
}

void TrollSkeletalTroll::attack()
{
 int i;
 bool pass = true;
 IUShort xCur, yCur;

 if (invincible)
 {
  invincible--;
 }
 time++;
 if (state == TROLL_SKELETALTROLL_SICKNESS)
 {
  if (time == 16)
  {
   time = 0;
   state = IRandom(5) % 3;
  }
 }
 else if (flyBack)
 {
  pass = true;
  if ((x % TROLL_SQUARE_X == 0) &&
    ((y - TROLL_BUFFER_Y) % TROLL_SQUARE_Y == 0))
  {
   xCur = x / TROLL_SQUARE_X;
   yCur = (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y;
   pass = checkPassabilityFrom(xCur, yCur);
  }
  if (pass)
  {
   move();
   frame++;
   frame %= 2;
  }
  flyBack--;
  if (flyBack == 0)
  {
   state = TROLL_SKELETALTROLL_ONESTEP;
  }
  time = 0;
 }
 else if (state == TROLL_SKELETALTROLL_SETDIR)
 {
  xCur = x / TROLL_SQUARE_X;
  yCur = (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y;
  i = IRandom(3);
  if (i == 0)
  {
   direction = IRandom(4);
  }
  else if (i == 1)
  {
   direction =
     (screen->getCharacter()->getX() < x) ? TROLL_LEFT : TROLL_RIGHT;
  }
  else
  {
   direction =
     (screen->getCharacter()->getY() < y) ? TROLL_UP : TROLL_DOWN;
  }
  pass = false;
  for (i = 0; (i < 4) && (!pass); i++)
  {
   pass = checkPassabilityFrom(xCur, yCur);
   if (!pass)
   {
    direction = (direction + 1) % 4;
   }
  }
  state++;
  time = 0;
 }
 else
 {
  if (state == TROLL_SKELETALTROLL_WAIT)
  {
   time = 0;
   state = IRandom(5) % 3;
  }
  if (time == TROLL_SKELETALTROLL_SPEED)
  {
   if ((x % TROLL_SQUARE_X == 0) &&
     ((y - TROLL_BUFFER_Y) % TROLL_SQUARE_Y == 0))
   {
    xCur = x / TROLL_SQUARE_X;
    yCur = (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y;
/*    direction = IRandom(4);
    pass = false;
    for (i = 0; (i < 4) && (!pass); i++)
    {*/
     pass = checkPassabilityFrom(xCur, yCur);
/*     if (!pass)
     {
      direction = (direction + 1) % 4;
     }
    }*/
    state++;
   }
   if ((state != 4) && (pass))
   {
    move();
    frame++;
    frame %= 2;
   }
   time = 0;
  }
 }
}

void TrollSkeletalTroll::die()
{
 TrollStandardMonster::die();
 screen->addMonster(new TrollExplosion(screen, x, y, 4));
}

void TrollSkeletalTroll::runAway()
{
 bool pass = true;
 IUShort xCur, yCur;
 IShort xDiff, yDiff;

 if (invincible)
 {
  invincible--;
 }
 time++;
 if (flyBack)
 {
  pass = true;
  if ((x % TROLL_SQUARE_X == 0) &&
    ((y - TROLL_BUFFER_Y) % TROLL_SQUARE_Y == 0))
  {
   xCur = x / TROLL_SQUARE_X;
   yCur = (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y;
   pass = checkPassabilityFrom(xCur, yCur);
  }
  if (pass)
  {
   move();
   frame++;
   frame %= 2;
  }
  flyBack--;
  time = 0;
 }
 else if (state == 4)
 {
  xCur = x / TROLL_SQUARE_X;
  yCur = (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y;
  xDiff = (IShort)screen->getCharacter()->getX() - (IShort)x;
  yDiff = (IShort)screen->getCharacter()->getY() - (IShort)y;
  if ((abs(xDiff) < abs(yDiff)))
  {
   if (xDiff == 0)
   {
    direction = (IRandom(10) > 4) ? TROLL_LEFT : TROLL_RIGHT;
   }
   else if (xDiff > 0)
   {
    direction = TROLL_LEFT;
   }
   else
   {
    direction = TROLL_RIGHT;
   }
  }
  else
  {
   if (yDiff == 0)
   {
    direction = (IRandom(10) > 4) ? TROLL_UP : TROLL_DOWN;
   }
   else if (yDiff > 0)
   {
    direction = TROLL_UP;
   }
   else
   {
    direction = TROLL_DOWN;
   }
  }
  state++;
  time = 0;
 }
 else
 {
  if (state == 5)
  {
   time = 0;
   state = IRandom(5) % 3;
  }
  if (time == TROLL_SKELETALTROLL_SPEED)
  {
   if ((x % TROLL_SQUARE_X == 0) &&
     ((y - TROLL_BUFFER_Y) % TROLL_SQUARE_Y == 0))
   {
    xCur = x / TROLL_SQUARE_X;
    yCur = (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y;
/*    direction = IRandom(4);
    pass = false;
    for (i = 0; (i < 4) && (!pass); i++)
    {*/
     pass = checkPassabilityFrom(xCur, yCur);
/*     if (!pass)
     {
      direction = (direction + 1) % 4;
     }
    }*/
    state++;
   }
   if ((state != 4) && (pass))
   {
    move();
    frame++;
    frame %= 2;
   }
   time = 0;
  }
 }
}

TrollSkeletalTroll::TrollSkeletalTroll(TrollScreen *scr, IUShort secrt)
 : TrollUndead(scr, secrt, 1)
{
 findOpen();
 sprite = TROLL_SPRITE_SKELTROLL;
 shift = 0;
 facing = 0;
 frame = 0;
 hp = 2;
// speed = spd;
 time = 0;
 state = TROLL_SKELETALTROLL_SICKNESS;
}

TrollThing *TrollSkeletalTrollWithItem::createSkeletalTrollWithKey(
  TrollScreen *scr, IUShort secrt = 0)
{
 return new TrollSkeletalTrollWithItem(scr, secrt,
   TrollLimitedActivatibleItem::createKey, TROLL_SPRITE_KEY, 0);
}

void TrollSkeletalTrollWithItem::die()
{
 screen->addItem((*itemConstructor)(screen, x, y, secret));
 secret = 0;
 TrollSkeletalTroll::die();
}

void TrollSkeletalTrollWithItem::draw(IScreen drawscreen)
{
 if (!(invincible & 2))
 {
  const Sprite *pic = TrollSpriteHandler.getSprite(itemSprite);
  pic->draw(drawscreen, x, y, 0, 0, itemShift);
 }
 TrollSkeletalTroll::draw(drawscreen);
}

TrollSkeletalTrollWithItem::TrollSkeletalTrollWithItem(TrollScreen *scr,
  IUShort secrt, TrollItemConstructor icon, IUShort isprt, IUShort ishft)
 : TrollSkeletalTroll(scr, secrt)
{
 itemConstructor = icon;
 itemSprite = isprt;
 itemShift = ishft;
}

