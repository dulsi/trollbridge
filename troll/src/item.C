/*-------------------------------------------------------------------------*\
  <item.C> -- Implemention for TrollItem methods

  Date      Programmer  Description
  04/17/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "troll.h"
#include <igrtext.h>

bool TrollItem::checkPickUp(TrollCharacter *troll)
{
 if (cost == 0)
 {
  return true;
 }

 IUShort gold = troll->getGold();

 if (gold >= cost)
 {
  troll->setGold(gold - cost);
  return true;
 }
 return false;
}

void TrollItem::draw(IScreen drawscreen)
{
 TrollThing::draw(drawscreen);
 if (cost != 0)
 {
  char number[10];

  sprintf(number, "%d", cost);
  ITextDraw(drawscreen, x - (strlen(number) * 4) + TROLL_SQUARE_X / 2,
    y + TROLL_SQUARE_Y + 2, 255, number);
 }
}

void TrollItem::react()
{
}

void TrollItem::takeHit(TrollThing *hitBy)
{
}

TrollItem::TrollItem(TrollScreen *scr, IUShort xStart, IUShort yStart,
 IUShort secrt = 0, IUShort cst = 0)
 :TrollThing(scr, secrt)
{
 x = xStart;
 y = yStart;
 cost = cst;
}

