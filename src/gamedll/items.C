/*-------------------------------------------------------------------------*\
  <items.C> -- Troll Bridge dynamically linked library - Items

  Date      Programmer  Description
  04/17/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "troll.h"
#include "dllconst.h"
#include "explosion.h"
#include "items.h"
#include <igrtext.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Defines
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define TROLL_ITEM_LIFESPAN 100

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Global variables
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollSwordItem TrollNormalSword(0, 1, 1);
TrollEnchantedSwordItem TrollEnchantedSword(2, 2, 1);
TrollKeyItem TrollKey;
TrollAnhkItem TrollAnhk(3, 1);
TrollShieldItem TrollShield(4, 1);
TrollEggItem TrollEgg(5);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollActivatibleItem::createAnhk - Creates an anhk.

    Parameters:
      scr          (In)  Screen the anhk is on
      x            (In)  X location
      y            (In)  Y location
      secrt        (In)  Secret number to set when dead

    Returns: An anhk
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollActivatibleItem::createAnhk(TrollScreen *scr, IShort x,
  IShort y, IUShort secrt)
{
 return new TrollActivatibleItem(scr, x, y, secrt, 0, &TrollAnhk);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollActivatibleItem::createEnchantedSword - Creates an enchanted sword.

    Parameters:
      scr          (In)  Screen the sword is on
      x            (In)  X location
      y            (In)  Y location
      secrt        (In)  Secret number to set when dead

    Returns: A sword
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollActivatibleItem::createEnchantedSword(TrollScreen *scr,
  IShort x, IShort y, IUShort secrt)
{
 return new TrollActivatibleItem(scr, x, y, secrt, 0, &TrollEnchantedSword);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollActivatibleItem::createEgg - Creates an egg.

    Parameters:
      scr          (In)  Screen the egg is on
      x            (In)  X location
      y            (In)  Y location
      secrt        (In)  Secret number to set when dead

    Returns: An egg
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollActivatibleItem::createEgg(TrollScreen *scr,
  IShort x, IShort y, IUShort secrt)
{
 return new TrollActivatibleItem(scr, x, y, secrt, 0, &TrollEgg);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollActivatibleItem::createShield - Creates a shield.

    Parameters:
      scr          (In)  Screen the shield is on
      x            (In)  X location
      y            (In)  Y location
      secrt        (In)  Secret number to set when dead

    Returns: A shield
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollActivatibleItem::createShield(TrollScreen *scr, IShort x,
  IShort y, IUShort secrt)
{
 return new TrollActivatibleItem(scr, x, y, secrt, 0, &TrollShield);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollActivatibleItem::createSoldShield - Creates a shield for sale.

    Parameters:
      scr          (In)  Screen the shield is on
      x            (In)  X location
      y            (In)  Y location
      secrt        (In)  Secret number to set when dead

    Returns: A shield for sale
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollActivatibleItem::createSoldShield(TrollScreen *scr,
  IShort x, IShort y, IUShort secrt)
{
 return new TrollActivatibleItem(scr, x, y, secrt, 70, &TrollShield);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollActivatibleItem::createSword - Creates a sword.

    Parameters:
      scr          (In)  Screen the sword is on
      x            (In)  X location
      y            (In)  Y location
      secrt        (In)  Secret number to set when dead

    Returns: A sword
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollActivatibleItem::createSword(TrollScreen *scr, IShort x,
  IShort y, IUShort secrt)
{
 return new TrollActivatibleItem(scr, x, y, secrt, 0, &TrollNormalSword);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollActivatibleItem::pickUp - Gives the item to the character.

    Parameters:
      troll        (In)  Character to get the item
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollActivatibleItem::pickUp(TrollCharacter *troll)
{
 if (checkPickUp(troll))
 {
  troll->addCarriedItem(activateItem);
  dead = 1;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollActivatibleItem::TrollActivatibleItem - Constructor for an activatible
    item.

    Parameters:
      scr          (In)  Screen the activatible item is on
      xStart       (In)  X location
      yStart       (In)  Y location
      secrt        (In)  Secret number to set when dead
      actItem      (In)  The activation item
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollActivatibleItem::TrollActivatibleItem(TrollScreen *scr, IShort xStart,
  IShort yStart, IUShort secrt, IUShort cst, TrollCarriedItem *actItem)
  :TrollItem(scr, xStart, yStart, secrt, cst)
{
 activateItem = actItem;
 sprite = actItem->getSprite();
 shift = actItem->getShift();
 facing = 0;
 frame = 0;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSwordItem::TrollSwordItem - Constructor for the sword.

    Parameters:
      num          (In)  Item number of the sword
      str          (In)  Strength of the sword
      dmg          (In)  Damage of the sword
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollSwordItem::TrollSwordItem(IUShort num, IUShort str, IUShort dmg)
  :TrollCarriedItem(num, TROLL_SLOT_SWORD, str)
{
 sprite = TROLL_SPRITE_SWORD;
 shift = 0;
 damage = dmg;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSwordItem::activate - Activates the sword.

    Parameters:
      troll        (In)  Character using the sword
      screen       (In)  Screen being affected
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollSwordItem::activate(TrollCharacter *troll, TrollScreen *screen)
{
 TrollSwordThrust *sword;
 IShort xPos, yPos;

 troll->getLocation(xPos, yPos);
 switch (troll->getDirection())
 {
  case TROLL_UP:
   if (yPos > TROLL_BUFFER_Y + TROLL_SQUARE_Y - 1)
   {
    sword = new TrollSwordThrust(screen, xPos - TROLL_SQUARE_X / 4,
      yPos - TROLL_SQUARE_Y + 2, TROLL_UP, damage);
    screen->addCharacterProjectile(sword);
    troll->setPause(5);
   }
   break;
  case TROLL_DOWN:
   if (yPos < 200 - TROLL_SQUARE_Y * 2)
   {
    sword = new TrollSwordThrust(screen, xPos + TROLL_SQUARE_X / 4,
      yPos + TROLL_SQUARE_Y - 2, TROLL_DOWN, damage);
    screen->addCharacterProjectile(sword);
    troll->setPause(5);
   }
   break;
  case TROLL_RIGHT:
   if (xPos < 320 - TROLL_SQUARE_X * 2)
   {
    sword = new TrollSwordThrust(screen, xPos + TROLL_SQUARE_X - 4, yPos,
      TROLL_RIGHT, damage);
    screen->addCharacterProjectile(sword);
    troll->setPause(5);
   }
   break;
  case TROLL_LEFT:
   if (xPos > TROLL_SQUARE_X - 1)
   {
    sword = new TrollSwordThrust(screen, xPos - TROLL_SQUARE_X + 4, yPos,
      TROLL_LEFT, damage);
    screen->addCharacterProjectile(sword);
    troll->setPause(5);
   }
   break;
  default:
   break;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollEnchantedSwordItem::TrollEnchantedSwordItem - Constructor for the
    enchanted sword.

    Parameters:
      num          (In)  Item number of the sword
      str          (In)  Strength of the sword
      dmg          (In)  Damage of the sword
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollEnchantedSwordItem::TrollEnchantedSwordItem(IUShort num, IUShort str,
  IUShort dmg)
 : TrollSwordItem(num, str, dmg)
{
 sprite = TROLL_SPRITE_MAGICSWORD;
 activeFlag = false;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollEnchantedSwordItem::activate - Activates the sword.

    Parameters:
      troll        (In)  Character using the sword
      screen       (In)  Screen being affected
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollEnchantedSwordItem::activate(TrollCharacter *troll,
  TrollScreen *screen)
{
 TrollSwordProjectile *sword;
 IShort xPos, yPos;

 TrollSwordItem::activate(troll, screen);
 sword = (TrollSwordProjectile *)0;
 if ((troll->getHp() == troll->getTotalHp()) && (!activeFlag))
 {
  troll->getLocation(xPos, yPos);
  switch (troll->getDirection())
  {
   case TROLL_UP:
    if (yPos > TROLL_BUFFER_Y + TROLL_SQUARE_Y - 1)
    {
     sword = new TrollSwordProjectile(screen, xPos - TROLL_SQUARE_X / 4,
       yPos - TROLL_SQUARE_Y + 2, TROLL_UP, damage, this);
     screen->addCharacterProjectile(sword);
     troll->setPause(5);
    }
    break;
   case TROLL_DOWN:
    if (yPos < 200 - TROLL_SQUARE_Y * 2)
    {
     sword = new TrollSwordProjectile(screen, xPos + TROLL_SQUARE_X / 4,
       yPos + TROLL_SQUARE_Y - 2, TROLL_DOWN, damage, this);
     screen->addCharacterProjectile(sword);
     troll->setPause(5);
    }
    break;
   case TROLL_RIGHT:
    if (xPos < 320 - TROLL_SQUARE_X * 2)
    {
     sword = new TrollSwordProjectile(screen, xPos + TROLL_SQUARE_X - 4,
       yPos, TROLL_RIGHT, damage, this);
     screen->addCharacterProjectile(sword);
     troll->setPause(5);
    }
    break;
   case TROLL_LEFT:
    if (xPos > TROLL_SQUARE_X - 1)
    {
     sword = new TrollSwordProjectile(screen, xPos - TROLL_SQUARE_X + 4,
       yPos, TROLL_LEFT, damage, this);
     screen->addCharacterProjectile(sword);
     troll->setPause(5);
    }
    break;
   default:
    break;
  }
  if (sword)
  {
   activeFlag = true;
  }
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollEnchantedSwordItem::clearActiveFlag - Clear active sword projectile
    flag.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollEnchantedSwordItem::clearActiveFlag()
{
 activeFlag = false;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSwordThrust::TrollSwordThrust - Constructor for the sword blade.

    Parameters:
      screen       (In)  Screen the sword is on
      xStart       (In)  X location
      yStart       (In)  Y location
      dir          (In)  Direction of attack
      dmg          (In)  Damage of the attack
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollSwordThrust::TrollSwordThrust(TrollScreen *screen, IShort xStart,
  IShort yStart, IUShort dir, IUShort dmg)
 : TrollProjectile(screen, dmg)
{
 switch (dir)
 {
  case TROLL_UP:
   sprite = TROLL_SPRITE_SWORD_V;
   facing = 1;
   direction = TROLL_UP;
   break;
  case TROLL_DOWN:
   sprite = TROLL_SPRITE_SWORD_V;
   facing = 0;
   direction = TROLL_DOWN;
   break;
  case TROLL_RIGHT:
   sprite = TROLL_SPRITE_SWORD_H;
   facing = 1;
   direction = TROLL_RIGHT;
   break;
  case TROLL_LEFT:
   sprite = TROLL_SPRITE_SWORD_H;
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
  TrollSwordThrust::react - Performs animation sequence for sword blade.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollSwordThrust::react()
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
  TrollSwordThrust::takeHit - Called when the sword blade hits something.

    Parameters:
      hitBy        (In)  Thing it hit
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollSwordThrust::takeHit(TrollThing *hitBy)
{
 // Sword thrust doesn't die from hitting things
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSwordProjectile::TrollSwordProjectile - Constructor for the sword
    blade.

    Parameters:
      screen       (In)  Screen the sword is on
      xStart       (In)  X location
      yStart       (In)  Y location
      dir          (In)  Direction of attack
      dmg          (In)  Damage of the attack
      swrd         (In)  Sword that fired the projectile
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollSwordProjectile::TrollSwordProjectile(TrollScreen *screen,
  IShort xStart, IShort yStart, IUShort dir, IUShort dmg,
  TrollEnchantedSwordItem *swrd)
 : TrollProjectile(screen, dmg)
{
 sword = swrd;
 switch (dir)
 {
  case TROLL_UP:
   sprite = TROLL_SPRITE_SWORD_V;
   facing = 1;
   direction = TROLL_UP;
   break;
  case TROLL_DOWN:
   sprite = TROLL_SPRITE_SWORD_V;
   facing = 0;
   direction = TROLL_DOWN;
   break;
  case TROLL_RIGHT:
   sprite = TROLL_SPRITE_SWORD_H;
   facing = 1;
   direction = TROLL_RIGHT;
   break;
  case TROLL_LEFT:
   sprite = TROLL_SPRITE_SWORD_H;
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
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSwordProjectile::~TrollSwordProjectile - Destructor.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollSwordProjectile::~TrollSwordProjectile()
{
 sword->clearActiveFlag();
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSwordProjectile::die - Kills sword blade.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollSwordProjectile::die()
{
 TrollThing::die();
 sword->clearActiveFlag();
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSwordProjectile::react - Move sword blade.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollSwordProjectile::react()
{
 switch (direction)
 {
  case TROLL_UP:
   if (y <= TROLL_BUFFER_Y + 5)
   {
    dead = 1;
   }
   else
   {
    y -= 6;
   }
   break;
  case TROLL_DOWN:
   if (y >= 200 - 11)
   {
    dead = 1;
   }
   else
   {
    y += 6;
   }
   break;
  case TROLL_RIGHT:
   if (x >= 320 - 15)
   {
    dead = 1;
   }
   else
   {
    x += 8;
   }
   break;
  case TROLL_LEFT:
   if (x <= 7)
   {
    dead = 1;
   }
   else
   {
    x -= 8;
   }
   break;
  default:
   // Should never get here
   break;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSwordProjectiles::takeHit - Called when the sword blade hits
    something.

    Parameters:
      hitBy        (In)  Thing it hit
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollSwordProjectile::takeHit(TrollThing *hitBy)
{
 TrollMonster *mon;
 if ((mon = dynamic_cast<TrollMonster *>(hitBy)))
 {
  // if the monster doesn't hurt the player don't hit him
  if (mon->getDamage())
  {
   dead = 1;
  }
 }
}

TrollAnhkItem::TrollAnhkItem(IUShort num, IUShort str)
  :TrollCarriedItem(num, TROLL_SLOT_ANHK, str)
{
 sprite = TROLL_SPRITE_ANHK;
 shift = 0;
}

void TrollAnhkItem::activate(TrollCharacter *troll, TrollScreen *screen)
{
 if (!checkActiveAnhk(screen))
 {
  screen->addCharacterProjectile(
    new TrollAnhkProjectile(screen, troll->getX(), troll->getY()));
 }
}

bool TrollAnhkItem::checkActiveAnhk(TrollScreen *screen)
{
 if (anhk)
 {
  return true;
 }
 return false;
}

TrollAnhkProjectile *TrollAnhkItem::anhk = NULL;

TrollAnhkProjectile::TrollAnhkProjectile(TrollScreen *screen, IShort xStart, IShort yStart)
 : TrollProjectile(screen, 0)
{
 sprite = 0;
 facing = 0;
 direction = 0;
 x = xStart;
 y = yStart;
 TrollAnhkItem::anhk = this;
}

TrollAnhkProjectile::~TrollAnhkProjectile()
{
 TrollAnhkItem::anhk = NULL;
}

void TrollAnhkProjectile::draw(IScreen drawscreen)
{
 // Don't draw anything
}

void TrollAnhkProjectile::react()
{
}

void TrollAnhkProjectile::takeHit(TrollThing *hitBy)
{
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollShieldItem::TrollShieldItem - Constructor for the shield.

    Parameters:
      num          (In)  Item number of the shield
      str          (In)  Strength of the shield
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollShieldItem::TrollShieldItem(IUShort num, IUShort str)
  :TrollCarriedItem(num, TROLL_SLOT_SHIELD, str)
{
 sprite = TROLL_SPRITE_SHIELD;
 shift = 10;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollShieldItem::activate - Activates the sheild.

    Parameters:
      troll        (In)  Character using the sheild
      screen       (In)  Screen being affected
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollShieldItem::activate(TrollCharacter *troll, TrollScreen *screen)
{
 TrollShieldProjectile *shield;
 IShort xPos, yPos;

 troll->getLocation(xPos, yPos);
 switch (troll->getDirection())
 {
  case TROLL_UP:
   if (yPos > TROLL_BUFFER_Y + TROLL_SQUARE_Y - 1)
   {
    shield = new TrollShieldProjectile(screen, xPos + TROLL_SQUARE_X / 4,
      yPos - TROLL_SQUARE_Y / 4, TROLL_UP, shift, troll);
    screen->addCharacterProjectile(shield);
   }
   break;
  case TROLL_DOWN:
   if (yPos < 200 - TROLL_SQUARE_Y * 2)
   {
    shield = new TrollShieldProjectile(screen, xPos - TROLL_SQUARE_X / 4,
      yPos + TROLL_SQUARE_X / 4, TROLL_DOWN, shift, troll);
    screen->addCharacterProjectile(shield);
   }
   break;
  case TROLL_RIGHT:
   if (xPos < 320 - TROLL_SQUARE_X * 2)
   {
    shield = new TrollShieldProjectile(screen, xPos + TROLL_SQUARE_X / 4,
      yPos, TROLL_RIGHT, shift, troll);
    screen->addCharacterProjectile(shield);
   }
   break;
  case TROLL_LEFT:
   if (xPos > TROLL_SQUARE_X - 1)
   {
    shield = new TrollShieldProjectile(screen, xPos - TROLL_SQUARE_X / 4,
      yPos, TROLL_LEFT, shift, troll);
    screen->addCharacterProjectile(shield);
   }
   break;
  default:
   break;
 }
}

TrollShieldProjectile::TrollShieldProjectile(TrollScreen *screen,
  IShort xStart, IShort yStart, IUShort dir, IUByte shft,
  TrollCharacter *trll)
 : TrollProjectile(screen, 0)
{
 sprite = TROLL_SPRITE_SHIELD;
 facing = dir;
 direction = dir;
 x = xStart;
 y = yStart;
 shift = shft;
 frame = 0;
 troll = trll;
 trll->getLocation(trollX, trollY);
 trollFacing = trll->getFacing();
 trollFrame = trll->getFrame();
}

void TrollShieldProjectile::react()
{
 if ((troll->getX() != trollX) || (troll->getY() != trollY) ||
   (troll->getFacing() != trollFacing) || (troll->getFrame() != trollFrame))
 {
  dead = 1;
 }
 else
 {
  screen->checkCollisionMonsterProjectile(this);
 }
}

void TrollShieldProjectile::takeHit(TrollThing *hitBy)
{
}

TrollPhoenixProjectile::TrollPhoenixProjectile(TrollScreen *screen, IShort xStart, IShort yStart,
  TrollCharacter *trll)
 : TrollProjectile(screen, 0)
{
 findTroll = true;
 delayTime = 4;
 sprite = TROLL_SPRITE_PHOENIX;
 facing = TROLL_DOWN;
 direction = TROLL_DOWN;
 const Sprite *pic = TrollSpriteHandler.getSprite(sprite);
 trollOffset = (pic->getXSize() - TROLL_SQUARE_X) / 2;
 x = xStart;
 y = yStart;
 x -= trollOffset;
 shift = 0;
 frame = 0;
 troll = trll;
 secondScreen = false;
}

void TrollPhoenixProjectile::react()
{
 if (findTroll)
 {
  IShort xLoc, yLoc;
  troll->getLocation(xLoc, yLoc);
  if ((xLoc == x + trollOffset) && (y == yLoc))
  {
   findTroll = false;
   keep = true;
   direction = facing = troll->getFacing();
   troll->setControlled(true);
  }
  else
  {
   if (delayTime)
   {
    delayTime--;
    if (x + trollOffset < xLoc)
    {
     x += TROLL_SQUARE_X / 4;
     direction = facing = TROLL_RIGHT;
    }
    else if (x + trollOffset > xLoc)
    {
     x -= TROLL_SQUARE_X / 4;
     direction = facing = TROLL_LEFT;
    }
    if (y < yLoc)
    {
     y += TROLL_SQUARE_Y / 4;
     direction = facing = TROLL_DOWN;
    }
    else if (y> yLoc)
    {
     y -= TROLL_SQUARE_Y / 4;
     direction = facing = TROLL_UP;
    }
   }
   else
   {
    dead = 1;
    screen->addMonster(new TrollExplosion(screen, x + trollOffset, y, 10, 0));
   }
  }
 }
 else
 {
  switch (direction)
  {
   case TROLL_UP:
    if ((secondScreen) && (y == TROLL_BUFFER_Y + TROLL_SCREEN_Y / 2 * TROLL_SQUARE_Y))
    {
     dead = 1;
    }
    else if (y == TROLL_BUFFER_Y)
    {
     troll->setControlled(TROLL_CONTROLLED_SCROLLUP);
     secondScreen = true;
    }
    else
    {
     y -= TROLL_SQUARE_Y / 4;
     troll->setLocation(x + trollOffset, y);
    }
    break;
   case TROLL_DOWN:
    if ((secondScreen) && (y == TROLL_BUFFER_Y + TROLL_SCREEN_Y / 2 * TROLL_SQUARE_Y))
    {
     dead = 1;
    }
    else if (y == TROLL_BUFFER_Y + (TROLL_SCREEN_Y - 1) * TROLL_SQUARE_Y)
    {
     troll->setControlled(TROLL_CONTROLLED_SCROLLDOWN);
     secondScreen = true;
    }
    else
    {
     y += TROLL_SQUARE_Y / 4;
     troll->setLocation(x + trollOffset, y);
    }
    break;
   case TROLL_RIGHT:
    if ((secondScreen) && (x == 0))
     x -= trollOffset;
    if ((secondScreen) && (x + trollOffset == TROLL_SCREEN_X / 2 * TROLL_SQUARE_X))
    {
     dead = 1;
    }
    else if (x + trollOffset == (TROLL_SCREEN_X - 1) * TROLL_SQUARE_X)
    {
     troll->setControlled(TROLL_CONTROLLED_SCROLLRIGHT);
     secondScreen = true;
    }
    else
    {
     x += TROLL_SQUARE_X / 4;
     troll->setLocation(x + trollOffset, y);
    }
    break;
   case TROLL_LEFT:
    if ((secondScreen) && (x == ((TROLL_SCREEN_X - 1) * TROLL_SQUARE_X) - (trollOffset * 2)))
     x += trollOffset;
    if ((secondScreen) && (x + trollOffset == TROLL_SCREEN_X / 2 * TROLL_SQUARE_X))
    {
     dead = 1;
    }
    else if (x + trollOffset == 0)
    {
     troll->setControlled(TROLL_CONTROLLED_SCROLLLEFT);
     secondScreen = true;
    }
    else
    {
     x -= TROLL_SQUARE_X / 4;
     troll->setLocation(x + trollOffset, y);
    }
    break;
  }
  if (dead)
   troll->setControlled(0);
 }
}

void TrollPhoenixProjectile::takeHit(TrollThing *hitBy)
{
}

TrollEggProjectile::TrollEggProjectile(TrollScreen *screen, IShort xStart, IShort yStart,
  TrollCharacter *trll)
 : TrollProjectile(screen, 0)
{
 sprite = TROLL_SPRITE_EGG;
 x = xStart;
 y = yStart;
 facing = 0;
 direction = 0;
 shift = 0;
 frame = 0;
 troll = trll;
 delayTime = 4;
}

void TrollEggProjectile::react()
{
 if (delayTime)
  delayTime--;
 else
 {
  if (sprite == TROLL_SPRITE_EGG)
  {
   IUShort sprt;
   IUByte shft;
   // Determine if egg is on a nest
   IUShort xSquare = x / TROLL_SQUARE_X;
   IUShort ySquare = (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y;
   IUShort xOffset = x % TROLL_SQUARE_X;
   IUShort yOffset = (y - TROLL_BUFFER_Y) % TROLL_SQUARE_Y;
   screen->getBackground(xSquare, ySquare, sprt, shft);
   if (sprt != TROLL_SPRITE_NEST)
   {
    if (xOffset != 0)
    {
     screen->getBackground(xSquare + 1, ySquare, sprt, shft);
     if (sprt != TROLL_SPRITE_NEST)
     {
      if (yOffset != 0)
      {
       screen->getBackground(xSquare + 1, ySquare + 1, sprt, shft);
       if (sprt != TROLL_SPRITE_NEST)
       {
        dead = 1;
        return;
       }
      }
      else
      {
       dead = 1;
       return;
      }
     }
    }
    else if (yOffset != 0)
    {
     screen->getBackground(xSquare, ySquare + 1, sprt, shft);
     if (sprt != TROLL_SPRITE_NEST)
     {
      dead = 1;
      return;
     }
    }
    else
    {
     dead = 1;
     return;
    }
   }
   dead = 1;
   screen->addCharacterProjectile(new TrollPhoenixProjectile(screen, x, y, troll));
  }
 }
}

void TrollEggProjectile::takeHit(TrollThing *hitBy)
{
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollEggItem::TrollEggItem - Constructor for the egg.

    Parameters:
      num          (In)  Item number of the egg
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollEggItem::TrollEggItem(IUShort num)
  :TrollCarriedItem(num, TROLL_SLOT_EGG, 1)
{
 sprite = TROLL_SPRITE_EGG;
 shift = 0;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollEggItem::activate - Activates the egg.

    Parameters:
      troll        (In)  Character using the egg
      screen       (In)  Screen being affected
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollEggItem::activate(TrollCharacter *troll, TrollScreen *screen)
{
 // Creates phoenix if there is a nest
 IShort xPos, yPos;
 TrollEggProjectile *egg;
 troll->getLocation(xPos, yPos);
 switch (troll->getDirection())
 {
  case TROLL_UP:
   if (yPos > TROLL_BUFFER_Y + TROLL_SQUARE_Y - 1)
   {
    egg = new TrollEggProjectile(screen, xPos,
      yPos - TROLL_SQUARE_Y, troll);
    screen->addCharacterProjectile(egg);
    troll->setPause(5);
   }
   break;
  case TROLL_DOWN:
   if (yPos < 200 - TROLL_SQUARE_Y * 2)
   {
    egg = new TrollEggProjectile(screen, xPos,
      yPos + TROLL_SQUARE_Y, troll);
    screen->addCharacterProjectile(egg);
    troll->setPause(5);
   }
   break;
  case TROLL_RIGHT:
   if (xPos < 320 - TROLL_SQUARE_X * 2)
   {
    egg = new TrollEggProjectile(screen, xPos + TROLL_SQUARE_X, yPos, troll);
    screen->addCharacterProjectile(egg);
    troll->setPause(5);
   }
   break;
  case TROLL_LEFT:
   if (xPos > TROLL_SQUARE_X - 1)
   {
    egg = new TrollEggProjectile(screen, xPos - TROLL_SQUARE_X, yPos, troll);
    screen->addCharacterProjectile(egg);
    troll->setPause(5);
   }
   break;
  default:
   break;
 }
}

TrollLimitedItem::TrollLimitedItem(IUShort num, IUShort slt, IUShort str)
  : TrollCarriedItem(num, slt, str)
{
 number = 0;
}

void TrollLimitedItem::draw(IScreen drawscreen, IShort x, IShort y)
{
 char text[10];

 TrollCarriedItem::draw(drawscreen, x, y);
 sprintf(text, "%d", number);
 ITextDraw(drawscreen, x - (strlen(text) * 4), y - 4, 255, text);
}

IUShort TrollLimitedItem::getNumber()
{
 return number;
}

void TrollLimitedItem::read(BinaryReadFile &f)
{
 f.readUShort(number);
}

void TrollLimitedItem::setNumber(IUShort num)
{
 number = num;
}

void TrollLimitedItem::write(BinaryWriteFile &f)
{
 f.writeUShort(number);
}

TrollThing *TrollLimitedActivatibleItem::createKey(TrollScreen *scr,
  IShort x, IShort y, IUShort secrt)
{
 return new TrollLimitedActivatibleItem(scr, x, y, secrt, 0, &TrollKey);
}

TrollThing *TrollLimitedActivatibleItem::createSoldKey(TrollScreen *scr,
  IShort x, IShort y, IUShort secrt)
{
 return new TrollLimitedActivatibleItem(scr, x, y, secrt, 60, &TrollKey);
}

void TrollLimitedActivatibleItem::pickUp(TrollCharacter *troll)
{
 TrollLimitedItem *actItem = static_cast<TrollLimitedItem *>(activateItem);

 if (checkPickUp(troll))
 {
  if (troll->getCarriedItem(activateItem->getSlot()) == activateItem)
  {
   actItem->setNumber(actItem->getNumber() + 1);
  }
  else
  {
   actItem->setNumber(1);
   troll->addCarriedItem(activateItem);
  }
  dead = 1;
 }
}

TrollLimitedActivatibleItem::TrollLimitedActivatibleItem(TrollScreen *scr,
  IShort xStart, IShort yStart, IUShort secrt, IUShort cst,
  TrollCarriedItem *actItem)
  : TrollActivatibleItem(scr, xStart, yStart, secrt, cst, actItem)
{
}

TrollKeyItem::TrollKeyItem()
  : TrollLimitedItem(1, TROLL_SLOT_KEY, 1)
{
 sprite = TROLL_SPRITE_KEY;
 shift = 0;
}

void TrollKeyItem::activate(TrollCharacter *troll, TrollScreen *screen)
{
 TrollKeyProjectile *key;
 IShort xPos, yPos;

 troll->getLocation(xPos, yPos);
 switch (troll->getDirection())
 {
  case TROLL_UP:
   //if (yPos > TROLL_BUFFER_Y + TROLL_SQUARE_Y - 1)
   {
    key = new TrollKeyProjectile(screen, xPos, yPos - 15, TROLL_UP, troll,
      slot);
    screen->addCharacterProjectile(key);
    troll->setPause(1);
   }
   break;
  case TROLL_DOWN:
   if (yPos < 200 - TROLL_SQUARE_Y * 2)
   {
    key = new TrollKeyProjectile(screen, xPos, yPos + 14, TROLL_DOWN, troll,
      slot);
    screen->addCharacterProjectile(key);
    troll->setPause(1);
   }
   break;
  case TROLL_RIGHT:
   if (xPos < 320 - TROLL_SQUARE_X * 2)
   {
    key = new TrollKeyProjectile(screen, xPos + 18, yPos, TROLL_RIGHT, troll,
      slot);
    screen->addCharacterProjectile(key);
    troll->setPause(1);
   }
   break;
  case TROLL_LEFT:
   if (xPos > TROLL_SQUARE_X - 1)
   {
    key = new TrollKeyProjectile(screen, xPos - 18, yPos, TROLL_LEFT, troll,
      slot);
    screen->addCharacterProjectile(key);
    troll->setPause(1);
   }
   break;
  default:
   break;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollKeyProjectile::TrollKeyProjectile - Constructor for the key.

    Parameters:
      screen       (In)  Screen the flyer is on
      xStart       (In)  X location
      yStart       (In)  Y location
      dir          (In)  Direction of attack
      trll         (In)  Troll using the key
      slt          (In)  Slot number of key item
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollKeyProjectile::TrollKeyProjectile(TrollScreen *screen,
  IShort xStart, IShort yStart, IUShort dir, TrollCharacter *trll,
  IUShort slt)
  :TrollProjectile(screen, 0)
{
 sprite = TROLL_SPRITE_KEY;
 facing = direction = dir;
 troll = trll;
 slot = slt;
 x = xStart;
 y = yStart;
 shift = 0;
 frame = 0;
 time = 2;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollKeyProjectile::decreaseNumber - Decrements the number of keys.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollKeyProjectile::decreaseNumber()
{
 TrollKeyItem *key = static_cast<TrollKeyItem*>(troll->getCarriedItem(slot));
 IUShort num = key->getNumber() - 1;
 if (num)
 {
  key->setNumber(num);
 }
 else
 {
  troll->dropCarriedItem(slot);
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollKeyProjectile::react - Waits for time to run out.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollKeyProjectile::react()
{
 time--;
 if (time == 0)
 {
  dead = 1;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollKeyProjectile::takeHit - Called when the key hits something.

    Parameters:
      hitBy        (In)  Thing it hit
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollKeyProjectile::takeHit(TrollThing *hitBy)
{
 // Key projectile doesn't die from hitting things
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollTimedItem::react - Waits for time to run out.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollTimedItem::react()
{
 if (cost == 0)
 {
  life--;
  if (!life)
  {
   dead = 1;
  }
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollTimedItem::TrollTimedItem - Constructor for timed items.

    Parameters:
      scr          (In)  Screen the item is on
      x            (In)  X location
      y            (In)  Y location
      secrt        (In)  Secret number to set when dead
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollTimedItem::TrollTimedItem(TrollScreen *scr, IShort xStart,
  IShort yStart, IUShort secrt /*= 0*/, IUShort cst /*= 0*/)
  : TrollItem(scr, xStart, yStart, secrt, cst)
{
 life = TROLL_ITEM_LIFESPAN;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGold::createGoldCoin - Creates a gold coin.

    Parameters:
      scr          (In)  Screen the gold coin is on
      x            (In)  X location
      y            (In)  Y location
      secrt        (In)  Secret number to set when dead

    Returns: A gold coin
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollGold::createGoldCoin(TrollScreen *scr, IShort x, IShort y,
  IUShort secrt)
{
 return new TrollGold(scr, x, y, secrt, 1);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGold::create3GoldCoins - Creates 3 gold coins.

    Parameters:
      scr          (In)  Screen the 3 gold coins are on
      x            (In)  X location
      y            (In)  Y location
      secrt        (In)  Secret number to set when dead

    Returns: 3 gold coins
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollGold::create3GoldCoins(TrollScreen *scr, IShort x,
    IShort y, IUShort secrt)
{
 return new TrollGold(scr, x, y, secrt, 3);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGold::pickUp - Gives the gold to the character.

    Parameters:
      troll        (In)  Character to get the item
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollGold::pickUp(TrollCharacter *troll)
{
 troll->setGold(troll->getGold() + amount);
 dead = 1;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGold::TrollGold - Constructor for gold coins.

    Parameters:
      scr          (In)  Screen the gold coins are on
      x            (In)  X location
      y            (In)  Y location
      secrt        (In)  Secret number to set when dead
      amt          (In)  Amount of gold coins
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollGold::TrollGold(TrollScreen *scr, IShort xStart, IShort yStart,
  IUShort secrt, IUShort amt)
 : TrollTimedItem(scr, xStart, yStart, secrt)
{
 if (amt == 1)
 {
  sprite = TROLL_SPRITE_GOLD;
 }
 else
 {
  sprite = TROLL_SPRITE_GOLD3;
 }
 amount = amt;
 shift = 0;
 facing = 0;
 frame = 0;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollHitPoint::createHitPoint - Creates a hit point.

    Parameters:
      scr          (In)  Screen the hit point is on
      x            (In)  X location
      y            (In)  Y location
      secrt        (In)  Secret number to set when dead

    Returns: A hit point
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollHitPoint::createHitPoint(TrollScreen *scr, IShort x,
  IShort y, IUShort secrt)
{
 return new TrollHitPoint(scr, x, y, secrt, 0, 1);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollHitPoint::createSoldHitPoint - Creates a sold hit point.

    Parameters:
      scr          (In)  Screen the hit point is on
      x            (In)  X location
      y            (In)  Y location
      secrt        (In)  Secret number to set when dead

    Returns: A hit point for sale
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollHitPoint::createSoldHitPoint(TrollScreen *scr, IShort x,
  IShort y, IUShort secrt)
{
 return new TrollHitPoint(scr, x, y, secrt, 20, 1);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollHitPoints::pickUp - Gives the hit points to the character.

    Parameters:
      troll        (In)  Character to get the item
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollHitPoint::pickUp(TrollCharacter *troll)
{
 if (checkPickUp(troll))
 {
  troll->addHp(amount);
  dead = 1;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollHitPoints::react - Performs animation sequence for hit points.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollHitPoint::react()
{
 time++;
 if (time == 6)
 {
  time = 0;
  shift += 5;
  shift %= 15;
 }
 TrollTimedItem::react();
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollHitPoint::TrollHitPoint - Constructor for hit points.

    Parameters:
      scr          (In)  Screen the hit points are on
      x            (In)  X location
      y            (In)  Y location
      secrt        (In)  Secret number to set when dead
      amt          (In)  Amount of hit points
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollHitPoint::TrollHitPoint(TrollScreen *scr, IShort xStart, IShort yStart,
  IUShort secrt, IUShort cst, IUShort amt)
 : TrollTimedItem(scr, xStart, yStart, secrt, cst)
{
 sprite = TROLL_SPRITE_HITPOINT;
 amount = amt;
 shift = 0;
 time = 0;
 facing = 0;
 frame = 0;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollLevelMap::createLevelMap - Creates a level map.

    Parameters:
      scr          (In)  Screen the level map is on
      x            (In)  X location
      y            (In)  Y location
      secrt        (In)  Secret number to set when dead

    Returns: A level map
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollThing *TrollLevelMap::createLevelMap(TrollScreen *scr, IShort x,
  IShort y, IUShort secrt)
{
 return new TrollLevelMap(scr, x, y, secrt);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollLevelMap::pickUp - Gives the level map to the character.

    Parameters:
      troll        (In)  Character to get the item
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollLevelMap::pickUp(TrollCharacter *troll)
{
 IUShort i, k;
 TrollGame *game = screen->getGame();
 const char *levelName = game->getLevelName();

 for (i = 0; i < TROLL_LEVEL_X; i++)
 {
  for (k = 0; k < TROLL_LEVEL_Y; k++)
  {
   troll->setMapInfo(levelName, i, k, game->getMapInfo(i, k));
  }
 }
 dead = 1;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollLevelMap::TrollLevelMap - Constructor for a level map.

    Parameters:
      scr          (In)  Screen the level map is on
      x            (In)  X location
      y            (In)  Y location
      secrt        (In)  Secret number to set when dead
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollLevelMap::TrollLevelMap(TrollScreen *scr, IShort xStart, IShort yStart,
  IUShort secrt)
 : TrollItem(scr, xStart, yStart, secrt)
{
 sprite = TROLL_SPRITE_MAP;
 shift = 0;
 facing = 0;
 frame = 0;
}

