#ifndef __PEOPLE_H
#define __PEOPLE_H
/*-------------------------------------------------------------------------*\
  <people.h> -- Troll Bridge dynamically linked library - People

  Date      Programmer  Description
  12/05/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"

#define TROLL_MERCHANT_SAYINGS 1

class TrollPerson: public TrollMonster
{
 public:
  static TrollThing *createGrayTroll(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt);

  void react();

 protected:
  TrollPerson(TrollScreen *scr, IShort xStart, IShort yStart,
    IUShort secrt, IUShort sprt, IUShort shft);
};

class TrollMerchant: public TrollPerson
{
 public:
  static TrollThing *createMerchant(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt);

  ~TrollMerchant();
  void draw(IScreen drawscreen);
  void react();

 protected:
  TrollMerchant(TrollScreen *scr, IShort xStart, IShort yStart,
    IUShort secrt);

  IUShort time;
  IUShort textNum, textLen;
  IUShort say;
  char *text[2];

  static char *saying[TROLL_MERCHANT_SAYINGS][2];
};

#endif

