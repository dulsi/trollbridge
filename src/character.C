/*-------------------------------------------------------------------------*\
  <character.C> -- Implementation of TrollCharacter methods

  Date      Programmer  Description
  04/18/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include <itimer.h>
#include <igrgeometry.h>
#include <igrtext.h>
#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::TrollCharacter - Constructor for the character.

    Parameters:
      gm           (In)  Game world the player is in
      nm           (In)  Name of the character
      startFile    (In)  Starting map file name
      ctrl         (In)  Nintendo controller
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollCharacter::TrollCharacter(TrollGame *gm, char *nm, const char *startFile,
  nes_controller *ctrl)
  :TrollThing((TrollScreen *)0)
{
 int i;
 char *filename;

 // Initialize all attributes
 game = gm;
 name = strdup(nm);
 for (i = 0; i < 3; i++)
 {
  screens[i] = IScreenCreate();
 }
 control = ctrl;

 // check for save file
 const char *savePath = game->getSavePath();
 filename = (char *)IMalloc(strlen(savePath) + strlen(name) + 5);
 strcpy(filename, savePath);
 strcat(filename, name);
 strcat(filename, ".trs");
 FILE *f = fopen(filename, "rb");
 IFree(filename);
 if (f)
 {
  fclose(f);
  load();
 }
 else
 {
  // new character
  thp = 6;
  shift = 0;
  gold = 0;
  bButton = 0;
  for (i = 0; i < TROLL_SECRETS_SIZE; i++)
  {
   secrets[i] = 0;
  }
  for (i = 0; i < TROLL_CHARACTER_CARRIED_ITEMS; i++)
  {
   items[i] = (TrollCarriedItem *)0;
  }
  save();
 }

 // Load the level and find the appropriate screen
 game->loadLevel(startFile);
 reset();
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::~TrollCharacter - Destructor for the character.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollCharacter::~TrollCharacter()
{
 int i;

 screen->removeCharacter(this);
 for (i = 0; i < 3; i++)
 {
  IScreenDestroy(screens[i]);
 }
 for (TrollMapIterator itr = mapInfo.begin(); itr != mapInfo.end(); itr++)
 {
  delete [] (*itr).second;
 }
 free(name);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::addCarriedItem - Gives the character an item that can be
    used with the A or B buttons.

    Parameters:
      item         (In)  New item to carry
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::addCarriedItem(TrollCarriedItem *item)
{
 IUShort num = item->getSlot();
 if (items[num])
 {
  if (items[num]->getStrength() > item->getStrength())
  {
   // Don't pickup items weaker than you already have
   return ;
  }
 }
 items[num] = item;
 if ((num > 0) && (bButton == 0))
 {
  bButton = num;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::addHp - Adds hit points to the character.

    Parameters:
      num          (In)  Number to add
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::addHp(IUShort num)
{
 hp += num;
 if (hp > thp)
 {
  hp = thp;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::addTotalHp - Adds to character's total hit points.

    Parameters:
      num          (In)  Number to add
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::addTotalHp(IUShort num)
{
 thp += num;
 hp += num;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::die - Called when the player dies.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::die()
{
 int where;
 signed char d, oldD;
 unsigned char start;

 // Most of the data from the nes controller is ignored.
 signed char ignore_1;
 unsigned char ignore_2;

 // Troll sprite
 const Sprite *trollPic = TrollSpriteHandler.getSprite(sprite);

 display();
 ITimerWait();
 where = 0;
 IScreenClear(screens[TROLL_TEMPORARY_SCREEN2]);
 ITextDraw(screens[TROLL_TEMPORARY_SCREEN2], 65, 54, 255,
   "Continue");
 ITextDraw(screens[TROLL_TEMPORARY_SCREEN2], 65, 70, 255,
   "Save");
 ITextDraw(screens[TROLL_TEMPORARY_SCREEN2], 65, 86, 255,
   "Retry");
 IScreenCopy(screens[TROLL_TEMPORARY_SCREEN1],
   screens[TROLL_TEMPORARY_SCREEN2]);
 trollPic->draw(screens[TROLL_TEMPORARY_SCREEN1], 40, where * 16 + 50, 0,
   TROLL_DOWN, shift);
 IScreenCopy(IScreenMain, screens[TROLL_TEMPORARY_SCREEN1]);
 IGraphicsRefresh();
 for (start = 0, oldD = 0; !start; oldD = d,
  control->status(d, ignore_1, ignore_2, ignore_2, start, ignore_2))
 {
  if ((d) && (d != oldD))
  {
   if ((d == -1) && (where != 0))
   {
    where--;
   }
   else if ((d == 1) && (where != 2))
   {
    where++;
   }
   IScreenCopy(screens[TROLL_TEMPORARY_SCREEN1],
     screens[TROLL_TEMPORARY_SCREEN2]);
   trollPic->draw(screens[TROLL_TEMPORARY_SCREEN1], 40, where * 16 + 50, 0,
     TROLL_DOWN, shift);
   IScreenCopy(IScreenMain, screens[TROLL_TEMPORARY_SCREEN1]);
   IGraphicsRefresh();
  }
  ITimerWait();
 }
 if (where == 0)
 {
  reset();
 }
 else if (where == 1)
 {
  save();
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::display - Shows the player what is going on.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::display()
{
 char s[20];
 IUShort i, k;

 // copy background screen to the temporary screen
 IScreenCopy(screens[TROLL_TEMPORARY_SCREEN1],
   screens[TROLL_BACKGROUND_SCREEN]);

 // display character status info
 sprintf(s, "  Hp: %-2d / %-2d", hp, thp);
 ITextDraw(screens[TROLL_TEMPORARY_SCREEN1], 208, 10, 255, s);
 sprintf(s, "Gold: %-3d", gold);
 ITextDraw(screens[TROLL_TEMPORARY_SCREEN1], 208, 23, 255, s);
 IRectangleDraw(screens[TROLL_TEMPORARY_SCREEN1], 130, 9, 154, 35, TROLL_LIGHTGRAY);
 ITextDraw(screens[TROLL_TEMPORARY_SCREEN1], 138, 5, 255,
   "B");
 IRectangleDraw(screens[TROLL_TEMPORARY_SCREEN1], 160, 9, 184, 35, TROLL_LIGHTGRAY);
 ITextDraw(screens[TROLL_TEMPORARY_SCREEN1], 168, 5, 255,
   "A");
 if (bButton && items[bButton])
 {
  items[bButton]->draw(screens[TROLL_TEMPORARY_SCREEN1], 142, 22);
 }
 if (items[0])
 {
  items[0]->draw(screens[TROLL_TEMPORARY_SCREEN1], 172, 22);
 }
 IUByte *curMapInfo = mapInfo[game->getLevelName()];
 for (i = 0; i < TROLL_LEVEL_X; i++)
 {
  for (k = 0; k < TROLL_LEVEL_Y; k++)
  {
   IBoxDraw(screens[TROLL_TEMPORARY_SCREEN1], 5 + 4 * i, 4 + 4 * k,
     8 + 4 * i, 7 + 4 * k, curMapInfo[i * TROLL_LEVEL_Y + k]);
  }
 }
/* IBoxDraw(screens[TROLL_TEMPORARY_SCREEN1], 5, 4, 4 + 4 * TROLL_LEVEL_X,
   3 + 4 * TROLL_LEVEL_Y, TROLL_LIGHTGRAY);*/
 IBoxDraw(screens[TROLL_TEMPORARY_SCREEN1], 5 + 4 * xScreen, 4 + 4 * yScreen,
   8 + 4 * xScreen, 7 + 4 * yScreen, 255);

 // draw all things onto the screen
 screen->drawThings(screens[TROLL_TEMPORARY_SCREEN1]);

 // copy the temporary screen to the physical display
 IScreenCopy(IScreenMain, screens[TROLL_TEMPORARY_SCREEN1]);
 IGraphicsRefresh();
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::draw - Draws the character to a screen.

    Parameters:
      drawscreen   (In & Out) Screen to draw the character to
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::draw(IScreen drawscreen)
{
 // Caused invincible characters to flash
 if (!(invincible & 2))
 {
  TrollThing::draw(drawscreen);
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::dropCarriedItem - Drops an item carried by the
    character.

    Parameters:
      num          (In)  Carried item slot number
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::dropCarriedItem(IUShort num)
{
 items[num] = (TrollCarriedItem *)0;
 if (bButton == num)
 {
  selectNextB();
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::getCarriedItem - Gets an item carried by the character.

    Parameters:
      num          (In)  Carried item slot number

    Returns: The carried item
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollCarriedItem *TrollCharacter::getCarriedItem(IUShort num)
{
 return(items[num]);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::getFacing - Gets the character's facing position.

    Returns: The character's facing position
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IUShort TrollCharacter::getFacing() const
{
 return(facing);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::getFrame - Gets the character's frame position.

    Returns: The character's frame position
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IUShort TrollCharacter::getFrame() const
{
 return(frame);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::getGold - Gets the character's gold.

    Returns: The character's gold
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IUShort TrollCharacter::getGold()
{
 return(gold);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::getHp - Gets the character's hit points.

    Returns: The character's hit points
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IUShort TrollCharacter::getHp()
{
 return(hp);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::getScreen - Gets the screen the character is on.

    Returns: The screen the character is on
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollScreen *TrollCharacter::getScreen()
{
 return(screen);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::getTotalHp - Gets the character's total hit points.

    Returns: The character's total hit points
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IUShort TrollCharacter::getTotalHp()
{
 return(thp);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::getX - Gets the character's x position.

    Returns: The character's x position
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IShort TrollCharacter::getX() const
{
 return(x);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::getY - Gets the character's y position.

    Returns: The character's y position (includes y buffer zone)
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IShort TrollCharacter::getY() const
{
 return(y);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::isInvincible - Returns whether or not the character is
    invincible.

    Returns: Whether the character is invincible or not
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
bool TrollCharacter::isInvincible()
{
 return(invincible);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::isSecretSet - Returns whether or not a secret is set.

    Parameters:
      num          (In)  Number of the secret to check

    Returns: Whether the secret is set or not
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
bool TrollCharacter::isSecretSet(IUShort num)
{
 if ((num >> 3) < TROLL_SECRETS_SIZE)
 {
  return(secrets[(num >> 3)] & (1 << (num & 7)));
 }
 else
 {
  return(0);
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::load - Loads a character from disk.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::load()
{
 char *filename;
 int i;
 IUShort num, num2;
 IUShort itemNum;
 TrollCarriedItem *item;
 IUByte *mapInfoNew;
 const char *savePath = game->getSavePath();

 // This should be dynamically allocated based on need.  As is it will crash
 // if level names are longer than 49 characters.
 char levelName[50];

 filename = (char *)IMalloc(strlen(savePath) + strlen(name) +
   strlen(TROLL_SAVE_EXT) + 1);
 strcpy(filename, savePath);
 strcat(filename, name);
 strcat(filename, TROLL_SAVE_EXT);

 BinaryReadFile f(filename);

 IFree(filename);
 f.readShort(thp);
 f.readUByte(shift);
 f.readUShort(gold);
 f.readUByteArray(TROLL_SECRETS_SIZE, secrets);
 f.readUShort(num);
 for (i = 0; i < TROLL_CHARACTER_CARRIED_ITEMS; i++)
 {
  items[i] = (TrollCarriedItem *)0;
 }
 for (i = 0; i < num; i++)
 {
  f.readUShort(itemNum);
  item = game->getCarriedItem(itemNum);
  items[item->getSlot()] = item;
  item->read(f);
 }
 f.readUShort(num);
 for (i = 0; i < num; i++)
 {
  for (f.readByte((signed char &)levelName[num2 = 0]); levelName[num2];
    f.readByte((signed char &)levelName[++num2]))
  {
  }
  mapInfoNew = new IUByte[TROLL_LEVEL_X * TROLL_LEVEL_Y];
  f.readUByteArray(TROLL_LEVEL_X * TROLL_LEVEL_Y, mapInfoNew);
  mapInfo[levelName] = mapInfoNew;
 }
 bButton = 1;
 selectNextB();
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::push - Allows the character to perform an action.

    Parameters:
      dir          (In)  Direction to push character
      time         (In)  Number of turns to push character
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::push(IUShort dir, IUShort time)
{
 switch (dir)
 {
  case TROLL_UP:
   facing = direction = TROLL_DOWN;
   break;
  case TROLL_DOWN:
   facing = direction = TROLL_UP;
   break;
  case TROLL_RIGHT:
   facing = direction = TROLL_LEFT;
   break;
  case TROLL_LEFT:
   facing = direction = TROLL_RIGHT;
   break;
 }
 flyBack += time;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::react - Allows the character to perform an action.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::react()
{
 signed char d, r;
 unsigned char a, b, st, sl;

 control->status(d, r, a, b, st, sl);
 // decrease invincibility timer
 if (invincible)
 {
  invincible--;
 }
 // a paused charcater performs no action
 if (pause)
 {
  pause--;
 }
 // when the character is hit he flies back without the player's control
 else if (flyBack)
 {
  IUShort oldFacing = facing;

  switch (facing)
  {
   case TROLL_UP:
    walk(TROLL_DOWN);
    break;
   case TROLL_DOWN:
    walk(TROLL_UP);
    break;
   case TROLL_LEFT:
    walk(TROLL_RIGHT);
    break;
   case TROLL_RIGHT:
    walk(TROLL_LEFT);
    break;
   default:
    break;
  }
  facing = oldFacing;
  flyBack--;
 }
 else
 {
  // otherwise move the character according to the player's wishes
  if (d == -1)
  {
   walk(TROLL_UP);
  }
  else if (d == 1)
  {
   walk(TROLL_DOWN);
  }
  else if (r == 1)
  {
   walk(TROLL_RIGHT);
  }
  else if (r == -1)
  {
   walk(TROLL_LEFT);
  }
  direction = facing;
  if (a  && items[0])
  {
   frame = 1;
   items[0]->activate(this, screen);
  }
  else if (b && bButton && items[bButton])
  {
   frame = 0;
   items[bButton]->activate(this, screen);
  }
  if (sl)
  {
   selectNextB();
  }
 }
 // check if the player is on an exit
 const TrollExit *screenExit = screen->getExit(x, y);
 if (screenExit)
 {
  game->loadLevel(screenExit->getLevelName());
  screen->removeCharacter(this);
  xScreen = screenExit->getNewScreenX();
  yScreen = screenExit->getNewScreenY();
  screen = game->getScreen(xScreen, yScreen);
  x = screenExit->getNewX();
  y = screenExit->getNewY();
  screen->addCharacter(this);
  setMapInfo(game->getLevelName(), xScreen, yScreen,
    game->getMapInfo(xScreen, yScreen));
  setupBackgroundScreen();
 }
 // exit the game if start is pressed
 if (st) dead = 1;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::save - Saves a character to disk.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::save()
{
 char *filename;
 int i;
 IUShort num;
 const char *savePath = game->getSavePath();

 filename = (char *)IMalloc(strlen(savePath) + strlen(name) +
   strlen(TROLL_SAVE_EXT) + 1);
 strcpy(filename, savePath);
 strcat(filename, name);
 strcat(filename, TROLL_SAVE_EXT);

 BinaryWriteFile f(filename);

 IFree(filename);
 f.writeShort(thp);
 f.writeUByte(shift);
 f.writeUShort(gold);
 f.writeUByteArray(TROLL_SECRETS_SIZE, secrets);
 for (i = 0, num = 0; i < TROLL_CHARACTER_CARRIED_ITEMS; i++)
 {
  if (items[i])
  {
   num++;
  }
 }
 f.writeUShort(num);
 for (i = 0; i < TROLL_CHARACTER_CARRIED_ITEMS; i++)
 {
  if (items[i])
  {
   f.writeUShort((items[i])->getItemNum());
   (items[i])->write(f);
  }
 }
 f.writeUShort(mapInfo.size());
 for (TrollMapIterator itr = mapInfo.begin(); itr != mapInfo.end(); itr++)
 {
  const char *levelName = (*itr).first.c_str();
  for (f.writeByte((signed char)levelName[num = 0]); levelName[num];
    f.writeByte((signed char)levelName[++num]))
  {
  }
  f.writeUByteArray(TROLL_LEVEL_X * TROLL_LEVEL_Y, (*itr).second);
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::setBackground - sets the sprite and color shift for a
    background square.

    Parameters:
      x            (In)  X location of square (not display screen location)
      y            (In)  Y location of square (not display screen location)
      sprt         (In)  Sprite number
      shft         (In)  Color shift
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::setBackground(IUShort x, IUShort y, IUShort sprt,
  IUByte shft)
{
 const Sprite *pic = TrollSpriteHandler.getSprite(sprt);
 pic->draw(screens[TROLL_BACKGROUND_SCREEN], TROLL_CALCULATE_X_POS(x),
   TROLL_CALCULATE_Y_POS(y), 0, 0, shft);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::setGold - Sets the gold of the character.

    Parameters:
      num          (In)  Gold
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::setGold(IUShort num)
{
 gold = num;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::setPause - Sets the pause counter of the character.

    Parameters:
      num          (In)  Time to pause
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::setPause(IUShort num)
{
 pause = num;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::setMapInfo - Sets the map info of the character.

    Parameters:
      levelName    (In)  Name of the level
      xMap         (In)  X position of map
      yMap         (In)  Y position of map
      val          (In)  Value of map info
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::setMapInfo(const char *levelName, IUShort xMap,
  IUShort yMap, IUByte val)
{
 if (!mapInfo[levelName])
 {
  IUShort i;
  IUByte *mapInfoNew = new IUByte[TROLL_LEVEL_X * TROLL_LEVEL_Y];

  for (i = 0; i < TROLL_LEVEL_X * TROLL_LEVEL_Y; i++)
  {
   mapInfoNew[i] = TROLL_DARKGRAY;
  }
  mapInfo[levelName] = mapInfoNew;
 }
 (mapInfo[levelName])[xMap * TROLL_LEVEL_Y + yMap] = val;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::setSecret - Sets a secret.

    Parameters:
      num          (In)  Number of the secret to set
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::setSecret(IUShort num)
{
 if ((num >> 3) < TROLL_SECRETS_SIZE)
 {
  secrets[(num >> 3)] |= (1 << (num & 7));
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::setText - sets the text to display on the screen.

    Parameters:
      msg1         (In)  First line of message
      msg2         (In)  Second line of message
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::setText(char *msg1, char *msg2)
{
 IUShort x, y, sprt;
 IUByte shft;

 for (y = 1; y < 3; y++)
 {
  for (x = 0; x < TROLL_SCREEN_X; x++)
  {
   screen->getBackground(x, y, sprt, shft);
   const Sprite *pic = TrollSpriteHandler.getSprite(sprt);
   pic->draw(screens[TROLL_BACKGROUND_SCREEN], TROLL_CALCULATE_X_POS(x),
     TROLL_CALCULATE_Y_POS(y), 0, 0, shft);
  }
 }
 if (msg1)
 {
  ITextDraw(screens[TROLL_BACKGROUND_SCREEN], 150 - strlen(msg1) * 4,
    TROLL_BUFFER_Y + TROLL_SQUARE_Y + 4, 255, msg1);
 }
 if (msg2)
 {
  ITextDraw(screens[TROLL_BACKGROUND_SCREEN], 150 - strlen(msg2) * 4,
    TROLL_BUFFER_Y + TROLL_SQUARE_Y * 2 + 4, 255, msg2);
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::takeHit - Called when the character collides with another
    object in the game.

    Parameters:
      hitBy        (In)  The object the character hit
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::takeHit(TrollThing *hitBy)
{
 TrollMonster *mon;
 TrollProjectile *proj;
 TrollItem *item;

 // Check if the character is dead
 if (isDead())
 {
  return ;
 }
 // Check if it is an item
 if ((item = (dynamic_cast<TrollItem *>(hitBy))))
 {
  item->pickUp(this);
 }
 // Don't take damage if the character is invincible
 else if (invincible)
 {
  return ;
 }
 // Check if it is a monster
 else if ((mon = (dynamic_cast<TrollMonster *>(hitBy))))
 {
  if (mon->getDamage())
  {
   hp -= mon->getDamage();
   invincible = TROLL_INVINCIBLE_TIME;
   flyBack = TROLL_FLYBACK_TIME;
  }
 }
 // Check if it is a projectile
 else if ((proj = (dynamic_cast<TrollProjectile *>(hitBy))))
 {
  if (proj->getDamage())
  {
   hp -= proj->getDamage();
   invincible = TROLL_INVINCIBLE_TIME;
   flyBack = TROLL_FLYBACK_TIME;
  }
 }
 // Kill the player if his hit points are too low
 if (hp < 1)
 {
  dead = 1;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::reset - Sets the character to starting stats
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::reset()
{
 flyBack = invincible = pause = 0;
 sprite = TROLL_SPRITE;
 facing = 0;
 frame = 0;
 hp = 6;
 dead = 0;

 xScreen = game->getXScreen();
 yScreen = game->getYScreen();
 x = game->getXStart();
 y = game->getYStart();

 if (screen)
 {
  screen->removeCharacter(this);
 }
 screen = game->getScreen(xScreen, yScreen);
 screen->addCharacter(this);
 setMapInfo(game->getLevelName(), xScreen, yScreen,
   game->getMapInfo(xScreen, yScreen));
 setupBackgroundScreen();
 display();
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::scrollUp - Scrolls the screen up one.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::scrollUp()
{
 IScreen swapScreen;
 int i;

 // Change the background screen with the second temporary screen
 swapScreen = screens[TROLL_TEMPORARY_SCREEN2];
 screens[TROLL_TEMPORARY_SCREEN2] = screens[TROLL_BACKGROUND_SCREEN];
 screens[TROLL_BACKGROUND_SCREEN] = swapScreen;

 // Get the new screen and draw the backgrounds
 screen->removeCharacter(this);
 yScreen--;
 screen = game->getScreen(xScreen, yScreen);
 screen->addCharacter(this);
 setMapInfo(game->getLevelName(), xScreen, yScreen,
   game->getMapInfo(xScreen, yScreen));
 setupBackgroundScreen();

 // Scroll over to the new screen
 for (i = 0; i < TROLL_SCREEN_Y * 2; i++)
 {
  // Note: This code make certain assumptions about the implementation of
  // IScreen.  This is valid on current supported platforms.

  // Copy old screen info
  IMemcpy(
    screens[TROLL_TEMPORARY_SCREEN1] +
      (TROLL_BUFFER_Y + i * TROLL_SQUARE_Y / 2) * 320,
    screens[TROLL_TEMPORARY_SCREEN2] + TROLL_BUFFER_Y * 320,
    64000 - (TROLL_BUFFER_Y + i * TROLL_SQUARE_Y / 2) * 320);

  // Copy new screen info
  IMemcpy(
    screens[TROLL_TEMPORARY_SCREEN1] + TROLL_BUFFER_Y * 320,
    screens[TROLL_BACKGROUND_SCREEN] + 64000 - (i * TROLL_SQUARE_Y / 2) * 320,
    (i * TROLL_SQUARE_Y / 2) * 320);

  // Draw the troll
  draw(screens[TROLL_TEMPORARY_SCREEN1]);

  // Display everything on the real screen
  IScreenCopy(IScreenMain, screens[TROLL_TEMPORARY_SCREEN1]);
  IGraphicsRefresh();
  ITimerWait();

  // Switch frame and change y position
  frame = 1 - frame;
  y += TROLL_SQUARE_Y / 2;
  if (y + TrollSpriteHandler.getSprite(sprite)->getYSize() > 200)
  {
   y = 200 - TrollSpriteHandler.getSprite(sprite)->getYSize();
  }
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::scrollDown - Scrolls the screen down one.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::scrollDown()
{
 IScreen swapScreen;
 int i;

 // Change the background screen with the second temporary screen
 swapScreen = screens[TROLL_TEMPORARY_SCREEN2];
 screens[TROLL_TEMPORARY_SCREEN2] = screens[TROLL_BACKGROUND_SCREEN];
 screens[TROLL_BACKGROUND_SCREEN] = swapScreen;

 // Get the new screen and draw the backgrounds
 screen->removeCharacter(this);
 yScreen++;
 screen = game->getScreen(xScreen, yScreen);
 screen->addCharacter(this);
 setMapInfo(game->getLevelName(), xScreen, yScreen,
   game->getMapInfo(xScreen, yScreen));
 setupBackgroundScreen();

 // Scroll over to the new screen
 for (i = 0; i < TROLL_SCREEN_Y * 2; i++)
 {
  // Note: This code make certain assumptions about the implementation of
  // IScreen.  This is valid on current supported platforms.

  // Copy old screen info
  IMemcpy(
    screens[TROLL_TEMPORARY_SCREEN1] + TROLL_BUFFER_Y * 320,
    screens[TROLL_TEMPORARY_SCREEN2] +
      (TROLL_BUFFER_Y + i * TROLL_SQUARE_Y / 2) * 320,
    64000 - (TROLL_BUFFER_Y + i * TROLL_SQUARE_Y / 2) * 320);

  // Copy new screen info
  IMemcpy(
    screens[TROLL_TEMPORARY_SCREEN1] + 
      64000 - (i * TROLL_SQUARE_Y / 2) * 320,
    screens[TROLL_BACKGROUND_SCREEN] + TROLL_BUFFER_Y * 320,
    (i * TROLL_SQUARE_Y / 2) * 320);

  // Draw the troll
  draw(screens[TROLL_TEMPORARY_SCREEN1]);

  // Display everything on the real screen
  IScreenCopy(IScreenMain, screens[TROLL_TEMPORARY_SCREEN1]);
  IGraphicsRefresh();
  ITimerWait();

  // Switch frame and change y position
  frame = 1 - frame;
  y -= TROLL_SQUARE_Y / 2;
  if (y < TROLL_BUFFER_Y)
  {
   y = TROLL_BUFFER_Y;
  }
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::scrollRight - Scrolls the screen right one.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::scrollRight()
{
 IScreen swapScreen;
 int i;
 int yRegion;

 // Change the background screen with the second temporary screen
 swapScreen = screens[TROLL_TEMPORARY_SCREEN2];
 screens[TROLL_TEMPORARY_SCREEN2] = screens[TROLL_BACKGROUND_SCREEN];
 screens[TROLL_BACKGROUND_SCREEN] = swapScreen;

 // Get the new screen and draw the backgrounds
 screen->removeCharacter(this);
 xScreen++;
 screen = game->getScreen(xScreen, yScreen);
 setMapInfo(game->getLevelName(), xScreen, yScreen,
   game->getMapInfo(xScreen, yScreen));
 screen->addCharacter(this);
 setupBackgroundScreen();

 // Scroll over to the new screen
 for (i = 0; i < TROLL_SCREEN_X * 2; i++)
 {
  // Note: This code make certain assumptions about the implementation of
  // IScreen.  This is valid on current supported platforms.

  // Unlike up and down, right scrolling must copy x regions seperately
  for (yRegion = TROLL_BUFFER_Y; yRegion < 200; yRegion++)
  {
   // Copy old screen info
   IMemcpy(
     screens[TROLL_TEMPORARY_SCREEN1] + yRegion * 320,
     screens[TROLL_TEMPORARY_SCREEN2] + yRegion * 320 +
       i * TROLL_SQUARE_X / 2,
     320 - i * TROLL_SQUARE_X / 2);

   // Copy new screen info
   IMemcpy(
     screens[TROLL_TEMPORARY_SCREEN1] + yRegion * 320 +
       320 - i * TROLL_SQUARE_X / 2,
     screens[TROLL_BACKGROUND_SCREEN] + yRegion * 320,
     i * TROLL_SQUARE_X / 2);
  }

  // Draw the troll
  draw(screens[TROLL_TEMPORARY_SCREEN1]);

  // Display everything on the real screen
  IScreenCopy(IScreenMain, screens[TROLL_TEMPORARY_SCREEN1]);
  IGraphicsRefresh();
  ITimerWait();

  // Switch frame and change x position
  frame = 1 - frame;
  if (x < TROLL_SQUARE_X / 2)
  {
   x = 0;
  }
  else
  {
   x -= TROLL_SQUARE_X / 2;
  }
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::scrollLeft - Scrolls the screen left one.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::scrollLeft()
{
 IScreen swapScreen;
 int i;
 int yRegion;


 // Change the background screen with the second temporary screen
 swapScreen = screens[TROLL_TEMPORARY_SCREEN2];
 screens[TROLL_TEMPORARY_SCREEN2] = screens[TROLL_BACKGROUND_SCREEN];
 screens[TROLL_BACKGROUND_SCREEN] = swapScreen;

 // Get the new screen and draw the backgrounds
 screen->removeCharacter(this);
 xScreen--;
 screen = game->getScreen(xScreen, yScreen);
 screen->addCharacter(this);
 setMapInfo(game->getLevelName(), xScreen, yScreen,
   game->getMapInfo(xScreen, yScreen));
 setupBackgroundScreen();

 // Scroll over to the new screen
 for (i = 0; i < TROLL_SCREEN_X * 2; i++)
 {
  // Note: This code make certain assumptions about the implementation of
  // IScreen.  This is valid on current supported platforms.

  // Unlike up and down, left scrolling must copy x regions seperately
  for (yRegion = TROLL_BUFFER_Y; yRegion < 200; yRegion++)
  {
   // Copy old screen info
   IMemcpy(
     screens[TROLL_TEMPORARY_SCREEN1] + yRegion * 320 +
       i * TROLL_SQUARE_X / 2,
     screens[TROLL_TEMPORARY_SCREEN2] + yRegion * 320,
     320 - i * TROLL_SQUARE_X / 2);

   // Copy new screen info
   IMemcpy(
     screens[TROLL_TEMPORARY_SCREEN1] + yRegion * 320,
     screens[TROLL_BACKGROUND_SCREEN] + yRegion * 320 +
       320 - i * TROLL_SQUARE_X / 2,
     i * TROLL_SQUARE_X / 2);
  }

  // Draw the troll
  draw(screens[TROLL_TEMPORARY_SCREEN1]);

  // Display everything on the real screen
  IScreenCopy(IScreenMain, screens[TROLL_TEMPORARY_SCREEN1]);
  IGraphicsRefresh();
  ITimerWait();

  // Switch frame and change x position
  frame = 1 - frame;
  x += TROLL_SQUARE_X / 2;
  if (x + TrollSpriteHandler.getSprite(sprite)->getXSize() > 320)
  {
   x = 320 - TrollSpriteHandler.getSprite(sprite)->getXSize();
  }
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::selectNextB - Switches the item activated by B to the next
    one.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::selectNextB()
{
 IUShort curBButton = bButton;

 if (bButton)
 {
  do
  {
   bButton = (bButton + 1) % TROLL_CHARACTER_CARRIED_ITEMS;
   if (bButton == 0)
   {
    bButton++;
   }
  } while ((!items[bButton]) && (curBButton != bButton));
  if (!items[bButton])
  {
   bButton = 0;
  }
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::setupBackgroundScreen - Loads all the images of the
    backgound onto one of the virtual screens.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::setupBackgroundScreen()
{
 IUShort x, y, sprt;
 IUByte shft;
 char *msg1;
 char *msg2;

 IScreenClear(screens[TROLL_BACKGROUND_SCREEN]);
 for (y = 0; y < TROLL_SCREEN_Y; y++)
 {
  for (x = 0; x < TROLL_SCREEN_X; x++)
  {
   screen->getBackground(x, y, sprt, shft);
   const Sprite *pic = TrollSpriteHandler.getSprite(sprt);
   pic->draw(screens[TROLL_BACKGROUND_SCREEN], TROLL_CALCULATE_X_POS(x),
     TROLL_CALCULATE_Y_POS(y), 0, 0, shft);
  }
 }
 screen->getText(msg1, msg2);
 if (msg1)
 {
  ITextDraw(screens[TROLL_BACKGROUND_SCREEN], 150 - strlen(msg1) * 4,
    TROLL_BUFFER_Y + TROLL_SQUARE_Y + 4, 255, msg1);
 }
 if (msg2)
 {
  ITextDraw(screens[TROLL_BACKGROUND_SCREEN], 150 - strlen(msg2) * 4,
    TROLL_BUFFER_Y + TROLL_SQUARE_Y * 2 + 4, 255, msg2);
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollCharacter::walk - Causes the character to move one fourth of a square
    in a direction.

    Parameters:
      dir          (In)  Direction to move in (see constants in trollconst.h)
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollCharacter::walk(IUShort dir)
{
 IUShort xSquare = x / TROLL_SQUARE_X;
 IUShort ySquare = (y - TROLL_BUFFER_Y) / TROLL_SQUARE_Y;
 IUShort xOffset = x % TROLL_SQUARE_X;
 IUShort yOffset = (y - TROLL_BUFFER_Y) % TROLL_SQUARE_Y;

 if (((facing == TROLL_UP) || (facing == TROLL_DOWN)) &&
   ((dir == TROLL_RIGHT) || (dir == TROLL_LEFT)))
 {
  if (yOffset == TROLL_SQUARE_Y / 4) // up to nearest square
  {
   y -= TROLL_SQUARE_Y / 4;
   yOffset = 0;
  }
  else if (yOffset == 3 * TROLL_SQUARE_Y / 4) // down to nearest square
  {
   y += TROLL_SQUARE_Y / 4;
   ySquare++;
   yOffset = 0;
  }
 }
 if (((dir == TROLL_UP) || (dir == TROLL_DOWN)) &&
   ((facing == TROLL_RIGHT) || (facing == TROLL_LEFT)))
 {
  if (xOffset == TROLL_SQUARE_X / 4) // left to nearest square
  {
   x -= TROLL_SQUARE_X / 4;
   xOffset = 0;
  }
  else if (xOffset == 3 * TROLL_SQUARE_X / 4) // right to nearest square
  {
   x += TROLL_SQUARE_X / 4;
   xSquare++;
   xOffset = 0;
  }
 }
 facing = dir;
 frame = 1 - frame;
 switch (dir)
 {
  case TROLL_UP:
   if ((ySquare == 0) && (yOffset == 0))
   {
    scrollUp();
   }
   else
   {
    if ((yOffset != TROLL_SQUARE_Y / 2) ||
      ((screen->getPassability(xSquare, ySquare) <= TROLL_NOMONSTER) &&
        ((xOffset == 0) ||
          (screen->getPassability(xSquare + 1, ySquare) <= TROLL_NOMONSTER))))
    {
     y -= TROLL_SQUARE_Y / 4;
    }
   }
   break;
  case TROLL_DOWN:
   if ((ySquare == TROLL_SCREEN_Y - 1) && (yOffset == 0))
   {
    scrollDown();
   }
   else
   {
    if ((yOffset != 0) ||
      ((screen->getPassability(xSquare, ySquare + 1) <= TROLL_NOMONSTER) &&
        ((xOffset == 0) ||
          (screen->getPassability(xSquare + 1, ySquare + 1) <= TROLL_NOMONSTER))))
    {
      y += TROLL_SQUARE_Y / 4;
    }
   }
   break;
  case TROLL_RIGHT:
   if ((xSquare == TROLL_SCREEN_X - 1) && (xOffset == 0))
   {
    scrollRight();
   }
   else
   {
    if ((xOffset != 0) ||
      ((yOffset == 0) &&
        (screen->getPassability(xSquare + 1, ySquare) <= TROLL_NOMONSTER)) ||
      ((yOffset == TROLL_SQUARE_Y / 4) &&
        (screen->getPassability(xSquare + 1, ySquare) <= TROLL_NOMONSTER) &&
        (screen->getPassability(xSquare + 1, ySquare + 1) <= TROLL_NOMONSTER)) ||
      ((yOffset > TROLL_SQUARE_Y / 4) &&
        (screen->getPassability(xSquare + 1, ySquare + 1) <= TROLL_NOMONSTER)))
    {
     x += TROLL_SQUARE_X / 4;
    }
   }
   break;
  case TROLL_LEFT:
   if ((xSquare == 0) && (xOffset == 0))
   {
    scrollLeft();
   }
   else
   {
    if ((xOffset != 0) ||
      ((yOffset == 0) &&
        (screen->getPassability(xSquare - 1, ySquare) <= TROLL_NOMONSTER)) ||
      ((yOffset == TROLL_SQUARE_Y / 4) &&
        (screen->getPassability(xSquare - 1, ySquare) <= TROLL_NOMONSTER) &&
        (screen->getPassability(xSquare - 1, ySquare + 1) <= TROLL_NOMONSTER)) ||
      ((yOffset > TROLL_SQUARE_Y / 4) &&
        (screen->getPassability(xSquare - 1, ySquare + 1) <= TROLL_NOMONSTER)))
    {
     x -= TROLL_SQUARE_X / 4;
    }
   }
   break;
  default:
   // Should never get here
   break;
 }
}

