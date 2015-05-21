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
  TrollDoor::createNorthDoor - Creates a door to the North.

    Parameters:
      scr          (In)  Screen the door is on
      secrt        (In)  Secret number to set when dead

    Returns: A door
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollDoor::createNorthDoor(TrollScreen *scr, IUShort secrt = 0)
{
 return
   new TrollDoor(scr, secrt, 140, TROLL_CALCULATE_Y_POS(0));
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollDoor::createSouthDoor - Creates a door to the South.

    Parameters:
      scr          (In)  Screen the door is on
      secrt        (In)  Secret number to set when dead

    Returns: A door
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollDoor::createSouthDoor(TrollScreen *scr, IUShort secrt = 0)
{
 return
   new TrollDoor(scr, secrt, 140, TROLL_CALCULATE_Y_POS(TROLL_SCREEN_Y - 1));
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollDoor::createWestDoor - Creates a door to the West.

    Parameters:
      scr          (In)  Screen the door is on
      secrt        (In)  Secret number to set when dead

    Returns: A door
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollDoor::createWestDoor(TrollScreen *scr, IUShort secrt = 0)
{
 return new TrollDoor(scr, secrt, TROLL_CALCULATE_X_POS(0),
   TROLL_CALCULATE_Y_POS(5));
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollDoor::createEastDoor - Creates a door to the East.

    Parameters:
      scr          (In)  Screen the door is on
      secrt        (In)  Secret number to set when dead

    Returns: A door
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollDoor::createEastDoor(TrollScreen *scr, IUShort secrt = 0)
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
TrollDoor::TrollDoor(TrollScreen *scr, IUShort secrt, IUShort xStart,
  IUShort yStart) : TrollMonster(scr, secrt)
{
 scr->getBackground(xStart / TROLL_SQUARE_X,
   (yStart - TROLL_BUFFER_Y) / TROLL_SQUARE_Y, sprite, shift);
 facing = 0;
 frame = 0;
 x = xStart;
 y = yStart;
}

