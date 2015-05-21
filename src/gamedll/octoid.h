#ifndef __OCTOID_H
#define __OCTOID_H
/*-------------------------------------------------------------------------*\
  <octoid.h> -- Troll Bridge dynamically linked library - Octoids

  Date      Programmer  Description
  03/15/99  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Classes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollOctoid : public TrollStandardMonster
{
 public:
  static TrollThing *createBlueOctoid(TrollScreen *scr, IShort sx,
    IShort sy, IUShort secrt = 0);

  void die();
  void react();

 protected:
  TrollOctoid(TrollScreen *scr, IShort sx, IShort sy, IUShort secrt,
    IUShort shft, IUShort spd);

  IUShort speed;
  IUShort time;
  IUShort state;
  IUShort fire;
};

#endif

