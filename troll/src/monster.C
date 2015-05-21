/*-------------------------------------------------------------------------*\
  <monster.C> -- Implemention for TrollMonster methods

  Date      Programmer  Description
  04/17/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "troll.h"

IShort TrollMonster::getDamage()
{
 return 0;
}

void TrollMonster::takeHit(TrollThing *hitBy)
{
}

TrollMonster::TrollMonster(TrollScreen *scr, IUShort secrt = 0)
 : TrollThing(scr, secrt)
{
}

bool TrollMonster::checkPassabilityFrom(IUShort xCur, IUShort yCur)
{
 switch (direction)
 {
  case TROLL_UP:
   if ((yCur > 0) &&
     (screen->getPassability(xCur, yCur - 1) <= TROLL_NOMONSTER))
   {
    return true;
   }
   break;
  case TROLL_DOWN:
   if ((yCur < TROLL_SCREEN_Y - 1) &&
     (screen->getPassability(xCur, yCur + 1) <= TROLL_NOMONSTER))
   {
    return true;
   }
   break;
  case TROLL_LEFT:
   if ((xCur > 0) &&
     (screen->getPassability(xCur - 1, yCur) <= TROLL_NOMONSTER))
   {
    return true;
   }
   break;
  case TROLL_RIGHT:
   if ((xCur < TROLL_SCREEN_X - 1) &&
     (screen->getPassability(xCur + 1, yCur) <= TROLL_NOMONSTER))
   {
    return true;
   }
   break;
  default:
   // should never get here
   break;
 }
 return false;
}

void TrollMonster::findOpen()
{
 int i, k;

 for (i = IRandom(TROLL_SCREEN_X * TROLL_SCREEN_Y) + 1, k = 0; i > 0; k++)
 {
  k %= TROLL_SCREEN_X * TROLL_SCREEN_Y;
  x = k % TROLL_SCREEN_X;
  y = k / TROLL_SCREEN_X;
  if (screen->getPassability(x, y) <= TROLL_OPEN)
  {
   i--;
  }
 }
 x *= TROLL_SQUARE_X;
 y = y * TROLL_SQUARE_Y + TROLL_BUFFER_Y;
}

void TrollMonster::move()
{
 switch (direction)
 {
  case TROLL_UP:
   y -= TROLL_SQUARE_Y / 4;
   break;
  case TROLL_DOWN:
   y += TROLL_SQUARE_Y / 4;
   break;
  case TROLL_RIGHT:
   x += TROLL_SQUARE_X / 4;
   break;
  case TROLL_LEFT:
   x -= TROLL_SQUARE_X / 4;
   break;
  default:
   // should never get here
   break;
 }
}

void TrollStandardMonster::draw(IScreen drawscreen)
{
 if (!(invincible & 2))
 {
  TrollThing::draw(drawscreen);
 }
}

IShort TrollStandardMonster::getDamage()
{
 return damage;
}

void TrollStandardMonster::takeHit(TrollThing *hitBy)
{
 TrollProjectile *proj;
 IUShort projDirection;

 if (isDead() || invincible) return;
 if ((proj = (dynamic_cast<TrollProjectile *>(hitBy))))
 {
  if (proj->getDamage())
  {
   invincible = TROLL_INVINCIBLE_TIME;
   projDirection = proj->getDirection();
   if ((((direction == TROLL_UP) || (direction == TROLL_DOWN)) &&
     ((projDirection == TROLL_UP) || (projDirection == TROLL_DOWN))) ||
     (((direction == TROLL_RIGHT) || (direction == TROLL_LEFT)) &&
     ((projDirection == TROLL_RIGHT) || (projDirection == TROLL_LEFT))))
   {
    direction = projDirection;
    flyBack = TROLL_FLYBACK_TIME;
   }
   hp -= proj->getDamage();
   if (hp < 1)
   {
    dead = 1;
   }
  }
 }
}

TrollStandardMonster::TrollStandardMonster(TrollScreen *scr,
  IUShort secrt = 0,   IShort dmg = 1)
 : TrollMonster(scr, secrt)
{
 damage = dmg;
 direction = 0;
 flyBack = 0;
 invincible = 0;
}

