/*-------------------------------------------------------------------------*\
  <blast.C> -- Troll Bridge dynamically linked library - Blasts

  Date      Programmer  Description
  01/24/99  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "troll.h"
#include "dllconst.h"
#include "blast.h"
#include "items.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollBlast::TrollBlast - Constructor for a blast.

    Parameters:
      screen       (In)  Screen the blast is on
      xStart       (In)  X location
      yStart       (In)  Y location
      dmg          (In)  Damage of the attack
      xSpd         (In)  X speed
      ySpd         (In)  Y speed
      shft         (In)  Color shift
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollBlast::TrollBlast(TrollScreen *screen, IUShort xStart, IUShort yStart,
  IUShort dmg, IShort xSpd, IShort ySpd, IUByte shft)
 : TrollProjectile(screen, dmg)
{
 sprite = TROLL_SPRITE_BLAST;
 facing = frame = 0;
 x = xStart;
 y = yStart;
 shift = shft;
 shift2 = shft + 1;
 xSpeed = xSpd;
 ySpeed = ySpd;
 if (abs(xSpeed) > abs(ySpeed))
 {
  if (xSpeed > 0)
  {
   direction = TROLL_LEFT;
  }
  else
  {
   direction = TROLL_RIGHT;
  }
 }
 else if (ySpeed > 0)
 {
  direction = TROLL_DOWN;
 }
 else
 {
  direction = TROLL_UP;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollBlast::react - Move the blast.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollBlast::react()
{
 IUByte tmpShift;
 const Sprite *pic = TrollSpriteHandler.getSprite(sprite);

 tmpShift = shift2;
 shift2 = shift;
 shift = tmpShift;
 x += xSpeed;
 y += ySpeed;
 if ((x < 0) || (y < TROLL_BUFFER_Y) || (x + pic->getXSize() > 319) ||
   (y + pic->getYSize() > 199))
 {
  dead = 1;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollBlast::takeHit - Called when the blast hits something.

    Parameters:
      hitBy        (In)  Thing it hit
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollBlast::takeHit(TrollThing *hitBy)
{
 TrollCharacter *trll;
 if ((trll = dynamic_cast<TrollCharacter *>(hitBy)))
 {
  dead = 1;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollStandardProjectile::TrollStandardProjectile - Constructor for a 
    standard projectile.

    Parameters:
      screen       (In)  Screen the blast is on
      dmg          (In)  Damage of the attack
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollStandardProjectile::TrollStandardProjectile(TrollScreen *screen,
  IUShort dmg = 1)
 : TrollProjectile(screen, dmg)
{
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollStandardProjectile::takeHit - Called when the projectile hits
    something.

    Parameters:
      hitBy        (In)  Thing it hit
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollStandardProjectile::takeHit(TrollThing *hitBy)
{
 TrollCharacter *trll;
 TrollShieldProjectile *shield;

 if (((trll = dynamic_cast<TrollCharacter *>(hitBy))) ||
    ((shield = dynamic_cast<TrollShieldProjectile *>(hitBy))))
 {
  dead = 1;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSpear::TrollSpear - Constructor for a spear.

    Parameters:
      screen       (In)  Screen the blast is on
      xStart       (In)  X location
      yStart       (In)  Y location
      dmg          (In)  Damage of the attack
      dir          (In)  Direction
      shft         (In)  Color shift
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollSpear::TrollSpear(TrollScreen *screen, IUShort xStart, IUShort yStart,
  IUShort dmg, IUShort dir, IUByte shft)
 : TrollStandardProjectile(screen, dmg)
{
 sprite = TROLL_SPRITE_SPEAR;
 frame = 0;
 x = xStart;
 y = yStart;
 shift = shft;
 facing = direction = dir;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSpear::react - Move the blast.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollSpear::react()
{
 const Sprite *pic = TrollSpriteHandler.getSprite(sprite);

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
 if ((x < 0) || (y < TROLL_BUFFER_Y) || (x + pic->getXSize() > 319) ||
   (y + pic->getYSize() > 199))
 {
  dead = 1;
 }
}

