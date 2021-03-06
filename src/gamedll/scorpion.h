#ifndef __SCORPION_H
#define __SCORPION_H
/*-------------------------------------------------------------------------*\
  <scorpion.h> -- Troll Bridge dynamically linked library - Scorpions

  Date      Programmer  Description
  12/12/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Classes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollScorpion : public TrollStandardMonster
{
 public:
  static TrollThing *createBlueScorpion(TrollScreen *scr, IShort sx,
    IShort sy, IUShort secrt);
  static TrollThing *createRedScorpion(TrollScreen *scr, IShort sx,
    IShort sy, IUShort secrt);

  void die();
  void react();

 protected:
  TrollScorpion(TrollScreen *scr, IShort sx, IShort sy, IUShort secrt,
    IUShort shft, IUShort spd);

  IUShort speed;
  IUShort time;
  IUShort state;
};

#endif

