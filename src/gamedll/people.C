/*-------------------------------------------------------------------------*\
  <people.C> -- Troll Bridge dynamically linked library - People

  Date      Programmer  Description
  12/05/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"
#include "dllconst.h"
#include "people.h"
#include <igrtext.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Globals
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
char *TrollMerchant::saying[TROLL_MERCHANT_SAYINGS][2] =
{
 {"What do you want?", ""},
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollPerson::createGrayTroll - Creates a Gray Troll.

    Parameters:
      scr          (In)  Screen the Gray Troll is on
      xStart       (In)  Starting X position
      yStart       (In)  Starting Y position
      secrt        (In)  Secret number to set when dead

    Returns: A Gray Troll
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollPerson::createGrayTroll(TrollScreen *scr, IShort xStart,
  IShort yStart, IUShort secrt)
{
 return
   new TrollPerson(scr, xStart, yStart, secrt, TROLL_SPRITE_GRAYTROLL, 0);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollPerson::react - Have the people do something.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollPerson::react()
{
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollPerson::TrollPerson - Constructor for a person.

    Parameters:
      scr          (In)  Screen the person is on
      xStart       (In)  Starting X position
      yStart       (In)  Starting Y position
      secrt        (In)  Secret number to set when dead
      sprt         (In)  Sprite of the person
      shft         (In)  Color Shift of the person
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollPerson::TrollPerson(TrollScreen *scr, IShort xStart, IShort yStart,
  IUShort secrt, IUShort sprt, IUShort shft) : TrollMonster(scr, secrt)
{
 if ((xStart == TROLL_XYRANDOM) && (yStart == TROLL_XYRANDOM))
 {
  x = 140;
  y = 88;
 }
 else
 {
  x = xStart;
  y = yStart;
 }
 sprite = sprt;
 shift = shft;
 facing = 0;
 frame = 0;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollMerchant::createMerchant - Creates a Merchant.

    Parameters:
      scr          (In)  Screen the Merchant is on
      xStart       (In)  Starting X position
      yStart       (In)  Starting Y position
      secrt        (In)  Secret number to set when dead

    Returns: A Merchant
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollMerchant::createMerchant(TrollScreen *scr, IShort xStart,
  IShort yStart, IUShort secrt)
{
 return new TrollMerchant(scr, xStart, yStart, secrt);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollMerchant::~TrollMerchant - Desctructor for merchant.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollMerchant::~TrollMerchant()
{
 delete [] text[0];
 delete [] text[1];
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollMerchant::draw - Draws the merchant.

    Parameters:
      drawscreen   (In & Out) Screen to draw to
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollMerchant::draw(IScreen drawscreen)
{
 TrollPerson::draw(drawscreen);
 const Sprite *pic = TrollSpriteHandler.getSprite(TROLL_SPRITE_GOLD);
 pic->draw(drawscreen, 60, 134, 0, 0, 0);
 ITextDraw(drawscreen, 80, 138, 255, "X");
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollMerchant::react - Have the merchant do something.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollMerchant::react()
{
 time++;
 if (time > 1)
 {
  time = 0;
  if (textNum < 2)
  {
   frame++;
   frame %= 2;
   if ((text[textNum][textLen] = saying[say][textNum][textLen]))
   {
    text[textNum][++textLen] = 0;
    screen->setText(text[0], text[1]);
   }
   else
   {
    textNum++;
    textLen = 0;
    frame = 0;
   }
  }
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollMerchant::TrollMerchant - Constructor for a merchant.

    Parameters:
      scr          (In)  Screen the merchant is on
      xStart       (In)  Starting X position
      yStart       (In)  Starting Y position
      secrt        (In)  Secret number to set when dead
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollMerchant::TrollMerchant(TrollScreen *scr, IShort xStart,
  IShort yStart, IUShort secrt)
 : TrollPerson(scr, xStart, yStart, secrt, TROLL_SPRITE_MERCHANT, 0)
{
 say = IRandom(TROLL_MERCHANT_SAYINGS);
 text[0] = new char[strlen(saying[say][0]) + 1];
 text[1] = new char[strlen(saying[say][1]) + 1];
 text[0][0] = text[1][0] = 0;
 textNum = 0;
 textLen = 0;
 time = 0;
}

