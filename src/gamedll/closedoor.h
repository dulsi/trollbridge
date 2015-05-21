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
  static TrollThing *createNorthDoor(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt);
  static TrollThing *createSouthDoor(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt);
  static TrollThing *createWestDoor(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt);
  static TrollThing *createEastDoor(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt);

  void react();

 private:
  TrollClosingDoor(TrollScreen *scr, IUShort secrt, IShort xStart,
    IShort yStart, IUShort dir);

  IUShort state;
};

#endif

