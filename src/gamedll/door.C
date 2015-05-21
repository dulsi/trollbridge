/*-------------------------------------------------------------------------*\
  <door.C> -- Troll Bridge dynamically linked library - Door

  Date      Programmer  Description
  12/08/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"
#include "dllconst.h"
#include "door.h"
#include "items.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollDoor::createDoor - Creates a door.

    Parameters:
      scr          (In)  Screen the door is on
      xStart       (In)  Starting X position
      yStart       (In)  Starting Y position
      secrt        (In)  Secret number to set when dead

    Returns: A door
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollDoor::createDoor(TrollScreen *scr, IShort xStart,
  IShort yStart, IUShort secrt)
{
 return
   new TrollDoor(scr, secrt, xStart, yStart);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollDoor::createSouthDoor - Creates a door to the South.

    Parameters:
      scr          (In)  Screen the door is on
      xStart       (In)  Starting X position
      yStart       (In)  Starting Y position
      secrt        (In)  Secret number to set when dead

    Returns: A door
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollDoor::createSouthDoor(TrollScreen *scr, IShort xStart,
  IShort yStart, IUShort secrt)
{
 return
   new TrollDoor(scr, secrt, 140, TROLL_CALCULATE_Y_POS(TROLL_SCREEN_Y - 1));
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollDoor::createWestDoor - Creates a door to the West.

    Parameters:
      scr          (In)  Screen the door is on
      xStart       (In)  Starting X position
      yStart       (In)  Starting Y position
      secrt        (In)  Secret number to set when dead

    Returns: A door
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollDoor::createWestDoor(TrollScreen *scr, IShort xStart,
  IShort yStart, IUShort secrt)
{
 return new TrollDoor(scr, secrt, TROLL_CALCULATE_X_POS(0),
   TROLL_CALCULATE_Y_POS(5));
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollDoor::createEastDoor - Creates a door to the East.

    Parameters:
      scr          (In)  Screen the door is on
      xStart       (In)  Starting X position
      yStart       (In)  Starting Y position
      secrt        (In)  Secret number to set when dead

    Returns: A door
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollDoor::createEastDoor(TrollScreen *scr, IShort xStart,
  IShort yStart, IUShort secrt)
{
 return new TrollDoor(scr, secrt, TROLL_CALCULATE_X_POS(TROLL_SCREEN_X - 1),
   TROLL_CALCULATE_Y_POS(5));
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollDoor::react - Have the door do something.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollDoor::react()
{
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollDoor::takeHit - Takes a hit from another thing.

    Parameters:
      hitBy        (In)  Thing that hit the person
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollDoor::takeHit(TrollThing *hitBy)
{
 TrollKeyProjectile *key;

 if (isDead()) return;
 if ((key = (dynamic_cast<TrollKeyProjectile *>(hitBy))))
 {
  key->decreaseNumber();
  dead = 1;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollDoor::TrollDoor - Constructor for a door.

    Parameters:
      scr          (In)  Screen the door is on
      secrt        (In)  Secret number to set when dead
      xStart       (In)  Starting x location
      yStart       (In)  Starting y location
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollDoor::TrollDoor(TrollScreen *scr, IUShort secrt, IShort xStart,
  IShort yStart) : TrollMonster(scr, secrt)
{
 if ((xStart == TROLL_XYRANDOM) && (yStart == TROLL_XYRANDOM))
 {
  x = 140;
  y = 40;
 }
 else
 {
  x = xStart;
  y = yStart;
 }
 scr->getBackground(x / TROLL_SQUARE_X,
   (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y, sprite, shift);
 facing = 0;
 frame = 0;
}

