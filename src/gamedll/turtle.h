#ifndef __TURTLE_H
#define __TURTLE_H
/*-------------------------------------------------------------------------*\
  <turtle.h> -- Troll Bridge dynamically linked library - Turtle

  Date      Programmer  Description
  12/22/00  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Classes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollTurtleTail;

class TrollTurtle
{
 public:
  static TrollThing *createTurtle(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt = 0);
};

class TrollTurtleUnbreakable : public TrollMonster
{
 public:
  TrollTurtleUnbreakable(TrollScreen *scr, TrollTurtleTail *tl, IUShort sprt,
    IUShort shft);

  IShort getDamage();
  void react();

 protected:
  TrollTurtleTail *tail;
};

class TrollTurtleTail : public TrollStandardMonster
{
 public:
  TrollTurtleTail(TrollScreen *scr, IShort xStart, IShort yStart,
    IUShort secrt, IUShort shft);

  void react();
  void takeHit(TrollThing *hitBy);

 protected:
  IUShort time;
  IUShort state;
  TrollTurtleUnbreakable *head;
  TrollTurtleUnbreakable *shell;
  TrollTurtleUnbreakable *legs[2];
};

#endif

