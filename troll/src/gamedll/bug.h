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
  static TrollThing *createBlueBug(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt = 0);
  static TrollThing *createRedBug(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt = 0);

  void react();

 protected:
  TrollBug(TrollScreen *scr, IShort xStart, IShort yStart, IUShort secrt,
    IUShort shft);

  void changeSpeed();

  IShort xSpeed, ySpeed;
  IUShort state;
  IUShort time;
};

#endif

