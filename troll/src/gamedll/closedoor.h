#ifndef __CLOSEDOOR_H
#define __CLOSEDOOR_H
/*-------------------------------------------------------------------------*\
  <closedoor.h> -- Troll Bridge dynamically linked library - Closing Door

  Date      Programmer  Description
  12/28/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Classes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollClosingDoor: public TrollMonster
{
 public:
  static TrollThing *createNorthDoor(TrollScreen *scr, IUShort xStart,
    IUShort yStart, IUShort secrt = 0);
  static TrollThing *createSouthDoor(TrollScreen *scr, IUShort xStart,
    IUShort yStart, IUShort secrt = 0);
  static TrollThing *createWestDoor(TrollScreen *scr, IUShort xStart,
    IUShort yStart, IUShort secrt = 0);
  static TrollThing *createEastDoor(TrollScreen *scr, IUShort xStart,
    IUShort yStart, IUShort secrt = 0);

  void react();

 private:
  TrollClosingDoor(TrollScreen *scr, IUShort secrt, IUShort xStart,
    IUShort yStart, IUShort dir);

  IUShort state;
};

#endif

