#ifndef __COBRA_H
#define __COBRA_H
/*-------------------------------------------------------------------------*\
  <cobra.h> -- Troll Bridge dynamically linked library - Cobras

  Date      Programmer  Description
  01/23/99  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Classes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollCobra : public TrollStandardMonster
{
 public:
  static TrollThing *createGreenCobra(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt = 0);

  void die();
  void react();
  void takeHit(TrollThing *hitBy);

 protected:
  TrollCobra(TrollScreen *scr, IShort xStart, IShort yStart, IUShort secrt,
    IUShort shft);

  IUShort time;
  IUShort state;
};

#endif

