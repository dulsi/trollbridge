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
      secrt        (In)  Secret number to set when dead

    Returns: A Healer
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollHealer::createHealer(TrollScreen *scr, IUShort secrt = 0)
{
 return new TrollHealer(scr, secrt);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollHealer::draw - Draw the healer.

    Parameters:
      drawscreen   (In & Out) Screen to draw to
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollHealer::draw(IScreen drawscreen)
{
 IUShort tx = screen->getCharacter()->getX();
 IUShort ty = screen->getCharacter()->getY();
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
 IUShort tx = troll->getX();
 IUShort ty = troll->getY();
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
      secrt        (In)  Secret number to set when dead
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollHealer::TrollHealer(TrollScreen *scr, IUShort secrt)
  : TrollMonster(scr, secrt)
{
 sprite = 0;
 shift = 0;
 facing = 0;
 frame = 0;
 x = 140;
 y = 88;
 state = 0;
}

