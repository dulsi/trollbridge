/*-------------------------------------------------------------------------*\
  <items.C> -- Troll Bridge dynamically linked library - Items

  Date      Programmer  Description
  04/17/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "troll.h"
#include "dllconst.h"
#include "solditems.h"
#include <igrtext.h>

bool TrollSoldItem::checkPickUp(TrollCharacter *troll)
{
 IUShort gold = troll->getGold();

 if (gold >= cost)
 {
  troll->setGold(gold - cost);
  return true;
 }
 return false;
}

void TrollSoldItem::draw(IScreen drawscreen, IUShort x, IUShort y)
{
 char number[10];

 sprintf(number, "%d", cost);
 ITextDraw(drawscreen, x - (strlen(number) * 4) + TROLL_SQUARE_X / 2,
   y + TROLL_SQUARE_Y + 2, 255, (unsigned char *)number);
}

TrollSoldItem::TrollSoldItem(IUShort cst)
{
 cost = cst;
}

TrollThing *TrollSoldActivatibleItem::createShield(TrollScreen *scr,
  IUShort x, IUShort y, IUShort secrt = 0)
{
 return new TrollSoldActivatibleItem(scr, x, y, secrt, &TrollShield, 70);
}

void TrollSoldActivatibleItem::draw(IScreen drawscreen)
{
 TrollSoldItem::draw(drawscreen, x, y);
 TrollItem::draw(drawscreen);
}

void TrollSoldActivatibleItem::pickUp(TrollCharacter *troll)
{
 if (TrollSoldItem::checkPickUp(troll))
 {
  TrollActivatibleItem::pickUp(troll);
  dead = 1;
 }
}

void TrollSoldActivatibleItem::takeHit(TrollThing *hitBy)
{
}

TrollSoldActivatibleItem::TrollSoldActivatibleItem(TrollScreen *scr,
  IUShort xStart, IUShort yStart, IUShort secrt, TrollCarriedItem *actItem,
  IUShort cst)
  : TrollActivatibleItem(scr, xStart, yStart, secrt, actItem),
    TrollSoldItem(cst)
{
}

TrollThing *TrollSoldLimitedActivatibleItem::createKey(TrollScreen *scr,
  IUShort x, IUShort y, IUShort secrt = 0)
{
 return new TrollSoldLimitedActivatibleItem(scr, x, y, secrt, &TrollKey, 60);
}

void TrollSoldLimitedActivatibleItem::draw(IScreen drawscreen)
{
 TrollSoldItem::draw(drawscreen, x, y);
 TrollItem::draw(drawscreen);
}

void TrollSoldLimitedActivatibleItem::pickUp(TrollCharacter *troll)
{
 if (TrollSoldItem::checkPickUp(troll))
 {
  TrollLimitedActivatibleItem::pickUp(troll);
  dead = 1;
 }
}

void TrollSoldLimitedActivatibleItem::takeHit(TrollThing *hitBy)
{
}

TrollSoldLimitedActivatibleItem::TrollSoldLimitedActivatibleItem(TrollScreen *scr,
  IUShort xStart, IUShort yStart, IUShort secrt, TrollCarriedItem *actItem,
  IUShort cst)
  : TrollLimitedActivatibleItem(scr, xStart, yStart, secrt, actItem),
    TrollSoldItem(cst)
{
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSoldHitPoint::createHitPoint - Creates a hit point.

    Parameters:
      scr          (In)  Screen the hit point is on
      x            (In)  X location
      y            (In)  Y location
      secrt        (In)  Secret number to set when dead

    Returns: A hit point
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollSoldHitPoint::createHitPoint(TrollScreen *scr, IUShort x,
  IUShort y, IUShort secrt = 0)
{
 return new TrollSoldHitPoint(scr, x, y, secrt, 1, 20);
}

void TrollSoldHitPoint::draw(IScreen drawscreen)
{
 TrollSoldItem::draw(drawscreen, x, y);
 TrollItem::draw(drawscreen);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSoldHitPoints::pickUp - Gives the hit points to the character.

    Parameters:
      troll        (In)  Character to get the item
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollSoldHitPoint::pickUp(TrollCharacter *troll)
{
 if (TrollSoldItem::checkPickUp(troll))
 {
  troll->addHp(amount);
  dead = 1;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSoldHitPoints::react - Performs animation sequence for hit points.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollSoldHitPoint::react()
{
 time++;
 if (time == 6)
 {
  time = 0;
  shift += 5;
  shift %= 15;
 }
 TrollItem::react();
}

void TrollSoldHitPoint::takeHit(TrollThing *hitBy)
{
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSoldHitPoint::TrollSoldHitPoint - Constructor for hit points.

    Parameters:
      scr          (In)  Screen the hit points are on
      x            (In)  X location
      y            (In)  Y location
      secrt        (In)  Secret number to set when dead
      amt          (In)  Amount of hit points
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollSoldHitPoint::TrollSoldHitPoint(TrollScreen *scr, IUShort xStart,
  IUShort yStart, IUShort secrt, IUShort amt, IUShort cst)
 : TrollItem(scr, xStart, yStart, secrt), TrollSoldItem(cst)
{
 sprite = TROLL_SPRITE_HITPOINT;
 amount = amt;
 shift = 0;
 time = 0;
 facing = 0;
 frame = 0;
}

