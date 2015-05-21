/*-------------------------------------------------------------------------*\
  <projectile.C> -- Implemention for TrollProjectile methods

  Date      Programmer  Description
  04/17/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "troll.h"

IShort TrollProjectile::getDamage()
{
 return(damage);
}

TrollProjectile::TrollProjectile(TrollScreen *scr, IUShort dmg /*= 1*/)
  :TrollThing(scr)
{
 damage = dmg;
}

