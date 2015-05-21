#ifndef __EXPLOSION_H
#define __EXPLOSION_H
/*-------------------------------------------------------------------------*\
  <explosion.h> -- Troll Bridge dynamically linked library - Explosion

  Date      Programmer  Description
  02/14/99  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Classes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollExplosion : public TrollMonster
{
 public:
  TrollExplosion(TrollScreen *scr, IShort xStart, IShort yStart,
    IUShort shft);

  void die();
  void react();

 protected:
  IUShort time;
};

#endif

