/*-------------------------------------------------------------------------*\
  <thing.C> -- Implemention for TrollThing methods

  Date      Programmer  Description
  04/17/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "troll.h"

TrollThing::~TrollThing()
{
 direction = 0;
}

void TrollThing::die()
{
 if (secret)
 {
  screen->setSecret(secret);
 }
}

void TrollThing::draw(IScreen drawscreen)
{
 const Sprite *pic = TrollSpriteHandler.getSprite(sprite);
 if (pic)
  pic->draw(drawscreen, x, y, frame, facing, shift);
}

IUShort TrollThing::getDirection() const
{
 return(direction);
}

void TrollThing::getLocation(IShort &xLoc, IShort &yLoc) const
{
 xLoc = x;
 yLoc = y;
}

void TrollThing::getSize(IShort &xSize, IShort &ySize) const
{
 const Sprite *spr = TrollSpriteHandler.getSprite(sprite);
 xSize = spr->getXSize();
 ySize = spr->getYSize();
}

bool TrollThing::isDead() const
{
 return(dead);
}

bool TrollThing::isKeep() const
{
 return(keep);
}

void TrollThing::setDead(IUByte value)
{
 dead = value;
}

void TrollThing::setLocation(IShort xNew, IShort yNew)
{
 x = xNew;
 y = yNew;
}

bool TrollThing::checkCollision(const TrollThing *a, const TrollThing *b)
{
 IShort aXSize, aYSize, bXSize, bYSize;
 a->getSize(aXSize, aYSize);
 b->getSize(bXSize, bYSize);
 if ((a->x + aXSize > b->x) &&
   (a->x < b->x + bXSize) &&
   (a->y + aYSize > b->y) &&
   (a->y < b->y + bYSize))
 {
  return 1;
 }
 else return 0;
}

TrollThing::TrollThing(TrollScreen *scr, IUShort secrt /*= 0*/)
{
 screen = scr;
 secret = secrt;
 dead = 0;
 keep = false;
}

