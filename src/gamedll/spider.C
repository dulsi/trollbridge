/*-------------------------------------------------------------------------*\
  <spider.C> -- Troll Bridge dynamically linked library - Spiders

  Date      Programmer  Description
  05/26/15  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"
#include "dllconst.h"
#include "spider.h"
#include "explosion.h"
#include <igrgeometry.h>

#define TROLL_SPIDER_MOVE_DOWN 1
#define TROLL_SPIDER_MOVE_UP   7
#define TROLL_SPIDER_WAIT1     2
#define TROLL_SPIDER_WAIT2     3
#define TROLL_SPIDER_WAIT3     4
#define TROLL_SPIDER_OFFSCREEN 6

TrollThing *TrollSpider::createSpider(TrollScreen *scr,
  IShort sx, IShort sy, IUShort secrt)
{
 return new TrollSpider(scr, sx, sy, secrt);
}

void TrollSpider::die()
{
 TrollStandardMonster::die();
 screen->addMonster(new TrollExplosion(screen, x, y, 39 + shift));
}

void TrollSpider::draw(IScreen drawscreen)
{
 if (state !=TROLL_SPIDER_OFFSCREEN)
 {
  ILineDraw(drawscreen, x + 10, TROLL_BUFFER_Y, x + 10, y + 8, 255);
 }
 TrollStandardMonster::draw(drawscreen);
}

void TrollSpider::react()
{
 IUShort yCur;

 if (invincible)
 {
  invincible--;
 }
 time++;
 if ((time % speed) == 0)
 {
  frame++;
  frame %= 4;
 }
 if (state == TROLL_SPIDER_OFFSCREEN)
 {
  if ((time >= 30) && (IRandom(20) == 0))
  {
   time = 0;
   state = TROLL_SPIDER_WAIT1;

   x = IRandom(TROLL_SCREEN_X - 1) + 1;
   x *= TROLL_SQUARE_X;
   y = TROLL_BUFFER_Y;
  }
 }
 else if (state == TROLL_SPIDER_WAIT1)
 {
  if (time == speed * 4)
  {
   time = 0;
   state = TROLL_SPIDER_MOVE_UP;
  }
 }
 else if (state == TROLL_SPIDER_WAIT2)
 {
  if (time == speed * 4)
  {
   time = 0;
   state = TROLL_SPIDER_MOVE_DOWN;
   direction = TROLL_UP;
   facing = TROLL_DOWN;
  }
 }
 else if (state == TROLL_SPIDER_WAIT3)
 {
  if (time == speed * 4)
  {
   time = 20;
   x = y = -100;
   state = TROLL_SPIDER_OFFSCREEN;
   direction = TROLL_DOWN;
   facing = TROLL_UP;
  }
 }
 else
 {
  if (time == speed)
  {
   if  ((state == TROLL_SPIDER_MOVE_UP) && ((y - TROLL_BUFFER_Y) % TROLL_SQUARE_Y == 0))
   {
    yCur = (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y;
    if ((yCur == TROLL_SCREEN_Y - 1) || (IRandom(5) == 0))
    {
     state = TROLL_SPIDER_WAIT2;
    }
   }
   if ((state == TROLL_SPIDER_MOVE_DOWN) && (((y - TROLL_BUFFER_Y) % TROLL_SQUARE_Y == 0) && (y == TROLL_BUFFER_Y)))
   {
     state = TROLL_SPIDER_WAIT3;
   }
   if ((state == TROLL_SPIDER_MOVE_DOWN) || (state == TROLL_SPIDER_MOVE_UP))
   {
    move();
   }
   time = 0;
  }
 }
}

TrollSpider::TrollSpider(TrollScreen *scr, IShort sx, IShort sy,
  IUShort secrt)
 : TrollStandardMonster(scr, secrt, 1)
{
 if ((sx == TROLL_XYRANDOM) && (sy == TROLL_XYRANDOM))
 {
  x = y = -100;
  state = TROLL_SPIDER_OFFSCREEN;
 }
 else
 {
  x = sx;
  y = sy;
  state = TROLL_SPIDER_WAIT1;
 }
 sprite = TROLL_SPRITE_SPIDER;
 shift = 0;
 direction = TROLL_DOWN;
 facing = TROLL_UP;
 frame = 0;
 hp = 1;
 speed = 2;
 time = 0;
}

