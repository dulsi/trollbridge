#ifndef __BLAST_H
#define __BLAST_H
/*-------------------------------------------------------------------------*\
  <blast.h> -- Troll Bridge dynamically linked library - Blasts

  Date      Programmer  Description
  01/24/99  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Classes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollBlast: public TrollProjectile
{
 public:
  TrollBlast(TrollScreen *screen, IShort xStart, IShort yStart,
    IUShort dmg, IShort xSpd, IShort ySpd, IUByte shft);
  void react();
  void takeHit(TrollThing *hitBy);

 protected:
  IShort xSpeed;
  IShort ySpeed;
  IUByte shift2;
};

class TrollStandardProjectile: public TrollProjectile
{
 public:
  TrollStandardProjectile(TrollScreen *screen, IUShort dmg = 1);
  void takeHit(TrollThing *hitBy);
};

class TrollSpear: public TrollStandardProjectile
{
 public:
  TrollSpear(TrollScreen *screen, IShort xStart, IShort yStart,
    IUShort dmg, IUShort dir, IUByte shft);
  void react();
};

#endif

