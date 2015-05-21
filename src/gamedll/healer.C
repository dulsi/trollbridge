/*-------------------------------------------------------------------------*\
  <healer.C> -- Troll Bridge dynamically linked library - Healer

  Date      Programmer  Description
  02/28/99  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"
#include "dllconst.h"
#include "healer.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollHealer::createHealer - Creates a Healer.

    Parameters:
      scr          (In)  Screen the Healer is on
      sx           (In)  Starting X position
      sy           (In)  Starting Y position
      secrt        (In)  Secret number to set when dead

    Returns: A Healer
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollHealer::createHealer(TrollScreen *scr, IShort sx,
  IShort sy, IUShort secrt = 0)
{
 return new TrollHealer(scr, sx, sy, secrt);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollHealer::draw - Draw the healer.

    Parameters:
      drawscreen   (In & Out) Screen to draw to
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollHealer::draw(IScreen drawscreen)
{
 IShort tx;
 IShort ty;

 screen->getCharacter()->getLocation(tx, ty);
 if ((tx >= 115) && (tx <= 165) && (ty >= 125) && (ty <= 136))
 {
  IUShort fr;
  const Sprite *pic = TrollSpriteHandler.getSprite(TROLL_SPRITE_EXPLOSION);
  fr = (state / 4) % 2;
  switch (state / 8)
  {
   case 0:
    pic->draw(drawscreen, 130, 120, fr, 0, 4);
    break;
   case 1:
    pic->draw(drawscreen, 140, 112, fr, 0, 4);
    break;
   case 2:
    pic->draw(drawscreen, 150, 120, fr, 0, 4);
    break;
   default:
    break;
  }
  state++;
  state %= 3 * 8;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollHealer::react - Have the healer do something.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollHealer::react()
{
 TrollCharacter *troll = const_cast<TrollCharacter *>(screen->getCharacter());
 IShort tx;
 IShort ty;

 troll->getLocation(tx, ty);
 if ((tx >= 115) && (tx <= 165) && (ty >= 125) && (ty <= 136) &&
   (state % 8 == 0))
 {
  troll->addHp(1);
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollHealer::TrollHealer - Constructor for a healer.

    Parameters:
      scr          (In)  Screen the healer is on
      sx           (In)  Starting X position
      sy           (In)  Starting Y position
      secrt        (In)  Secret number to set when dead
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollHealer::TrollHealer(TrollScreen *scr, IShort sx, IShort sy,
  IUShort secrt)
  : TrollMonster(scr, secrt)
{
 if ((sx == TROLL_XYRANDOM) && (sy == TROLL_XYRANDOM))
 {
  x = 140;
  y = 88;
 }
 else
 {
  x = sx;
  y = sy;
 }
 sprite = 0;
 shift = 0;
 facing = 0;
 frame = 0;
 state = 0;
}

