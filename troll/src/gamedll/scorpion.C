/*-------------------------------------------------------------------------*\
  <scorpion.C> -- Troll Bridge dynamically linked library - Scorpions

  Date      Programmer  Description
  12/12/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"
#include "dllconst.h"
#include "scorpion.h"
#include "explosion.h"

#define TROLL_SCORPION_ONESTEP  3
#define TROLL_SCORPION_SETDIR   4
#define TROLL_SCORPION_WAIT     5
#define TROLL_SCORPION_SICKNESS 6

TrollThing *TrollScorpion::createBlueScorpion(TrollScreen *scr,
  IUShort secrt = 0)
{
 return new TrollScorpion(scr, secrt, 0, 3);
}

TrollThing *TrollScorpion::createRedScorpion(TrollScreen *scr,
  IUShort secrt = 0)
{
 return new TrollScorpion(scr, secrt, 10, 2);
}

void TrollScorpion::die()
{
 TrollStandardMonster::die();
 screen->addMonster(new TrollExplosion(screen, x, y, shift));
}

void TrollScorpion::react()
{
 int i;
 bool pass = true;
 IUShort xCur, yCur;

 if (invincible)
 {
  invincible--;
 }
 time++;
 if (state == TROLL_SCORPION_SICKNESS)
 {
  if (time == 16)
  {
   time = 0;
   state = IRandom(5) % 3;
  }
 }
 else if (state == TROLL_SCORPION_WAIT)
 {
  if (time == speed * 4)
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
   state = TROLL_SCORPION_ONESTEP;
  }
  time = 0;
 }
 else if (state == TROLL_SCORPION_SETDIR)
 {
  xCur = x / TROLL_SQUARE_X;
  yCur = (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y;
  i = IRandom(3);
  if (i == 0)
  {
   facing = direction = IRandom(4);
  }
  else if (i == 1)
  {
   facing = direction =
     (screen->getCharacter()->getX() < x) ? TROLL_LEFT : TROLL_RIGHT;
  }
  else
  {
   facing = direction =
     (screen->getCharacter()->getY() < y) ? TROLL_UP : TROLL_DOWN;
  }
  pass = false;
  for (i = 0; (i < 4) && (!pass); i++)
  {
   pass = checkPassabilityFrom(xCur, yCur);
   if (!pass)
   {
    direction = (direction + 1) % 4;
    facing = direction;
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
  }
 }
}

TrollScorpion::TrollScorpion(TrollScreen *scr, IUShort secrt, IUShort shft,
  IUShort spd)
 : TrollStandardMonster(scr, secrt, 1)
{
 findOpen();
 sprite = TROLL_SPRITE_SCORPION;
 shift = shft;
 facing = direction = IRandom(4);
 frame = 0;
 hp = 1;
 speed = spd;
 time = 0;
 state = TROLL_SCORPION_SICKNESS;
}

