#ifndef __TROLL_H
#define __TROLL_H
/*-------------------------------------------------------------------------*\
  <troll.h> -- Header file for Troll Bridge

  Date      Programmer  Description
  04/17/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include <typeinfo> /* G++ produces warnings when ".h" is used */

// string produces errors when included latter for some reason.
#include <string>

#include <time.h>
#include <istdlib.h>
#include <igrbasics.h>
#include <igrimage.h>
#include "trollconst.h"
#include "sprite.h"
#include "nescont.h"

// Although only a singly linked list is required an STL's doubly linked list
// is used rather than spending the time to implement a custom class.
#include <list.h>
#define TrollThingList list<TrollThing *>
#define TrollThingIterator list<TrollThing *>::iterator

// Level maps are stored in STL maps.
#include <map.h>
#define TrollMap map<string, IUByte *, less<string> >
#define TrollMapIterator map<string, IUByte *, less<string> >::iterator

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  New Types
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TrollScreen;
class TrollExit;
class TrollSecret;
class TrollThing;
class TrollMonster;
class TrollItem;
class TrollProjectile;
class TrollCarriedItem;
class TrollCharacter;

typedef TrollThing *(*TrollMonsterConstructor)
  (TrollScreen *scr, IShort x, IShort y, IUShort secrt = 0);
typedef TrollThing *(*TrollItemConstructor)
  (TrollScreen *scr, IShort x, IShort y, IUShort secrt = 0);

class TrollDefinition
{
 public:
  TrollDefinition(const char *filename);
  ~TrollDefinition();
  const char *getDllFile();
  const char *getPaletteFile();
  const char *getSpriteFile();
  const char *getStartFile();
  const char *getTitleFile();

 private:
  char *dllFile;
  char *paletteFile;
  char *spriteFile;
  char *startFile;
  char *titleFile;
};

class TrollGame
{
 public:
  TrollGame(int argc, char **argv);
  ~TrollGame();
  void run();

  /* Used by DLLs only to add monster and item types */
  void addCarriedItem(TrollCarriedItem *item);
  void addItemType(IUShort num, TrollItemConstructor construct);
  void addMonsterType(IUShort num, TrollMonsterConstructor construct);

  TrollItemConstructor createItem(int itemNum);
  TrollMonsterConstructor createMonster(int monsterNum);
  TrollCarriedItem *getCarriedItem(IUShort num);
  const char *getLevelName();
  IUByte getMapInfo(IUShort x, IUShort y);
  const char *getSavePath();
  TrollScreen *getScreen(IUShort x, IUShort y);
  IUShort getXScreen();
  IUShort getXStart();
  IUShort getYScreen();
  IUShort getYStart();
  void loadLevel(const char *filename);

 protected:
  char *buildFullPath(const char *path, const char *file);
  void loadLibrary(const char *filename);
  void selectName(char *name);
  bool titleScreen(char *name);
  void turnCleanUp();

  TrollDefinition *definition;
  TrollMonsterConstructor monsterTypes[TROLL_MONSTER_TYPES];
  TrollItemConstructor itemTypes[TROLL_ITEM_TYPES];
  TrollCarriedItem *carriedItems[TROLL_CARRIED_ITEMS];
  TrollScreen *screen[TROLL_LEVEL_X][TROLL_LEVEL_Y];
  IUByte mapInfo[TROLL_LEVEL_X][TROLL_LEVEL_Y];
  TrollCharacter *troll;
  nes_controller *control;
  IImage titlePic;
  char *levelName;
  char *savePath;
  IUShort xScreen, yScreen;
  IUShort xStart, yStart;

  // Used for switching levels
  TrollScreen *extraScreen;
};

