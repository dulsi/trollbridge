/*-------------------------------------------------------------------------*\
  <carrieditem.C> -- Implemention for TrollCarriedItem methods

  Date      Programmer  Description
  04/17/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "troll.h"

TrollCarriedItem::TrollCarriedItem(IUShort num, IUShort slt, IUShort str)
{
 // Derived classes must make sure to set the sprite and shift of the object
 itemNum = num;
 slot = slt;
 strength = str;
}

TrollCarriedItem::~TrollCarriedItem()
{
}

void TrollCarriedItem::draw(IScreen drawscreen, IUShort x, IUShort y)
{
 const Sprite *itemSprite = TrollSpriteHandler.getSprite(sprite);

 itemSprite->draw(drawscreen, x - (itemSprite->getXSize() / 2),
    y - (itemSprite->getYSize() / 2), 0, 0, shift);
}

IUShort TrollCarriedItem::getItemNum()
{
 return itemNum;
}

IUShort TrollCarriedItem::getSlot()
{
 return slot;
}

IUShort TrollCarriedItem::getStrength()
{
 return strength;
}

IUShort TrollCarriedItem::getSprite()
{
 return sprite;
}

IUShort TrollCarriedItem::getShift()
{
 return shift;
}

void TrollCarriedItem::load(BinaryReadFile &f)
{
 // no default extra information
}

void TrollCarriedItem::save(BinaryWriteFile &f)
{
 // no default extra information
}

