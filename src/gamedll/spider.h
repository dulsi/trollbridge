#ifndef __SPIDER_H
#define __SPIDER_H
/*-------------------------------------------------------------------------*\
  <spider.h> -- Troll Bridge dynamically linked library - Spiders

  Date      Programmer  Description
  05/26/15  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Classes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollSpider : public TrollStandardMonster
{
 public:
  static TrollThing *createSpider(TrollScreen *scr, IShort sx,
    IShort sy, IUShort secrt);

  void die();
  void react();

 protected:
  TrollSpider(TrollScreen *scr, IShort sx, IShort sy, IUShort secrt);

  IUShort speed;
  IUShort time;
  IUShort state;
};

#endif

