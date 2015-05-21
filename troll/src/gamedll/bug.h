#ifndef __BUG_H
#define __BUG_H
/*-------------------------------------------------------------------------*\
  <bug.h> -- Troll Bridge dynamically linked library - Bugs

  Date      Programmer  Description
  12/13/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Classes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollBug : public TrollStandardMonster
{
 public:
  static TrollThing *createBlueBug(TrollScreen *scr, IUShort secrt = 0);
  static TrollThing *createRedBug(TrollScreen *scr, IUShort secrt = 0);

  void react();

 protected:
  TrollBug(TrollScreen *scr, IUShort secrt, IUShort shft);

  void changeSpeed();

  IShort xSpeed, ySpeed;
};

#endif

