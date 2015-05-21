/*-------------------------------------------------------------------------*\
  <octoid.C> -- Troll Bridge dynamically linked library - Octoids

  Date      Programmer  Description
  03/16/99  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"
#include "dllconst.h"
#include "octoid.h"
#include "explosion.h"
#include "blast.h"

#define TROLL_OCTOID_ONESTEP  3
#define TROLL_OCTOID_SETDIR   4
#define TROLL_OCTOID_WAIT     5
#define TROLL_OCTOID_SICKNESS 6

TrollThing *TrollOctoid::createBlueOctoid(TrollScreen *scr, IShort sx,
  IShort sy, IUShort secrt)
{
 return new TrollOctoid(scr, sx, sy, secrt, 0, 3);
}

void TrollOctoid::die()
{
 TrollStandardMonster::die();
 screen->addMonster(new TrollExplosion(screen, x, y, shift));
}

void TrollOctoid::react()
{
 int i;
 bool pass = true;
 IUShort xCur, yCur;

 if (invincible)
 {
  invincible--;
 }
 time++;
 if (state == TROLL_OCTOID_SICKNESS)
 {
  if (time == 16)
  {
   time = 0;
   state = IRandom(5) % 3;
  }
 }
 else if (state == TROLL_OCTOID_WAIT)
 {
  if (time == speed * 2)
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
   state = TROLL_OCTOID_ONESTEP;
  }
  time = 0;
 }
 else if (state == TROLL_OCTOID_SETDIR)
 {
  xCur = x / TROLL_SQUARE_X;
  yCur = (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y;
  i = IRandom(3);
  if (i == 0)
  {
   /*facing =*/ direction = IRandom(4);
  }
  else if (i == 1)
  {
   /*facing =*/ direction =
     (screen->getCharacter()->getX() < x) ? TROLL_LEFT : TROLL_RIGHT;
  }
  else
  {
   /*facing =*/ direction =
     (screen->getCharacter()->getY() < y) ? TROLL_UP : TROLL_DOWN;
  }
  pass = false;
  for (i = 0; (i < 4) && (!pass); i++)
  {
   pass = checkPassabilityFrom(xCur, yCur);
   if (!pass)
   {
    direction = (direction + 1) % 4;
//    facing = direction;
   }
  }
  state++;
 }
 else
 {
  if (time == speed)
  {
   if ((x % TROLL_SQUARE_X == 0) &&
     ((y - TROLL_BUFFER_Y) % TROLL_SQUARE_Y == 0))
   {
    xCur = x / TROLL_SQUARE_X;
    yCur = (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y;
/*    facing = direction = IRandom(4);
    pass = false;
    for (i = 0; (i < 4) && (!pass); i++)
    {*/
     pass = checkPassabilityFrom(xCur, yCur);
/*     if (!pass)
     {
      direction = (direction + 1) % 4;
      facing = direction;
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
   fire++;
   if (fire == 12)
   {
    screen->addMonsterProjectile(new TrollSpear(screen, x, y, damage,
      direction, shift));
    fire = 0;
   }
  }
 }
}

TrollOctoid::TrollOctoid(TrollScreen *scr, IShort sx, IShort sy,
  IUShort secrt, IUShort shft, IUShort spd)
 : TrollStandardMonster(scr, secrt, 1)
{
 if ((sx == TROLL_XYRANDOM) && (sy == TROLL_XYRANDOM))
 {
  findOpen();
 }
 else
 {
  x = sx;
  y = sy;
 }
 sprite = TROLL_SPRITE_OCTOID;
 shift = shft;
 facing = 0;
 direction = IRandom(4);
 frame = 0;
 hp = 2;
 speed = spd;
 time = 0;
 state = TROLL_OCTOID_SICKNESS;
 fire = 0;
}

