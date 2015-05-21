/*-------------------------------------------------------------------------*\
  <jumper.C> -- Troll Bridge dynamically linked library - Jumpers

  Date      Programmer  Description
  03/14/99  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"
#include "dllconst.h"
#include "jumper.h"
#include "explosion.h"

#define TROLL_JUMPER_SPEED    2

#define TROLL_JUMPER_ONELEAP  8
#define TROLL_JUMPER_WAIT     12
#define TROLL_JUMPER_SICKNESS 13

TrollThing *TrollJumper::createGreenJumper(TrollScreen *scr, IShort sx,
  IShort sy, IUShort secrt)
{
 return new TrollJumper(scr, sx, sy, secrt, 5);
}

TrollThing *TrollJumper::createRedJumper(TrollScreen *scr, IShort sx,
  IShort sy, IUShort secrt)
{
 return new TrollJumper(scr, sx, sy, secrt, 10);
}

void TrollJumper::die()
{
 TrollStandardMonster::die();
 screen->addMonster(new TrollExplosion(screen, x, y, shift));
}

void TrollJumper::react()
{
 int i;
 bool pass = true;
 IUShort xCur, yCur;

 if (invincible)
 {
  invincible--;
 }
 time++;
 if (state == TROLL_JUMPER_SICKNESS)
 {
  if (time == 32)
  {
   time = 0;
   state = 0;
//   state = (IRandom(5) % 3) * 4;
  }
  if (time % TROLL_JUMPER_SPEED == 0)
  {
   frame++;
   frame %= 2;
  }
 }
 else if (state == TROLL_JUMPER_WAIT)
 {
  if (facing > 1) facing %= 2;
  if (time == TROLL_JUMPER_SPEED * 4)
  {
   time = 0;
   state = 0;
//   state = (IRandom(5) % 3) * 4;
  }
  if (time % TROLL_JUMPER_SPEED == 0)
  {
   frame++;
   frame %= 2;
  }
 }
 else if (flyBack)
 {
  pass = true;
  if ((x % TROLL_SQUARE_X == 0) &&
    ((y - TROLL_BUFFER_Y) % TROLL_SQUARE_Y == 0))
  {
   xCur = x / TROLL_SQUARE_X;
   yCur = (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y;
   pass = checkPassabilityFrom(xCur, yCur);
  }
  if (pass)
  {
   move();
   frame++;
   frame %= 2;
  }
  flyBack--;
  if (flyBack == 0)
  {
   state = TROLL_JUMPER_ONELEAP;
  }
  time = 0;
 }
 else if (time == TROLL_JUMPER_SPEED)
 {
  const Sprite *pic = TrollSpriteHandler.getSprite(sprite);

  switch (state % 4)
  {
   case 0:
    if (state == 0) direction = IRandom(4);
    switch (direction)
    {
     case TROLL_UP: // up and right
      x += TROLL_SQUARE_X / 4;
      y -= TROLL_SQUARE_Y / 4;
      facing = 2;
      break;
     case TROLL_DOWN: // up and left
      x -= TROLL_SQUARE_X / 4;
      y -= TROLL_SQUARE_Y / 4;
      facing = 3;
      break;
     case TROLL_RIGHT: // up and right
      x += TROLL_SQUARE_X / 4;
      y -= TROLL_SQUARE_Y / 4;
      facing = 2;
      break;
     case TROLL_LEFT: // up and left
      x -= TROLL_SQUARE_X / 4;
      y -= TROLL_SQUARE_Y / 4;
      facing = 3;
      break;
    }
    break;
   case 1:
   case 2:
    switch (direction)
    {
     case TROLL_UP: // up and right
      x += TROLL_SQUARE_X / 4;
      y -= TROLL_SQUARE_Y / 4;
      break;
     case TROLL_DOWN: // down and left
      x -= TROLL_SQUARE_X / 4;
      y += TROLL_SQUARE_Y / 4;
      break;
     case TROLL_RIGHT: // down and right
      x += TROLL_SQUARE_X / 4;
      y += TROLL_SQUARE_Y / 4;
      break;
     case TROLL_LEFT: // up and left
      x -= TROLL_SQUARE_X / 4;
      y -= TROLL_SQUARE_Y / 4;
      break;
    }
    break;
   case 3:
    switch (direction)
    {
     case TROLL_UP: // down and right
      x += TROLL_SQUARE_X / 4;
      y += TROLL_SQUARE_Y / 4;
      break;
     case TROLL_DOWN: // down and left
      x -= TROLL_SQUARE_X / 4;
      y += TROLL_SQUARE_Y / 4;
      break;
     case TROLL_RIGHT: // down and right
      x += TROLL_SQUARE_X / 4;
      y += TROLL_SQUARE_Y / 4;
      break;
     case TROLL_LEFT: // down and left
      x -= TROLL_SQUARE_X / 4;
      y += TROLL_SQUARE_Y / 4;
      break;
    }
    break;
  }
  if (x < 0)
  {
   x = 0;
  }
  else if (x + pic->getXSize() > 320)
  {
   x = 320 - pic->getXSize();
  }
  if (y < TROLL_BUFFER_Y)
  {
   y = TROLL_BUFFER_Y;
  }
  else if (y + pic->getYSize() > 200)
  {
   y = 200 - pic->getYSize();
  }
  state++;
  time = 0;
  frame++;
  frame %= 2;
 }
}

TrollJumper::TrollJumper(TrollScreen *scr, IShort sx, IShort sy,
  IUShort secrt, IUShort shft)
 : TrollStandardMonster(scr, secrt, 1)
{
 if ((sx == TROLL_XYRANDOM) && (sy == TROLL_XYRANDOM))
 {
  findOpen();
 }
 else
 {
  x = sx;
  y = sy;
 }
 sprite = TROLL_SPRITE_TOAD;
 shift = shft;
 facing = 0;
 direction = IRandom(4);
 frame = 0;
 hp = 1;
 time = IRandom(8) * 2;
 state = TROLL_JUMPER_SICKNESS;
}

