#ifndef __JUMPER_H
#define __JUMPER_H
/*-------------------------------------------------------------------------*\
  <jumper.h> -- Troll Bridge dynamically linked library - Jumpers

  Date      Programmer  Description
  03/14/99  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Classes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollJumper : public TrollStandardMonster
{
 public:
  static TrollThing *createGreenJumper(TrollScreen *scr, IShort sx,
    IShort sy, IUShort secrt);
  static TrollThing *createRedJumper(TrollScreen *scr, IShort sx,
    IShort sy, IUShort secrt);

  void die();
  void react();

 protected:
  TrollJumper(TrollScreen *scr, IShort sx, IShort sy, IUShort secrt,
    IUShort shft);

  IUShort time;
  IUShort state;
};

#endif

