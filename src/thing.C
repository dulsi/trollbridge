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
 if ((a->x + (TrollSpriteHandler.getSprite(a->sprite))->getXSize() > b->x) &&
   (a->x < b->x + (TrollSpriteHandler.getSprite(b->sprite))->getXSize()) &&
   (a->y + (TrollSpriteHandler.getSprite(a->sprite))->getYSize() > b->y) &&
   (a->y < b->y + (TrollSpriteHandler.getSprite(b->sprite))->getYSize()))
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

