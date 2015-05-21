#ifndef __DOOR_H
#define __DOOR_H
/*-------------------------------------------------------------------------*\
  <door.h> -- Troll Bridge dynamically linked library - Door

  Date      Programmer  Description
  12/08/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Classes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollDoor: public TrollMonster
{
 public:
  static TrollThing *createDoor(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt);
  static TrollThing *createSouthDoor(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt);
  static TrollThing *createWestDoor(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt);
  static TrollThing *createEastDoor(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt);

  void react();
  void takeHit(TrollThing *hitBy);

 private:
  TrollDoor(TrollScreen *scr, IUShort secrt, IShort xStart, IShort yStart);
};

#endif

