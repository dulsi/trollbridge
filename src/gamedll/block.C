/*-------------------------------------------------------------------------*\
  <block.C> -- Troll Bridge dynamically linked library - Movable Block

  Date      Programmer  Description
  12/18/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"
#include "dllconst.h"
#include "block.h"

#define TROLL_PUSH_TIME 5

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollBlock::createBlock - Creates a movable block.

    Parameters:
      scr          (In)  Screen the door is on
      xStart       (In)  Starting X position
      yStart       (In)  Starting Y position
      secrt        (In)  Secret number to set when dead

    Returns: A movable block
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollBlock::createBlock(TrollScreen *scr, IShort xStart,
  IShort yStart, IUShort secrt)
{
 return
   new TrollBlock(scr, secrt, TROLL_CALCULATE_X_POS(5),
     TROLL_CALCULATE_Y_POS(5));
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollBlock::react - Have the block do something.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollBlock::react()
{
 if (!moving)
 {
  IShort tx, ty;
  IUShort tfacing, tframe;
  const TrollCharacter *troll = screen->getCharacter();

  troll->getLocation(tx, ty);
  tfacing = troll->getFacing();
  tframe = troll->getFrame();
  if ((tframe != lastFrame) &&
    (((ty == y) && (tx == x - TROLL_SQUARE_X) && (tfacing == TROLL_RIGHT)) ||
    ((ty == y) && (tx == x + TROLL_SQUARE_X) && (tfacing == TROLL_LEFT)) ||
    ((tx == x) && (ty == y - TROLL_SQUARE_Y) && (tfacing == TROLL_DOWN)) ||
    ((tx == x) && (ty == y + (TROLL_SQUARE_Y / 2)) && (tfacing == TROLL_UP))))
  {
   direction = tfacing;
   tx = x / TROLL_SQUARE_X;
   ty = (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y;
   if (checkPassabilityFrom(tx, ty))
   {
    pushTime++;
    if (pushTime == TROLL_PUSH_TIME)
    {
     moving = true;
     screen->setBackground(tx, ty, TROLL_SPRITE_SQUARE, shift);
     screen->setPassability(tx, ty, TROLL_OPEN);
     screen->setSecret(secret);
     secret = 0; // Stops secret from being run at death
    }
   }
  }
  else
  {
   pushTime = 0;
  }
  lastFrame = tframe;
 }
 else
 {
  switch (direction)
  {
   case TROLL_UP:
    y -= TROLL_SQUARE_Y / 4;
    break;
   case TROLL_DOWN:
    y += TROLL_SQUARE_Y / 4;
    break;
   case TROLL_RIGHT:
    x += TROLL_SQUARE_X / 4;
    break;
   case TROLL_LEFT:
    x -= TROLL_SQUARE_X / 4;
    break;
   default:
    // should never get here
    break;
  }
  if ((x % TROLL_SQUARE_X == 0) &&
    ((y - TROLL_BUFFER_Y) % TROLL_SQUARE_Y == 0))
  {
   dead = 1;
   screen->setBackground(x / TROLL_SQUARE_X,
     (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y, sprite, shift);
   screen->setPassability(x / TROLL_SQUARE_X,
     (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y, TROLL_BLOCKED);
  }
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollBlock::takeHit - Takes a hit from another thing.

    Parameters:
      hitBy        (In)  Thing that hit the person
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollBlock::takeHit(TrollThing *hitBy)
{
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollBlock::TrollBlock - Constructor for a movable block.

    Parameters:
      scr          (In)  Screen the block is on
      secrt        (In)  Secret number to set when pushed
      xStart       (In)  Starting x location
      yStart       (In)  Starting y location
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollBlock::TrollBlock(TrollScreen *scr, IUShort secrt, IShort xStart,
  IShort yStart) : TrollMonster(scr, secrt)
{
 scr->getBackground(xStart / TROLL_SQUARE_X,
   (yStart - TROLL_BUFFER_Y) / TROLL_SQUARE_Y, sprite, shift);
 facing = 0;
 frame = 0;
 x = xStart;
 y = yStart;
 pushTime = 0;
 lastFrame = 0;
 moving = false;
}

