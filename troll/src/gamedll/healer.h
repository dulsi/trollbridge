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
  static TrollThing *createHealer(TrollScreen *scr, IUShort secrt = 0);

  void draw(IScreen drawscreen);
  void react();

 protected:
  TrollHealer(TrollScreen *scr, IUShort secrt);

  IUShort state;
};

#endif

