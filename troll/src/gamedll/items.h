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
  static TrollThing *createAnhk(TrollScreen *scr, IUShort x, IUShort y,
    IUShort secrt = 0);
  static TrollThing *createEnchantedSword(TrollScreen *scr, IUShort x,
    IUShort y, IUShort secrt = 0);
  static TrollThing *createShield(TrollScreen *scr, IUShort x,
    IUShort y, IUShort secrt = 0);
  static TrollThing *createSoldShield(TrollScreen *scr, IUShort x,
    IUShort y, IUShort secrt = 0);
  static TrollThing *createSword(TrollScreen *scr, IUShort x,
    IUShort y, IUShort secrt = 0);

  void pickUp(TrollCharacter *troll);

 protected:
  TrollActivatibleItem(TrollScreen *scr, IUShort xStart, IUShort yStart,
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
  TrollSwordThrust(TrollScreen *screen, IUShort xStart, IUShort yStart,
    IUShort dir,IUShort dmg);
  void react();
  void takeHit(TrollThing *hitBy);

 protected:
  IUByte state;
};

class TrollSwordProjectile: public TrollProjectile
{
 public:
  TrollSwordProjectile(TrollScreen *screen, IUShort xStart, IUShort yStart,
    IUShort dir, IUShort dmg, TrollEnchantedSwordItem *swrd);
  ~TrollSwordProjectile();
  void die();
  void react();
  void takeHit(TrollThing *hitBy);

 protected:
  TrollEnchantedSwordItem *sword;
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

class TrollAnhkProjectile: public TrollProjectile
{
 public:
  TrollAnhkProjectile(TrollScreen *screen, IUShort xStart, IUShort yStart);
  ~TrollAnhkProjectile();
  void draw(IScreen drawscreen);
  void react();
  void takeHit(TrollThing *hitBy);
};

class TrollShieldItem: public TrollCarriedItem
{
 public:
  TrollShieldItem(IUShort num, IUShort str);
  void activate(TrollCharacter *troll, TrollScreen *screen);

 protected:
};

class TrollShieldProjectile: public TrollProjectile
{
 public:
  TrollShieldProjectile(TrollScreen *screen, IUShort xStart, IUShort yStart,
    IUShort dir, IUByte shft, TrollCharacter *trll);
  void react();
  void takeHit(TrollThing *hitBy);

 protected:
  TrollCharacter *troll;
  IUShort trollX, trollY;
  IUShort trollFacing, trollFrame;
};

class TrollLimitedItem: public TrollCarriedItem
{
 public:
  TrollLimitedItem(IUShort num, IUShort slt, IUShort str);
  void draw(IScreen drawscreen, IUShort x, IUShort y);
  IUShort getNumber();
  void load(BinaryReadFile &f);
  void save(BinaryWriteFile &f);
  void setNumber(IUShort num);

 private:
  IUShort number;
};

class TrollLimitedActivatibleItem: public TrollActivatibleItem
{
 public:
  static TrollThing *createKey(TrollScreen *scr, IUShort x,
    IUShort y, IUShort secrt = 0);
  static TrollThing *createSoldKey(TrollScreen *scr, IUShort x,
    IUShort y, IUShort secrt = 0);

  void pickUp(TrollCharacter *troll);

 protected:
  TrollLimitedActivatibleItem(TrollScreen *scr, IUShort xStart,
    IUShort yStart, IUShort secrt, IUShort cst, TrollCarriedItem *actItem);
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
  TrollKeyProjectile(TrollScreen *screen, IUShort xStart, IUShort yStart,
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
  TrollTimedItem(TrollScreen *scr, IUShort xStart, IUShort yStart,
    IUShort secrt = 0, IUShort cst = 0);

  IUShort life;
};

class TrollGold: public TrollTimedItem
{
 public:
  static TrollThing *createGoldCoin(TrollScreen *scr, IUShort x,
    IUShort y, IUShort secrt = 0);
  static TrollThing *create3GoldCoins(TrollScreen *scr, IUShort x,
    IUShort y, IUShort secrt = 0);

  void pickUp(TrollCharacter *troll);

 protected:
  TrollGold(TrollScreen *scr, IUShort xStart, IUShort yStart,
    IUShort secrt, IUShort amt);

  IUShort amount;
};

class TrollHitPoint: public TrollTimedItem
{
 public:
  static TrollThing *createHitPoint(TrollScreen *scr, IUShort x,
    IUShort y, IUShort secrt = 0);
  static TrollThing *createSoldHitPoint(TrollScreen *scr, IUShort x,
    IUShort y, IUShort secrt = 0);

  void pickUp(TrollCharacter *troll);
  void react();

 protected:
  TrollHitPoint(TrollScreen *scr, IUShort xStart, IUShort yStart,
    IUShort secrt, IUShort cst, IUShort amt);

  IUShort amount;
  IUShort time;
};

class TrollLevelMap: public TrollItem
{
 public:
  static TrollThing *createLevelMap(TrollScreen *scr, IUShort x,
    IUShort y, IUShort secrt = 0);

  void pickUp(TrollCharacter *troll);

 protected:
  TrollLevelMap(TrollScreen *scr, IUShort xStart, IUShort yStart,
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

#endif