class TrollScreen
{
 public:
  TrollScreen(TrollGame *gm, BinaryReadFile &f);
  ~TrollScreen();
  void addCharacter(TrollCharacter *trll);
  void addCharacterProjectile(TrollThing *charProj);
  void addExit(TrollExit *exit);
  void addItem(TrollThing *item);
  void addMonster(TrollThing *mon);
  void addMonsterProjectile(TrollThing *monProj);
  void checkCollisionMonsterProjectile(TrollThing *a);
  void drawThings(IScreen scr);
  void getBackground(IUShort x, IUShort y, IUShort &sprt, IUByte &shft);
  const TrollCharacter *getCharacter();
  const TrollExit *getExit(IUShort x, IUShort y);
  TrollGame *getGame();
  IUShort getPassability(IUShort x, IUShort y);
  IUShort getStandardMonsterNum();
  void getText(char *&msg1, char *&msg2);
  void removeCharacter(TrollCharacter *trll);
  void setBackground(IUShort x, IUShort y, IUShort sprt, IUByte shft);
  void setPassability(IUShort x, IUShort y, IUShort pass);
  void setSecret(IUShort num);
  void setText(char *msg1, char *msg2);
  void update();

 protected:
  TrollGame *game;
  IUShort passability[TROLL_SCREEN_X][TROLL_SCREEN_Y];
  IUShort backgroundSprite[TROLL_SCREEN_X][TROLL_SCREEN_Y];
  IUByte backgroundShift[TROLL_SCREEN_X][TROLL_SCREEN_Y];

  char *message1;
  char *message2;

  IUShort exitNumPerm;
  IUShort exitNumTemp;
  TrollExit **exits;

  IUShort permMonsterNum;
  IUShort *permMonsterTypes;
  IUShort respawnMonsterNum;
  IUShort *respawnMonsterTypes;

  IUShort secretNum;
  TrollSecret **secrets;

  IUShort standardMonsterNum;
  IUShort respawnNum;
  time_t respawnTime;

  TrollThingList permMonsters;
  TrollThingList respawnMonsters;
  TrollThingList monsterProj;
  TrollThingList items;
  TrollThingList characterProj;

  TrollCharacter *troll;

 private:
  static IUShort orgPassability[TROLL_SCREEN_X][TROLL_SCREEN_Y];
  static IUShort orgBackgroundSprite[TROLL_SCREEN_X][TROLL_SCREEN_Y];
  static IUByte orgBackgroundShift[TROLL_SCREEN_X][TROLL_SCREEN_Y];
};

class TrollExit
{
 public:
  TrollExit(BinaryReadFile &f);
  TrollExit(IUShort ex, IUShort ey, const char *lvlNm = NULL,
    IUShort nsx = 0, IUShort nsy = 0, IUShort nx = 0, IUShort ny = 0);
  ~TrollExit();
  IUShort getX() const;
  IUShort getY() const;
  const char *getLevelName() const;
  IUShort getNewScreenX() const;
  IUShort getNewScreenY() const;
  IUShort getNewX() const;
  IUShort getNewY() const;
  void setX(IUShort val);
  void setY(IUShort val);
  void setLevelName(const char *val);
  void setNewScreenX(IUShort val);
  void setNewScreenY(IUShort val);
  void setNewX(IUShort val);
  void setNewY(IUShort val);
  void write(BinaryWriteFile &f);

 protected:
  IUShort x, y;
  char *levelName;
  IUShort newScreenX, newScreenY;
  IUShort newX, newY;
};

class TrollSecret
{
 public:
  TrollSecret(TrollGame *gm, TrollScreen *scr, BinaryReadFile &f);
  ~TrollSecret();
  void executeSet();
  void executeUnset();
  IUShort getNumber();

 protected:
  void execute(IUByte *script);

  TrollGame *game;
  TrollScreen *screen;
  IUShort number;
  IUByte *setScript;
  IUByte *unsetScript;
};

class TrollThing
{
 public:
  virtual ~TrollThing();
  virtual void die();
  virtual void draw(IScreen drawscreen);
  IUShort getDirection() const;
  void getLocation(IShort &xLoc, IShort &yLoc) const;
  bool isDead() const;
  virtual void react() = 0;
  void setDead(IUByte value);
  virtual void takeHit(TrollThing *hitby) = 0;

  static bool checkCollision(const TrollThing *a, const TrollThing *b);

 protected:
  TrollThing(TrollScreen *scr, IUShort secrt = 0);

  TrollScreen *screen;
  IUShort sprite;
  IUShort direction;
  IUShort facing, frame;
  IUByte shift; // color shift
  IShort x, y;
  IUByte dead;
  IUShort secret;
};

