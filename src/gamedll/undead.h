#ifndef __UNDEAD_H
#define __UNDEAD_H
/*-------------------------------------------------------------------------*\
  <undead.h> -- Troll Bridge dynamically linked library - Undead

  Date      Programmer  Description
  12/25/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Classes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollUndead : public TrollStandardMonster
{
 public:
  virtual void attack() = 0;
  void react();
  virtual void runAway() = 0;

 protected:
  TrollUndead(TrollScreen *scr, IUShort secrt, IUShort dmg);
};

class TrollSkeletalTroll : public TrollUndead
{
 public:
  static TrollThing *createSkeletalTroll(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt);

  void attack();
  void die();
  void runAway();

 protected:
  TrollSkeletalTroll(TrollScreen *scr, IShort xStart, IShort yStart,
    IUShort secrt);

  IUShort state;
  IUShort time;
};

class TrollSkeletalTrollWithItem: public TrollSkeletalTroll
{
 public:
  static TrollThing *createSkeletalTrollWithKey(TrollScreen *scr,
    IShort xStart, IShort yStart, IUShort secrt);

  void die();
  void draw(IScreen drawscreen);

 protected:
  TrollSkeletalTrollWithItem(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt, TrollItemConstructor icon,
    IUShort isprt, IUShort ishft);

  TrollItemConstructor itemConstructor;
  IUShort itemSprite;
  IUByte itemShift;
};

#endif

