#ifndef __ITEMS_H
#define __ITEMS_H
/*-------------------------------------------------------------------------*\
  <items.h> -- Troll Bridge dynamically linked library - Items

  Date      Programmer  Description
  04/17/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Classes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollActivatibleItem: public TrollItem
{
 public:
  static TrollThing *createAnhk(TrollScreen *scr, IShort x, IShort y,
    IUShort secrt);
  static TrollThing *createEgg(TrollScreen *scr, IShort x, IShort y,
    IUShort secrt);
  static TrollThing *createEnchantedSword(TrollScreen *scr, IShort x,
    IShort y, IUShort secrt);
  static TrollThing *createShield(TrollScreen *scr, IShort x,
    IShort y, IUShort secrt);
  static TrollThing *createSoldShield(TrollScreen *scr, IShort x,
    IShort y, IUShort secrt);
  static TrollThing *createSword(TrollScreen *scr, IShort x,
    IShort y, IUShort secrt);

  void pickUp(TrollCharacter *troll);

 protected:
  TrollActivatibleItem(TrollScreen *scr, IShort xStart, IShort yStart,
    IUShort secrt, IUShort cst, TrollCarriedItem *actItem);

  TrollCarriedItem *activateItem;
};

class TrollSwordItem: public TrollCarriedItem
{
 public:
  TrollSwordItem(IUShort num, IUShort str, IUShort dmg);
  void activate(TrollCharacter *troll, TrollScreen *screen);

 protected:
  IUShort damage;
};

class TrollEnchantedSwordItem: public TrollSwordItem
{
 public:
  TrollEnchantedSwordItem(IUShort num, IUShort str, IUShort dmg);
  void activate(TrollCharacter *troll, TrollScreen *screen);
  void clearActiveFlag();

 protected:
  bool activeFlag;
};

class TrollSwordThrust: public TrollProjectile
{
 public:
  TrollSwordThrust(TrollScreen *screen, IShort xStart, IShort yStart,
    IUShort dir,IUShort dmg);
  void react();
  void takeHit(TrollThing *hitBy);

 protected:
  IUByte state;
};

class TrollSwordProjectile: public TrollProjectile
{
 public:
  TrollSwordProjectile(TrollScreen *screen, IShort xStart, IShort yStart,
    IUShort dir, IUShort dmg, TrollEnchantedSwordItem *swrd);
  ~TrollSwordProjectile();
  void die();
  void react();
  void takeHit(TrollThing *hitBy);

 protected:
  TrollEnchantedSwordItem *sword;
};

class TrollAnhkProjectile: public TrollProjectile
{
 public:
  TrollAnhkProjectile(TrollScreen *screen, IShort xStart, IShort yStart);
  ~TrollAnhkProjectile();
  void draw(IScreen drawscreen);
  void react();
  void takeHit(TrollThing *hitBy);
};

class TrollAnhkItem: public TrollCarriedItem
{
 friend class TrollAnhkProjectile;

 public:
  TrollAnhkItem(IUShort num, IUShort str);
  void activate(TrollCharacter *troll, TrollScreen *screen);

  static bool checkActiveAnhk(TrollScreen *screen);

 protected:
  static TrollAnhkProjectile *anhk;
};

class TrollShieldItem: public TrollCarriedItem
{
 public:
  TrollShieldItem(IUShort num, IUShort str);
  void activate(TrollCharacter *troll, TrollScreen *screen);
};

class TrollShieldProjectile: public TrollProjectile
{
 public:
  TrollShieldProjectile(TrollScreen *screen, IShort xStart, IShort yStart,
    IUShort dir, IUByte shft, TrollCharacter *trll);
  void react();
  void takeHit(TrollThing *hitBy);

 protected:
  TrollCharacter *troll;
  IShort trollX, trollY;
  IUShort trollFacing, trollFrame;
};

class TrollEggItem: public TrollCarriedItem
{
 public:
  TrollEggItem(IUShort num);
  void activate(TrollCharacter *troll, TrollScreen *screen);
};

class TrollLimitedItem: public TrollCarriedItem
{
 public:
  TrollLimitedItem(IUShort num, IUShort slt, IUShort str);
  void draw(IScreen drawscreen, IShort x, IShort y);
  IUShort getNumber();
  void read(BinaryReadFile &f);
  void setNumber(IUShort num);
  void write(BinaryWriteFile &f);

 private:
  IUShort number;
};

class TrollLimitedActivatibleItem: public TrollActivatibleItem
{
 public:
  static TrollThing *createKey(TrollScreen *scr, IShort x,
    IShort y, IUShort secrt);
  static TrollThing *createSoldKey(TrollScreen *scr, IShort x,
    IShort y, IUShort secrt);

  void pickUp(TrollCharacter *troll);

 protected:
  TrollLimitedActivatibleItem(TrollScreen *scr, IShort xStart,
    IShort yStart, IUShort secrt, IUShort cst, TrollCarriedItem *actItem);
};

class TrollKeyItem: public TrollLimitedItem
{
 public:
  TrollKeyItem();
  void activate(TrollCharacter *troll, TrollScreen *screen);

 protected:
};

class TrollKeyProjectile: public TrollProjectile
{
 public:
  TrollKeyProjectile(TrollScreen *screen, IShort xStart, IShort yStart,
    IUShort dir, TrollCharacter *trll, IUShort slt);
  void decreaseNumber();
  void react();
  void takeHit(TrollThing *hitBy);

 protected:
  IUByte time;
  TrollCharacter *troll;
  IUShort slot;
};

class TrollTimedItem: public TrollItem
{
 public:
  void react();

 protected:
  TrollTimedItem(TrollScreen *scr, IShort xStart, IShort yStart,
    IUShort secrt = 0, IUShort cst = 0);

  IUShort life;
};

class TrollGold: public TrollTimedItem
{
 public:
  static TrollThing *createGoldCoin(TrollScreen *scr, IShort x,
    IShort y, IUShort secrt);
  static TrollThing *create3GoldCoins(TrollScreen *scr, IShort x,
    IShort y, IUShort secrt);

  void pickUp(TrollCharacter *troll);

 protected:
  TrollGold(TrollScreen *scr, IShort xStart, IShort yStart,
    IUShort secrt, IUShort amt);

  IUShort amount;
};

class TrollHitPoint: public TrollTimedItem
{
 public:
  static TrollThing *createHitPoint(TrollScreen *scr, IShort x,
    IShort y, IUShort secrt);
  static TrollThing *createSoldHitPoint(TrollScreen *scr, IShort x,
    IShort y, IUShort secrt);

  void pickUp(TrollCharacter *troll);
  void react();

 protected:
  TrollHitPoint(TrollScreen *scr, IShort xStart, IShort yStart,
    IUShort secrt, IUShort cst, IUShort amt);

  IUShort amount;
  IUShort time;
};

class TrollLevelMap: public TrollItem
{
 public:
  static TrollThing *createLevelMap(TrollScreen *scr, IShort x,
    IShort y, IUShort secrt);

  void pickUp(TrollCharacter *troll);

 protected:
  TrollLevelMap(TrollScreen *scr, IShort xStart, IShort yStart,
    IUShort secrt);
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Global variables
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
extern TrollSwordItem TrollNormalSword;
extern TrollEnchantedSwordItem TrollEnchantedSword;
extern TrollKeyItem TrollKey;
extern TrollAnhkItem TrollAnhk;
extern TrollShieldItem TrollShield;
extern TrollEggItem TrollEgg;

#endif