class TrollMonster: public TrollThing
{
 public:
  virtual IShort getDamage();
  void push(IShort xOffset, IShort yOffset);
  void setDirection(IUShort dir);
  void setFacing(IUShort face);
  void setFrame(IUShort frame);
  void setLocation(IShort xNew, IShort yNew);
  void setSprite(IUShort sprt, IUShort face);
  void takeHit(TrollThing *hitBy);

 protected:
  TrollMonster(TrollScreen *scr, IUShort secrt = 0);
  bool checkPassabilityFrom(IUShort xCur, IUShort yCur);
  void findOpen();
  void move();
};

class TrollStandardMonster: public TrollMonster
{
 public:
  void draw(IScreen drawscreen);
  IShort getDamage();
  void takeHit(TrollThing *hitBy);

 protected:
  TrollStandardMonster(TrollScreen *scr, IUShort secrt = 0, IShort dmg = 1);

  IShort hp;
  IShort damage;
  IUShort invincible;
  IUShort flyBack;
};

class TrollItem: public TrollThing
{
 public:
  bool checkPickUp(TrollCharacter *troll);
  void draw(IScreen drawscreen);
  virtual void pickUp(TrollCharacter *troll) = 0;
  void react();
  void takeHit(TrollThing *hitBy);

 protected:
  TrollItem(TrollScreen *scr, IUShort xStart, IUShort yStart,
    IUShort secrt = 0, IUShort cst = 0);

  IUShort cost;
};

class TrollProjectile: public TrollThing
{
 public:
  virtual IShort getDamage();

 protected:
  TrollProjectile(TrollScreen *scr, IUShort dmg = 1);

  IShort damage;
};

class TrollCarriedItem
{
 public:
  TrollCarriedItem(IUShort num, IUShort slt, IUShort str);
  virtual ~TrollCarriedItem();
  virtual void activate(TrollCharacter *troll, TrollScreen *screen) = 0;
  virtual void draw(IScreen drawscreen, IUShort x, IUShort y);
  IUShort getItemNum();
  IUShort getSlot();
  IUShort getStrength();
  IUShort getSprite();
  IUShort getShift();
  virtual void read(BinaryReadFile &f);
  virtual void write(BinaryWriteFile &f);

 protected:
  IUShort itemNum;
  IUShort slot;
  IUShort strength;
  IUShort sprite;
  IUShort shift;
};

class TrollCharacter: public TrollThing
{
 public:
  TrollCharacter(TrollGame *gm, char *nm, const char *startFile,
    nes_controller *ctrl);
  ~TrollCharacter();
  void addCarriedItem(TrollCarriedItem *item);
  void addHp(IUShort num);
  void addTotalHp(IUShort num);
  void die();
  void display();
  void draw(IScreen drawscreen);
  void dropCarriedItem(IUShort num);
  TrollCarriedItem *getCarriedItem(IUShort num);
  IUShort getFacing() const;
  IUShort getFrame() const;
  IUShort getGold();
  IUShort getHp();
  TrollScreen *getScreen();
  IUShort getTotalHp();
  IShort getX() const;
  IShort getY() const;
  bool isInvincible();
  bool isSecretSet(IUShort num);
  void load();
  void push(IUShort dir, IUShort time);
  void react();
  void save();
  void setBackground(IUShort x, IUShort y, IUShort sprt, IUByte shft);
  void setGold(IUShort num);
  void setPause(IUShort num);
  void setMapInfo(const char *levelName, IUShort x, IUShort y, IUByte val);
  void setSecret(IUShort num);
  void setText(char *message1, char *message2);
  void takeHit(TrollThing *hitby);

 protected:
  void reset();
  void scrollUp();
  void scrollDown();
  void scrollRight();
  void scrollLeft();
  void selectNextB();
  void setupBackgroundScreen();
  void walk(IUShort dir);

  TrollGame *game;
  char *name;
  nes_controller *control;
  IScreen screens[3];
  IShort hp, thp;
  IUShort xScreen, yScreen;
  IUShort gold;
  IUShort bButton;
  IUShort invincible;
  IUShort pause;
  IUShort flyBack;
  IUByte secrets[TROLL_SECRETS_SIZE];
  TrollCarriedItem *items[TROLL_CHARACTER_CARRIED_ITEMS];
  TrollMap mapInfo;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Global variables
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
extern SpriteHandler TrollSpriteHandler;

#endif

