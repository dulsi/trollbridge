/*-------------------------------------------------------------------------*\
  <closedoor.C> -- Troll Bridge dynamically linked library - Closing Door

  Date      Programmer  Description
  12/28/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"
#include "dllconst.h"
#include "closedoor.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollClosingDoor::createNorthDoor - Creates a closing door to the North.

    Parameters:
      scr          (In)  Screen the door is on
      secrt        (In)  Secret number to set when dead

    Returns: A closing door
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollClosingDoor::createNorthDoor(TrollScreen *scr,
  IUShort secrt = 0)
{
 return new TrollClosingDoor(scr, secrt, 140, TROLL_CALCULATE_Y_POS(0),
   TROLL_DOWN);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollClosingDoor::createSouthDoor - Creates a closing door to the South.

    Parameters:
      scr          (In)  Screen the door is on
      secrt        (In)  Secret number to set when dead

    Returns: A closing door
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollClosingDoor::createSouthDoor(TrollScreen *scr,
  IUShort secrt = 0)
{
 return new TrollClosingDoor(scr, secrt, 140,
   TROLL_CALCULATE_Y_POS(TROLL_SCREEN_Y - 1), TROLL_UP);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollClosingDoor::createWestDoor - Creates a closing door to the West.

    Parameters:
      scr          (In)  Screen the door is on
      secrt        (In)  Secret number to set when dead

    Returns: A closing door
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollClosingDoor::createWestDoor(TrollScreen *scr,
  IUShort secrt = 0)
{
 return new TrollClosingDoor(scr, secrt, TROLL_CALCULATE_X_POS(0),
   TROLL_CALCULATE_Y_POS(5), TROLL_RIGHT);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollClosingDoor::createEastDoor - Creates a closing door to the East.

    Parameters:
      scr          (In)  Screen the door is on
      secrt        (In)  Secret number to set when dead

    Returns: A closing door
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollClosingDoor::createEastDoor(TrollScreen *scr,
  IUShort secrt = 0)
{
 return new TrollClosingDoor(scr, secrt,
   TROLL_CALCULATE_X_POS(TROLL_SCREEN_X - 1), TROLL_CALCULATE_Y_POS(5),
   TROLL_LEFT);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollClosingDoor::react - Have the door do something.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollClosingDoor::react()
{
 if (screen->getStandardMonsterNum() == 0)
 {
  dead = 1;
  screen->setPassability(x / TROLL_SQUARE_X,
    (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y, TROLL_OPEN);
 }
 else if (state == 0)
 {
  IUShort xCur, yCur;
  const TrollCharacter *troll = screen->getCharacter();

  xCur = x / TROLL_SQUARE_X;
  yCur = (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y;
  screen->setPassability(xCur, yCur, TROLL_BLOCKED);
  if ((troll) && (troll->getX() == x) && (troll->getY() == y))
  {
   (const_cast<TrollCharacter *>(troll))->push(direction, 4);
   state = 6;
  }
  else
  {
   frame = 1;
   state = 1;
  }
 }
 else if (state > 1)
 {
  state--;
  if (state == 1)
  {
   frame = 1;
  }
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollClosingDoor::TrollClosingDoor - Constructor for a closing door.

    Parameters:
      scr          (In)  Screen the door is on
      secrt        (In)  Secret number to set when dead
      xStart       (In)  Starting x location
      yStart       (In)  Starting y location
      dir          (In)  Direction to push player
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollClosingDoor::TrollClosingDoor(TrollScreen *scr, IUShort secrt,
  IUShort xStart, IUShort yStart, IUShort dir)
 : TrollMonster(scr, secrt)
{
 scr->getBackground(xStart / TROLL_SQUARE_X,
   (yStart - TROLL_BUFFER_Y) / TROLL_SQUARE_Y, sprite, shift);
 sprite = TROLL_SPRITE_CLOSEDOOR;
 facing = 0;
 frame = 0;
 x = xStart;
 y = yStart;
 state = 0;
 direction = dir;
}

