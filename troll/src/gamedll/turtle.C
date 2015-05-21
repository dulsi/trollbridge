/*-------------------------------------------------------------------------*\
  <turtle.C> -- Troll Bridge dynamically linked library - Turtle

  Date      Programmer  Description
  12/22/00  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"
#include "dllconst.h"
#include "turtle.h"

#define TROLL_TURTLE_SHELLED 1
#define TROLL_TURTLE_ACTIVE  2

TrollThing *TrollTurtle::createTurtle(TrollScreen *scr, IShort xStart,
  IShort yStart, IUShort secrt = 0)
{
 return new TrollTurtleTail(scr, xStart, yStart, secrt, 0);
}

TrollTurtleUnbreakable::TrollTurtleUnbreakable(TrollScreen *scr,
  TrollTurtleTail *tl, IUShort sprt, IUShort shft)
 : TrollMonster(scr, 0)
{
 sprite = sprt;
 shift = shft;
 tail = tl;
 facing = 0;
 frame = 0;
 x = y = TROLL_XYOFFSCREEN;
 scr->addMonster(this);
}

IShort TrollTurtleUnbreakable::getDamage()
{
 tail->getDamage();
}

void TrollTurtleUnbreakable::react()
{
}

TrollTurtleTail::TrollTurtleTail(TrollScreen *scr, IShort xStart, IShort yStart,
  IUShort secrt, IUShort shft)
 : TrollStandardMonster(scr, secrt, 1)
{
 sprite = TROLL_SPRITE_TURTLETAIL;
 shift = shft;
 direction = TROLL_LEFT;
 facing = 0;
 frame = 0;
 hp = 1;
 if ((xStart == TROLL_XYRANDOM) && (yStart == TROLL_XYRANDOM))
 {
  findOpen();
 }
 else
 {
  x = xStart;
  y = yStart;
 }
 head = new TrollTurtleUnbreakable(scr, this, TROLL_SPRITE_TURTLEHEAD, shift);
 shell = new TrollTurtleUnbreakable(scr, this, TROLL_SPRITE_TURTLESHELL, shift);
 legs[0] = new TrollTurtleUnbreakable(scr, this, TROLL_SPRITE_TURTLEHLEGS, shift);
 legs[1] = new TrollTurtleUnbreakable(scr, this, TROLL_SPRITE_TURTLEHLEGS, shift);
 shell->setLocation(x, y);
 x = y = TROLL_XYOFFSCREEN;
 state = TROLL_TURTLE_SHELLED;
 time = 0;
}

void TrollTurtleTail::react()
{
 IShort shellX, shellY;

 time++;
 switch (state)
 {
  case TROLL_TURTLE_SHELLED:
   if (20 == time)
   {
    time = 0;
    state = TROLL_TURTLE_ACTIVE;
    direction = IRandom(4);
    facing = direction;
    head->setFacing(direction);
    head->setDirection(direction);
    shell->getLocation(shellX, shellY);
    shell->setDirection(direction);
    legs[0]->setDirection(direction);
    legs[1]->setDirection(direction);
    switch (direction)
    {
     case TROLL_DOWN:
      x = shellX + 14;
      y = shellY - 4;
      head->setLocation(shellX + 11, shellY + 18);
      legs[0]->setSprite(TROLL_SPRITE_TURTLEVLEGS, 0);
      legs[0]->setLocation(shellX - 4, shellY + 1);
      legs[1]->setSprite(TROLL_SPRITE_TURTLEVLEGS, 1);
      legs[1]->setLocation(shellX + 28, shellY + 1);
      break;
     case TROLL_UP:
      x = shellX + 14;
      y = shellY + 23;
      head->setLocation(shellX + 11, shellY - 4);
      legs[0]->setSprite(TROLL_SPRITE_TURTLEVLEGS, 2);
      legs[0]->setLocation(shellX - 4, shellY + 3);
      legs[1]->setSprite(TROLL_SPRITE_TURTLEVLEGS, 3);
      legs[1]->setLocation(shellX + 28, shellY + 3);
      break;
     case TROLL_LEFT:
      x = shellX + 32;
      y = shellY + 10;
      head->setLocation(shellX - 4, shellY + 7);
      legs[0]->setSprite(TROLL_SPRITE_TURTLEHLEGS, 0);
      legs[0]->setLocation(shellX + 6, shellY + 22);
      legs[1]->setSprite(TROLL_SPRITE_TURTLEHLEGS, 1);
      legs[1]->setLocation(shellX + 6, shellY - 4);
      break;
     case TROLL_RIGHT:
      x = shellX - 4;
      y = shellY + 10;
      head->setLocation(shellX + 27, shellY + 7);
      legs[0]->setSprite(TROLL_SPRITE_TURTLEHLEGS, 2);
      legs[0]->setLocation(shellX + 4, shellY + 22);
      legs[1]->setSprite(TROLL_SPRITE_TURTLEHLEGS, 3);
      legs[1]->setLocation(shellX + 4, shellY - 4);
      break;
     default:
      break;
    }
   }
   break;
  case TROLL_TURTLE_ACTIVE:
   if (time & 1)
   {
    frame = (frame + 1) % 2;
    if ((time & 6) == 0)
    {
     legs[0]->setFrame(0);
     legs[1]->setFrame(0);
    }
    else if ((time & 6) == 2)
    {
     legs[0]->setFrame(1);
     legs[1]->setFrame(1);
    }
    if (16 <= time)
    {
     state = TROLL_TURTLE_SHELLED;
     x = y = TROLL_XYOFFSCREEN;
     head->setLocation(x, y);
     legs[0]->setLocation(x, y);
     legs[1]->setLocation(x, y);
     time = 0;
    }
    else
    {
     switch(direction)
     {
      case TROLL_DOWN:
       if (y < 200 - (TROLL_SQUARE_Y * 3))
       {
        y += TROLL_SQUARE_Y / 4;
        head->push(0, TROLL_SQUARE_Y / 4);
        shell->push(0, TROLL_SQUARE_Y / 4);
        legs[0]->push(0, TROLL_SQUARE_Y / 4);
        legs[1]->push(0, TROLL_SQUARE_Y / 4);
       }
       break;
      case TROLL_UP:
       if (y > TROLL_BUFFER_Y + (TROLL_SQUARE_Y * 3))
       {
        y -= TROLL_SQUARE_Y / 4;
        head->push(0, -TROLL_SQUARE_Y / 4);
        shell->push(0, -TROLL_SQUARE_Y / 4);
        legs[0]->push(0, -TROLL_SQUARE_Y / 4);
        legs[1]->push(0, -TROLL_SQUARE_Y / 4);
       }
       break;
      case TROLL_LEFT:
       if (x > (TROLL_SQUARE_X * 3))
       {
        x -= TROLL_SQUARE_X / 4;
        head->push(-TROLL_SQUARE_X / 4, 0);
        shell->push(-TROLL_SQUARE_X / 4, 0);
        legs[0]->push(-TROLL_SQUARE_X / 4, 0);
        legs[1]->push(-TROLL_SQUARE_X / 4, 0);
       }
       break;
      case TROLL_RIGHT:
       if (x < 320 - (TROLL_SQUARE_X * 3))
       {
        x += TROLL_SQUARE_X / 4;
        head->push(TROLL_SQUARE_X / 4, 0);
        shell->push(TROLL_SQUARE_X / 4, 0);
        legs[0]->push(TROLL_SQUARE_X / 4, 0);
        legs[1]->push(TROLL_SQUARE_X / 4, 0);
       }
       break;
      default:
       break;
     }
    }
   }
   break;
  default:
   break;
 }
}

void TrollTurtleTail::takeHit(TrollThing *hitBy)
{
 TrollStandardMonster::takeHit(hitBy);
 if (isDead())
 {
  head->setDead(1);
  shell->setDead(1);
  legs[0]->setDead(1);
  legs[1]->setDead(1);
 }
}

