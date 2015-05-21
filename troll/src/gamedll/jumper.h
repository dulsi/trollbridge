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
  static TrollThing *createGreenJumper(TrollScreen *scr, IUShort secrt = 0);
  static TrollThing *createRedJumper(TrollScreen *scr, IUShort secrt = 0);

  void die();
  void react();

 protected:
  TrollJumper(TrollScreen *scr, IUShort secrt, IUShort shft);

  IUShort time;
  IUShort state;
};

#endif

