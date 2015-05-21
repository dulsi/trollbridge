/*-------------------------------------------------------------------------*\
  <trolldll.C> -- Sample Troll Bridge dynamically linked library

  Date      Programmer  Description
  04/17/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Classes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollFlyer:public TrollMonster
{
 public:
  static TrollThing *createTrollFlyerSlow(TrollScreen *scr, IUShort secrt = 0);
  static TrollThing *createTrollFlyerFast(TrollScreen *scr, IUShort secrt = 0);

  ~TrollFlyer();
  void react();

 protected:
  TrollFlyer(TrollScreen *scr, IUShort secrt = 0, IUShort spd = 1);

  IUShort speed;
  IUShort time1, time2;
};

class TrollActivatibleItem: public TrollItem
{
 public:
  static TrollThing *createTrollSwordItem(TrollScreen *scr, IUShort x,
    IUShort y, IUShort secrt = 0);

  void pickUp(TrollCharacter *troll);

 protected:
  TrollActivatibleItem(TrollScreen *scr, IUShort xStart, IUShort yStart,
    IUShort secrt, IUShort slt, TrollCarriedItem *actItem);

  TrollCarriedItem *activateItem;
  IUShort slot;
};

class TrollSwordItem: public TrollCarriedItem
{
 public:
  TrollSwordItem(IUShort str);
  void activate(TrollCharacter *troll, TrollScreen *screen);

 protected:
};

class TrollSwordProjectile: public TrollProjectile
{
 public:
  TrollSwordProjectile(TrollScreen *screen, IUShort xStart, IUShort yStart,
    IUShort dir,IUShort dmg);
  void react();
  void takeHit(TrollThing *hitBy);

 protected:
  IUByte state;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Global variables
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollSwordItem TrollNormalSword(1);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollFlyer::createTrollFlyerSlow - Creates a slow TrollFlyer.

    Parameters:
      scr          (In)  Screen the flyer is on
      secrt        (In)  Secret number to set when dead

    Returns: A slow TrollFlyer
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollFlyer::createTrollFlyerSlow(TrollScreen *scr,
  IUShort secrt = 0)
{
 return new TrollFlyer(scr, secrt, 2);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollFlyer::createTrollFlyerFast - Creates a fast TrollFlyer.

    Parameters:
      scr          (In)  Screen the flyer is on
      secrt        (In)  Secret number to set when dead

    Returns: A fast TrollFlyer
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollFlyer::createTrollFlyerFast(TrollScreen *scr,
  IUShort secrt = 0)
{
 return new TrollFlyer(scr, secrt, 1);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollFlyer::~TrollFlyer - Destructor of a flyer.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollFlyer::~TrollFlyer()
{
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollFlyer::react - Have the flyer do something.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollFlyer::react()
{
 if (invincible)
 {
  invincible--;
 }
 if ((flyBack) || (time1 == 0))
 {
  time1 = speed;
  if ((!flyBack) && (time2 == 0))
  {
   // Flyers move in a random direction
   direction = IRandom(4);
   time2 = IRandom(10) + 10;
  }
  switch (direction)
  {
   case TROLL_UP:
    if (y > TROLL_BUFFER_Y + TROLL_SQUARE_Y)
    {
     y -= 2;
    }
    break;
   case TROLL_DOWN:
    if (y < 199 - TROLL_SQUARE_Y * 2)
    {
     y += 2;
    }
    break;
   case TROLL_LEFT:
    if (x > TROLL_SQUARE_X)
    {
     x -= 2;
    }
    break;
   case TROLL_RIGHT:
    if (x < 319 - TROLL_SQUARE_X * 2)
    {
     x += 2;
    }
    break;
   default:
    // Should never get here
    break;
  }
  if (flyBack)
  {
   flyBack--;
  }
  else
  {
   time2--;
   frame++;
   if (frame == 3) frame = 0;
  }
 }
 time1--;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollFlyer::TrollFlyer - Constructor for a flyer.

    Parameters:
      scr          (In)  Screen the flyer is on
      secrt        (In)  Secret number to set when dead
      spd          (In)  Speed of the flyer (lower is faster)
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollFlyer::TrollFlyer(TrollScreen *scr, IUShort secrt = 0, IUShort spd = 1)
  :TrollMonster(scr, secrt)
{
 sprite = 13;
 shift = 0;
 facing = 0;
 frame = 0;
 x = IRandom(260) + 20;
 y = IRandom((152 - TROLL_BUFFER_Y)) + 16 + TROLL_BUFFER_Y;
 hp = 2;
 speed = spd;
 time1 = time2 = 0;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollActivatibleItem::createTrollSwordItem - Creates a sword.

    Parameters:
      scr          (In)  Screen the flyer is on
      x            (In)  X location
      y            (In)  Y location
      secrt        (In)  Secret number to set when dead

    Returns: A sword
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollActivatibleItem::createTrollSwordItem(TrollScreen *scr,
  IUShort x, IUShort y, IUShort secrt = 0)
{
 return new TrollActivatibleItem(scr, x, y, secrt, 0, &TrollNormalSword);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollActivatibleItem::pickUp - Gives the item to the character.

    Parameters:
      trll         (In)  Character to get the item
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollActivatibleItem::pickUp(TrollCharacter *troll)
{
 troll->addCarriedItem(slot, activateItem);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollActivatibleItem::TrollActivatibleItem - Constructor for an activatible
    item.

    Parameters:
      scr          (In)  Screen the flyer is on
      xStart       (In)  X location
      yStart       (In)  Y location
      secrt        (In)  Secret number to set when dead
      slt          (In)  Slot in character's carried items array for the item
      actItem      (In)  The activation item
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollActivatibleItem::TrollActivatibleItem(TrollScreen *scr, IUShort xStart,
  IUShort yStart, IUShort secrt, IUShort slt, TrollCarriedItem *actItem)
  :TrollItem(scr, xStart, yStart, secrt)
{
 slot = slt;
 activateItem = actItem;
 sprite = actItem->getSprite();
 shift = actItem->getShift();
 facing = 0;
 frame = 0;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSwordItem::TrollSwordItem - Constructor for the sword.

    Parameters:
      str          (In)  Strength of the sword
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollSwordItem::TrollSwordItem(IUShort str)
  :TrollCarriedItem(str)
{
 sprite = 9;
 shift = 0;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSwordItem::activate - Activates the sword.

    Parameters:
      troll        (In)  Character using the sword
      screen       (In)  Screen being affected
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollSwordItem::activate(TrollCharacter *troll, TrollScreen *screen)
{
 TrollSwordProjectile *sword;
 IUShort xPos, yPos;

 xPos = troll->getX();
 yPos = troll->getY();
 switch (troll->getDirection())
 {
  case TROLL_UP:
   if (yPos > TROLL_BUFFER_Y + TROLL_SQUARE_Y - 1)
   {
    sword = new TrollSwordProjectile(screen, xPos + 1, yPos - 14, TROLL_UP,
      strength);
    screen->addCharacterProjectile(sword);
    troll->setPause(5);
   }
   break;
  case TROLL_DOWN:
   if (yPos < 200 - TROLL_SQUARE_Y * 2)
   {
    sword = new TrollSwordProjectile(screen, xPos + 13, yPos + 14, TROLL_DOWN,
      strength);
    screen->addCharacterProjectile(sword);
    troll->setPause(5);
   }
   break;
  case TROLL_RIGHT:
   if (yPos < 320 - TROLL_SQUARE_X * 2)
   {
    sword = new TrollSwordProjectile(screen, xPos + 16, yPos + 8, TROLL_RIGHT,
      strength);
    screen->addCharacterProjectile(sword);
    troll->setPause(5);
   }
   break;
  case TROLL_LEFT:
   if (xPos > TROLL_SQUARE_X - 1)
   {
    sword = new TrollSwordProjectile(screen, xPos - 12, yPos + 8, TROLL_LEFT,
      strength);
    screen->addCharacterProjectile(sword);
    troll->setPause(5);
   }
   break;
  default:
   break;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSwordProjectile::TrollSwordProjectile - Constructor for the sword
    blade.

    Parameters:
      screen       (In)  Screen the flyer is on
      xStart       (In)  X location
      yStart       (In)  Y location
      dir          (In)  Direction of attack
      dmg          (In)  Damage of the attack
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollSwordProjectile::TrollSwordProjectile(TrollScreen *screen,
  IUShort xStart, IUShort yStart, IUShort dir, IUShort dmg)
  :TrollProjectile(screen, dmg)
{
 switch (dir)
 {
  case TROLL_UP:
   sprite = 11;
   facing = 0;
   direction = TROLL_UP;
   break;
  case TROLL_DOWN:
   sprite = 11;
   facing = 1;
   direction = TROLL_DOWN;
   break;
  case TROLL_RIGHT:
   sprite = 10;
   facing = 1;
   direction = TROLL_RIGHT;
   break;
  case TROLL_LEFT:
   sprite = 10;
   facing = 0;
   direction = TROLL_LEFT;
   break;
  default:
   // Should never get here
   break;
 }
 x = xStart;
 y = yStart;
 shift = 0;
 frame = 0;
 state = 0;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSwordProjectile::react - Performs animation sequence for sword blade.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollSwordProjectile::react()
{
 switch (state)
 {
  case 1:
   frame = 1;
  case 2:
   break;
  case 3:
   frame = 0;
   break;
  case 4:
   dead = 1;
   break;
  case 0:
  default:
   break;
 }
 state++;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSwordProjectile::takeHit - Called when the sword blade hits something.

    Parameters:
      hitBy        (In)  Thing it hit
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollSwordProjectile::takeHit(TrollThing *hitBy)
{
 // Sword projectile doesn't die from hitting things
}

// This function must be extern "C" so that C++ doesn't rename the function.
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollDllInit - Sends information about monsters and items to the main
    program.

    Parameters:
      game         (In)  Game being played

    Returns: 0 on success, any other value indicates failure
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
extern "C" int TrollDllInit(TrollGame *game)
{
 // add monster types
 game->addMonsterType(0, TrollFlyer::createTrollFlyerSlow);
 game->addMonsterType(1, TrollFlyer::createTrollFlyerFast);
 // add items
 game->addItemType(0, TrollActivatibleItem::createTrollSwordItem);
 // add carried items
 game->addCarriedItem(0, &TrollNormalSword);
 return 0;
}

