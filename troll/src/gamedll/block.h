#ifndef __BLOCK_H
#define __BLOCK_H
/*-------------------------------------------------------------------------*\
  <block.h> -- Troll Bridge dynamically linked library - Movable  Block

  Date      Programmer  Description
  12/18/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Classes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollBlock: public TrollMonster
{
 public:
  static TrollThing *createBlock(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt = 0);

  void react();
  void takeHit(TrollThing *hitBy);

 private:
  TrollBlock(TrollScreen *scr, IUShort secrt, IShort xStart, IShort yStart);

  IUShort pushTime;
  IUShort lastFrame;
  bool moving;
};

#endif

