/*-------------------------------------------------------------------------*\
  <fireelemental.C> -- Troll Bridge dynamically linked library - Fire Elementals

  Date      Programmer  Description
  07/07/19  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"
#include "dllconst.h"
#include "items.h"
#include "fireelemental.h"
#include "blast.h"

#define TROLL_FIRE_STATE_QUIET 0
#define TROLL_FIRE_STATE_PREFIRE  84
#define TROLL_FIRE_STATE_ATTACK 90
#define TROLL_FIRE_STATE_ATTACK2 150
#define TROLL_FIRE_STATE_REPEAT   220

TrollThing *TrollFireElemental::createFireElemental(TrollScreen *scr, IShort xStart,
  IShort yStart, IUShort secrt)
{
 return new TrollFireElemental(scr, xStart, yStart, secrt, 10);
}

void TrollFireElemental::die()
{
 IUShort num;
 TrollThing *item;
 TrollCharacter *troll = const_cast<TrollCharacter *>(screen->getCharacter());

 TrollStandardMonster::die();
 troll->addTotalHp(2);
 if (IRandom(3) == 0)
 {
  num = IRandom(10);
  if (num > 8)
  {
   item = TrollGold::create3GoldCoins(screen, x, y, 0);
  }
  else if (num > 3)
  {
   item = TrollGold::createGoldCoin(screen, x, y, 0);
  }
  else
  {
   item = TrollHitPoint::createHitPoint(screen, x, y, 0);
  }
  screen->addItem(item);
 }
}

void TrollFireElemental::react()
{
 int i;
 bool pass = true;
 IUShort xCur, yCur;

 if (invincible)
 {
  invincible--;
 }
 if (state > TROLL_FIRE_STATE_PREFIRE)
 {
  frame++;
  frame %= 2;
 }
 state++;
 if ((state == TROLL_FIRE_STATE_ATTACK) ||
   (state == TROLL_FIRE_STATE_ATTACK2))
 {
  const TrollCharacter *trll = screen->getCharacter();
  IShort trollXPos, trollYPos, trollX, trollY;
  trll->getLocation(trollXPos, trollYPos);
  trollX = trollXPos / TROLL_SQUARE_X;
  trollY = (trollYPos - TROLL_BUFFER_Y) / TROLL_SQUARE_Y;
  int offset = 3 - (trollY * TROLL_SCREEN_X + trollX) % 3;
  for (int squareY = 0; squareY < TROLL_SCREEN_Y; squareY++)
  {
   for (int squareX = 0; squareX < TROLL_SCREEN_X; squareX++)
   {
    if ((((squareY * TROLL_SCREEN_X + squareX + offset) % 3) == 0) &&
     (screen->getPassability(squareX, squareY) <= TROLL_NOMONSTER))
    {
     screen->addMonster(TrollLavaSquare::createLavaSquare(screen, squareX * TROLL_SQUARE_X, squareY * TROLL_SQUARE_Y + TROLL_BUFFER_Y, 0));
    }
   }
  }
 }
 if (state == TROLL_FIRE_STATE_REPEAT)
 {
  state = TROLL_FIRE_STATE_QUIET;
 }
}

void TrollFireElemental::takeHit(TrollThing *hitBy)
{
 TrollStandardMonster::takeHit(hitBy);
 if (invincible == TROLL_INVINCIBLE_TIME)
 {
  if (state < TROLL_FIRE_STATE_PREFIRE)
   state = TROLL_FIRE_STATE_PREFIRE;
 }
}

TrollFireElemental::TrollFireElemental(TrollScreen *scr, IShort xStart, IShort yStart,
  IUShort secrt, IUShort shft)
 : TrollStandardMonster(scr, secrt, 2)
{
 if ((xStart == TROLL_XYRANDOM) && (yStart == TROLL_XYRANDOM))
 {
  const Sprite *pic = TrollSpriteHandler.getSprite(TROLL_SPRITE_FIREELEMENTAL);
  x = TROLL_CALCULATE_X_POS(7) - (pic->getXSize() / 2);
  y = TROLL_CALCULATE_Y_POS(1);
 }
 else
 {
  x = xStart;
  y = yStart;
 }
 sprite = TROLL_SPRITE_FIREELEMENTAL;
 shift = shft;
 direction = TROLL_DOWN;
 facing = 0;
 frame = 0;
 hp = 4;
 time = 0;
 state = TROLL_FIRE_STATE_QUIET;
}

TrollThing *TrollLavaSquare::createLavaSquare(TrollScreen *scr, IShort xStart,
  IShort yStart, IUShort secrt)
{
 return new TrollLavaSquare(scr, xStart, yStart, secrt, 10);
}

#define TROLL_LAVASQUARE_LAVA   40
#define TROLL_LAVASQUARE_RECEED 80
#define TROLL_LAVASQUARE_COMPLETE 120

IShort TrollLavaSquare::getDamage()
{
 const TrollCharacter *trll = screen->getCharacter();
 IShort trollY = trll->getY();
 if (sprite == TROLL_SPRITE_LAVA)
  return 1;
 else
  return 0;
}

void TrollLavaSquare::getSize(IShort &xSize, IShort &ySize) const
{
 TrollThing::getSize(xSize, ySize);
 ySize = ySize / 2;
}

void TrollLavaSquare::react()
{
 state++;
 if (state == TROLL_LAVASQUARE_LAVA)
 {
  sprite = TROLL_SPRITE_LAVA;
 }
 else if (state == TROLL_LAVASQUARE_RECEED)
 {
  sprite = TROLL_SPRITE_LAVASQUARE;
 }
 else if (state == TROLL_LAVASQUARE_COMPLETE)
 {
  dead = 1;
 }
 if (state % 2)
 {
  frame = (frame + 1) % 2;
 }
}

TrollLavaSquare::TrollLavaSquare(TrollScreen *scr, IShort xStart, IShort yStart,
  IUShort secrt, IUShort shft)
 : TrollMonster(scr, secrt)
{
 if ((xStart == TROLL_XYRANDOM) && (yStart == TROLL_XYRANDOM))
 {
  findOpen();
 }
 else
 {
  x = xStart;
  y = yStart;
 }
 sprite = TROLL_SPRITE_LAVASQUARE;
 shift = shft;
 direction = TROLL_DOWN;
 facing = 0;
 frame = 0;
 state = 0;
}
