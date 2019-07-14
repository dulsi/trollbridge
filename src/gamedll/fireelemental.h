#ifndef __FIREELEMENTAL_H
#define __FIREELEMENTAL_H
/*-------------------------------------------------------------------------*\
  <fireelemental.h> -- Troll Bridge dynamically linked library - Fire Elementals

  Date      Programmer  Description
  07/07/19  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Classes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollFireElemental : public TrollStandardMonster
{
 public:
  static TrollThing *createFireElemental(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt);

  void die();
  void react();
  void takeHit(TrollThing *hitBy);

 protected:
  TrollFireElemental(TrollScreen *scr, IShort xStart, IShort yStart, IUShort secrt,
    IUShort shft);

  IUShort time;
  IUShort state;
};

class TrollLavaSquare : public TrollMonster
{
 public:
  static TrollThing *createLavaSquare(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt);

  IShort getDamage();
  void getSize(IShort &xSize, IShort &ySize) const;
  void react();

 protected:
  TrollLavaSquare(TrollScreen *scr, IShort xStart, IShort yStart, IUShort secrt,
    IUShort shft);

  IUShort state;
};

#endif

