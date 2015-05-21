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
  static TrollThing *createDoor(TrollScreen *scr, IUShort xStart,
    IUShort yStart, IUShort secrt = 0);
  static TrollThing *createSouthDoor(TrollScreen *scr, IUShort xStart,
    IUShort yStart, IUShort secrt = 0);
  static TrollThing *createWestDoor(TrollScreen *scr, IUShort xStart,
    IUShort yStart, IUShort secrt = 0);
  static TrollThing *createEastDoor(TrollScreen *scr, IUShort xStart,
    IUShort yStart, IUShort secrt = 0);

  void react();
  void takeHit(TrollThing *hitBy);

 private:
  TrollDoor(TrollScreen *scr, IUShort secrt, IUShort xStart, IUShort yStart);
};

#endif

