#ifndef __SOLDITEMS_H
#define __SOLDITEMS_H
/*-------------------------------------------------------------------------*\
  <solditems.h> -- Troll Bridge dynamically linked library - Items

  Date      Programmer  Description
  01/02/99  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"
#include "items.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Classes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollSoldItem
{
 public:
  bool checkPickUp(TrollCharacter *troll);
  void draw(IScreen drawscreen, IUShort x, IUShort y);

 protected:
  TrollSoldItem(IUShort cst);

  IUShort cost;
};

class TrollSoldActivatibleItem: public TrollActivatibleItem, public TrollSoldItem
{
 public:
  static TrollThing *createShield(TrollScreen *scr, IUShort x,
    IUShort y, IUShort secrt = 0);

  void draw(IScreen drawscreen);
  void pickUp(TrollCharacter *troll);
  void takeHit(TrollThing *hitBy);

 protected:
  TrollSoldActivatibleItem(TrollScreen *scr, IUShort xStart,
    IUShort yStart, IUShort secrt, TrollCarriedItem *actItem, IUShort cst);
};

class TrollSoldLimitedActivatibleItem: public TrollLimitedActivatibleItem, public TrollSoldItem
{
 public:
  static TrollThing *createKey(TrollScreen *scr, IUShort x,
    IUShort y, IUShort secrt = 0);

  void draw(IScreen drawscreen);
  void pickUp(TrollCharacter *troll);
  void takeHit(TrollThing *hitBy);

 protected:
  TrollSoldLimitedActivatibleItem(TrollScreen *scr, IUShort xStart,
    IUShort yStart, IUShort secrt, TrollCarriedItem *actItem, IUShort cst);
};

class TrollSoldHitPoint: public TrollItem, public TrollSoldItem
{
 public:
  static TrollThing *createHitPoint(TrollScreen *scr, IUShort x,
    IUShort y, IUShort secrt = 0);

  void draw(IScreen drawscreen);
  void pickUp(TrollCharacter *troll);
  void react();
  void takeHit(TrollThing *hitBy);

 protected:
  TrollSoldHitPoint(TrollScreen *scr, IUShort xStart, IUShort yStart,
    IUShort secrt, IUShort amt, IUShort cst);

  IUShort amount;
  IUShort time;
};

#endif

