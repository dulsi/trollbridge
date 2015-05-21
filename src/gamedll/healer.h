#ifndef __HEALER_H
#define __HEALER_H
/*-------------------------------------------------------------------------*\
  <healer.h> -- Troll Bridge dynamically linked library - Healer

  Date      Programmer  Description
  02/28/99  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Classes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollHealer: public TrollMonster
{
 public:
  static TrollThing *createHealer(TrollScreen *scr, IShort sx, IShort sy,
    IUShort secrt);

  void draw(IScreen drawscreen);
  void react();

 protected:
  TrollHealer(TrollScreen *scr, IShort sx, IShort sy, IUShort secrt);

  IUShort state;
};

#endif

