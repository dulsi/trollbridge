/*-------------------------------------------------------------------------*\
  <screen.C> -- Implemention for TrollScreen methods

  Date      Programmer  Description
  04/17/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Globals
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IUShort TrollScreen::orgPassability[TROLL_SCREEN_X][TROLL_SCREEN_Y];
IUShort TrollScreen::orgBackgroundSprite[TROLL_SCREEN_X][TROLL_SCREEN_Y];
IUByte TrollScreen::orgBackgroundShift[TROLL_SCREEN_X][TROLL_SCREEN_Y];

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::TrollScreen - Constructor for a screen.

    Parameters:
      gm           (In)  The game that is loading the screen
      f            (In)  A file pointing to where the screen's information is
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollScreen::TrollScreen(TrollGame *gm, BinaryReadFile &f)
{
 int num;

 game = gm;
 // read in background sprites
 f.readUShortArray(TROLL_SCREEN_X * TROLL_SCREEN_Y,
   (IUShort *)backgroundSprite);
 // read in background sprites' color shift
 f.readUByteArray(TROLL_SCREEN_X * TROLL_SCREEN_Y, (IUByte *)backgroundShift);
 // read in passability information for squares
 f.readUShortArray(TROLL_SCREEN_X * TROLL_SCREEN_Y, (IUShort *)passability);
 // read in the number of exits followed by the exit information
 f.readUShort(exitNumPerm);
 exitNumTemp = exitNumPerm;
 exits = (TrollExit **)IMalloc(exitNumPerm * sizeof(TrollExit *));
 // According to g++ ANSI C++ disallows new with a type in () and an array
 for (num = 0; num < exitNumPerm; num++)
 {
  exits[num] = new TrollExit(f);
 }
 // read in the number of monsters followed by their types
 f.readUShort(permMonsterNum);
 if (permMonsterNum)
 {
  permMonsterTypes = new IUShort[permMonsterNum];
  f.readUShortArray(permMonsterNum, permMonsterTypes);
 }
 else
 {
  permMonsterTypes = NULL;
 }
 // read in the number of monsters followed by their types
 f.readUShort(respawnMonsterNum);
 if (respawnMonsterNum)
 {
  respawnMonsterTypes = new IUShort[respawnMonsterNum];
  f.readUShortArray(respawnMonsterNum, respawnMonsterTypes);
 }
 else
 {
  respawnMonsterTypes = NULL;
 }
 respawnNum = respawnMonsterNum;
 respawnTime = time(NULL);
 // read in the number of secrets followed by the secret information
 f.readUShort(secretNum);
 if (secretNum)
 {
  // According to g++ ANSI C++ disallows new with a type in () and an array
  secrets = (TrollSecret **)IMalloc(secretNum * sizeof(TrollSecret *));
  for (num = 0; num < secretNum; num++)
  {
   secrets[num] = new TrollSecret(game, this, f);
  }
 }
 else
 {
  secrets = NULL;
 }
 standardMonsterNum = 0;
 troll = (TrollCharacter *)0;
 message1 = message2 = NULL;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::~TrollScreen - Destructor for a screen.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollScreen::~TrollScreen()
{
 int num;

 // destroy exits
 for (num = 0; num < exitNumPerm; num++)
 {
  delete exits[num];
 }
 IFree(exits);
 // destroy monster types
 if (permMonsterTypes)
 {
  delete permMonsterTypes;
 }
 // destroy monster types
 if (respawnMonsterTypes)
 {
  delete respawnMonsterTypes;
 }
 // destroy secrets
 if (secrets)
 {
  for (num = 0; num < secretNum; num++)
  {
   delete secrets[num];
  }
  IFree(secrets);
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::addCharacter - adds a character to the screen.

    Parameters:
      trll         (In)  Character to add
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollScreen::addCharacter(TrollCharacter *trll)
{
 int i;

 // screens are inactive until a character walks unto them

 // store old backgrounds and passabilities
 memcpy(orgPassability, passability, sizeof(orgPassability));
 memcpy(orgBackgroundSprite, backgroundSprite, sizeof(orgBackgroundSprite));
 memcpy(orgBackgroundShift, backgroundShift, sizeof(orgBackgroundShift));
 // creates instances of each monster type
 for (i = 0; i < permMonsterNum; i++)
 {
  TrollThing *mon = (game->createMonster(permMonsterTypes[i]))(this);
  if (dynamic_cast<TrollStandardMonster *>(mon))
  {
   standardMonsterNum++;
  }
  permMonsters.push_back(mon);
 }
 // creates instances of each monster type
 if ((respawnMonsterNum) && (respawnNum == 0) &&
   (respawnTime + 4 /*min*/ * 60 /*sec/min*/ < time(NULL)))
 {
  respawnNum = respawnMonsterNum;
 }
 for (i = 0; i < respawnNum; i++)
 {
  TrollThing *mon = (game->createMonster(respawnMonsterTypes[i]))(this);
  if (dynamic_cast<TrollStandardMonster *>(mon))
  {
   standardMonsterNum++;
  }
  respawnMonsters.push_back(mon);
 }
 // run all appropriate secret scripts
 for (i = 0; i < secretNum; i++)
 {
  if (trll->isSecretSet(secrets[i]->getNumber()))
  {
   secrets[i]->executeSet();
  }
  else
  {
   secrets[i]->executeUnset();
  }
 }
 troll = trll;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::addCharacterProjectile - adds a character projectile to the
    screen.

    Parameters:
      charProj     (In)  Projectile to add
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollScreen::addCharacterProjectile(TrollThing *charProj)
{
 characterProj.push_back(charProj);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::addExit - adds an exit to the screen.

    Parameters:
      exit     (In)  Exit to add
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollScreen::addExit(TrollExit *exit)
{
 exitNumTemp++;
 exits = (TrollExit **)IRealloc(exits, exitNumTemp * sizeof(TrollExit *));
 exits[exitNumTemp - 1] = exit;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::addItem - adds an item to the screen.

    Parameters:
      item         (In)  Item to add
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollScreen::addItem(TrollThing *item)
{
 items.push_back(item);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::addMonster - adds a monster to the screen.

    Parameters:
      mon          (In)  Monster to add
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollScreen::addMonster(TrollThing *mon)
{
 if (dynamic_cast<TrollStandardMonster *>(mon))
 {
  standardMonsterNum++;
 }
 permMonsters.push_back(mon);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::addMonsterProjectile - adds a monster projectile to the
    screen.

    Parameters:
      monProj      (In)  Monster projectile to add
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollScreen::addMonsterProjectile(TrollThing *monProj)
{
 monsterProj.push_back(monProj);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::checkCollisionMonsterProjectile - checks if the given object
    collides with any monster projectile.

    Parameters:
      a            (In)  Thing to check against
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollScreen::checkCollisionMonsterProjectile(TrollThing *a)
{
 TrollThingIterator i;

 for (i = monsterProj.begin(); i != monsterProj.end(); i++)
 {
  if (TrollThing::checkCollision(a, (*i)))
  {
   a->takeHit(*i);
   (*i)->takeHit(troll);
  }
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::drawThings - draw everything that's on the screen to the
    virtual display screen.

    Parameters:
      scr          (In)  Screen to draw everything to
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollScreen::drawThings(IScreen scr)
{
 TrollThingIterator i;

 // Order is important here to make everything look right.
 // For example monsters should walk over items not under them.
 for (i = items.begin(); i != items.end(); i++)
 {
  (*i)->draw(scr);
 }
 for (i = permMonsters.begin(); i != permMonsters.end(); i++)
 {
  (*i)->draw(scr);
 }
 for (i = respawnMonsters.begin(); i != respawnMonsters.end(); i++)
 {
  (*i)->draw(scr);
 }
 for (i = monsterProj.begin(); i != monsterProj.end(); i++)
 {
  (*i)->draw(scr);
 }
 troll->draw(scr);
 for (i = characterProj.begin(); i != characterProj.end(); i++)
 {
  (*i)->draw(scr);
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::getBackground - get the sprite and color shift for a
    background square.

    Parameters:
      x            (In)  X location of square (not display screen location)
      y            (In)  Y location of square (not display screen location)
      sprt         (Out) Sprite number
      shft         (Out) Color shift
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollScreen::getBackground(IUShort x, IUShort y, IUShort &sprt,
  IUByte &shft)
{
 sprt = backgroundSprite[x][y];
 shft = backgroundShift[x][y];
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::getCharacter - get the character on the screen.

    Returns: A character (NULL is no one currently on the screen)
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
const TrollCharacter *TrollScreen::getCharacter()
{
 return troll;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::getExit - get an exit at the given position.

    Parameters:
      x            (In)  X location of square (display screen location)
      y            (In)  Y location of square (display screen location)

    Returns: An exit (NULL is none exist at given location)
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
const TrollExit *TrollScreen::getExit(IUShort x, IUShort y)
{
 int i;

 for (i = 0; i < exitNumTemp; i++)
 {
  if ((exits[i]->getX() == x) && (exits[i]->getY() == y))
  {
   return exits[i];
  }
 }
 return NULL;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::getGame - gets the game for the screen.

    Returns: The game object
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollGame *TrollScreen::getGame()
{
 return(game);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::getPassability - get the passibility value for a background
    square.

    Parameters:
      x            (In)  X location of square (not display screen location)
      y            (In)  Y location of square (not display screen location)

    Returns: The passability value for the square
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IUShort TrollScreen::getPassability(IUShort x, IUShort y)
{
 return(passability[x][y]);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::getStandardMonsterNum - get the number of standard monsters
    on the screen.

    Returns: The number of standard monsters on screen
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IUShort TrollScreen::getStandardMonsterNum()
{
 return(standardMonsterNum);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::getText - gets the text displayed on the screen.

    Parameters:
      msg1         (Out) First line of message
      msg2         (Out) Second line of message
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollScreen::getText(char *&msg1, char *&msg2)
{
 msg1 = message1;
 msg2 = message2;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::removeCharacter - removes the character from the screen.

    Parameters:
      trll         (In)  Troll character to remove
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollScreen::removeCharacter(TrollCharacter *trll)
{
 TrollThingIterator i;
 int num;

 // Note: trll is not currently needed since only one character can be on a
 // screen.

 // screens are inactive when no characters are on them

 // delete all monsters
 for (i = permMonsters.begin(); i != permMonsters.end(); i++)
 {
  delete(*i);
 }
 permMonsters.erase(permMonsters.begin(), permMonsters.end());
 // delete all monsters
 for (i = respawnMonsters.begin(); i != respawnMonsters.end(); i++)
 {
  delete(*i);
 }
 respawnMonsters.erase(respawnMonsters.begin(), respawnMonsters.end());
 respawnTime = time(NULL);
 // delete all moster projectiles
 for (i = monsterProj.begin(); i != monsterProj.end(); i++)
 {
  delete(*i);
 }
 monsterProj.erase(monsterProj.begin(), monsterProj.end());
 // delete all items
 for (i = items.begin(); i != items.end(); i++)
 {
  delete(*i);
 }
 items.erase(items.begin(), items.end());
 // delete all character projectiles
 for (i = characterProj.begin(); i != characterProj.end(); i++)
 {
  delete(*i);
 }
 characterProj.erase(characterProj.begin(), characterProj.end());
 // restore old backgrounds and passability
 memcpy(passability, orgPassability, sizeof(orgPassability));
 memcpy(backgroundSprite, orgBackgroundSprite, sizeof(orgBackgroundSprite));
 memcpy(backgroundShift, orgBackgroundShift, sizeof(orgBackgroundShift));
 // delete temporary exits
 for (num = exitNumPerm; num < exitNumTemp; num++)
 {
  delete exits[num];
 }
 exitNumTemp = exitNumPerm;
 exits = (TrollExit **)IRealloc(exits, exitNumPerm * sizeof(TrollExit *));
 standardMonsterNum = 0;
 // delete the character
 troll = (TrollCharacter *)0;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::setBackground - set the sprite and color shift for a
    background square.

    Parameters:
      x            (In)  X location of square (not display screen location)
      y            (In)  Y location of square (not display screen location)
      sprt         (In)  Sprite number
      shft         (In)  Color shift
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollScreen::setBackground(IUShort x, IUShort y, IUShort sprt,
  IUByte shft)
{
 backgroundSprite[x][y] = sprt;
 backgroundShift[x][y] = shft;
 if (troll)
 {
  troll->setBackground(x, y, sprt, shft);
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::setPassability - set the passibility value for a background
    square.

    Parameters:
      x            (In)  X location of square (not display screen location)
      y            (In)  Y location of square (not display screen location)
      pass         (In)  New passability value
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollScreen::setPassability(IUShort x, IUShort y, IUShort pass)
{
 passability[x][y] = pass;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::setSecret - changes a secret flag to set and runs any scripts.

    Parameters:
      num          (In)  Number of the secret to set
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollScreen::setSecret(IUShort num)
{
 int i;

 troll->setSecret(num);
 for (i = 0; i < secretNum; i++)
 {
  if (secrets[i]->getNumber() == num)
  {
   secrets[i]->executeSet();
  }
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::setText - sets the text displayed on the screen.

    Parameters:
      msg1         (In)  First line of message
      msg2         (In)  Second line of message
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollScreen::setText(char *msg1, char *msg2)
{
 message1 = msg1;
 message2 = msg2;
 if (troll)
 {
  troll->setText(message1, message2);
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollScreen::update - causes everything to react and collide.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollScreen::update()
{
 TrollThingIterator i, j;

 // Everything do something
 for (i = permMonsters.begin(); i != permMonsters.end(); i++)
 {
  (*i)->react();
 }
 for (i = respawnMonsters.begin(); i != respawnMonsters.end(); i++)
 {
  (*i)->react();
 }
 for (i = monsterProj.begin(); i != monsterProj.end(); i++)
 {
  (*i)->react();
 }
 for (i = items.begin(); i != items.end(); i++)
 {
  (*i)->react();
 }
 for (i = characterProj.begin(); i != characterProj.end(); i++)
 {
  (*i)->react();
 }
 troll->react();

 // The character has left the screen
 if (!troll)
 {
  return;
 }

 // Check collisions
 if (!troll->isInvincible())
 {
  // Check character - monster collisions
  for (i = permMonsters.begin(); i != permMonsters.end(); i++)
  {
   if (TrollThing::checkCollision(troll, (*i)))
   {
    troll->takeHit(*i);
    (*i)->takeHit(troll);
   }
  }
  // Check character - monster collisions
  for (i = respawnMonsters.begin(); i != respawnMonsters.end(); i++)
  {
   if (TrollThing::checkCollision(troll, (*i)))
   {
    troll->takeHit(*i);
    (*i)->takeHit(troll);
   }
  }
  // Check character - monster projectile collisions
  for (i = monsterProj.begin(); i != monsterProj.end(); i++)
  {
   if (TrollThing::checkCollision(troll, (*i)))
   {
    troll->takeHit(*i);
    (*i)->takeHit(troll);
   }
  }
 }
 // Check character - item collisions
 for (i = items.begin(); i != items.end(); i++)
 {
  if (TrollThing::checkCollision(troll, (*i)))
  {
   troll->takeHit(*i);
   (*i)->takeHit(troll);
  }
 }
 // Check monster - character projectile collisions
 for (j = permMonsters.begin(); j != permMonsters.end(); j++)
 {
  for (i = characterProj.begin(); i != characterProj.end(); i++)
  {
   if (TrollThing::checkCollision((*j), (*i)))
   {
    (*j)->takeHit(*i);
    (*i)->takeHit(*j);
   }
  }
 }
 // Check monster - character projectile collisions
 for (j = respawnMonsters.begin(); j != respawnMonsters.end(); j++)
 {
  for (i = characterProj.begin(); i != characterProj.end(); i++)
  {
   if (TrollThing::checkCollision((*j), (*i)))
   {
    (*j)->takeHit(*i);
    (*i)->takeHit(*j);
   }
  }
 }

 // Bring out yer dead
 for (i = permMonsters.begin(); i != permMonsters.end();)
 {
  j = i++;
  if ((*j)->isDead())
  {
   if (dynamic_cast<TrollStandardMonster *>(*j))
   {
    standardMonsterNum--;
   }
   (*j)->die();
   delete(*j);
   permMonsters.erase(j, i);
  }
 }
 for (i = respawnMonsters.begin(); i != respawnMonsters.end();)
 {
  j = i++;
  if ((*j)->isDead())
  {
   if (dynamic_cast<TrollStandardMonster *>(*j))
   {
    standardMonsterNum--;
   }
   (*j)->die();
   delete(*j);
   respawnMonsters.erase(j, i);
   respawnNum--;
  }
 }
 for (i = monsterProj.begin(); i != monsterProj.end();)
 {
  j = i++;
  if ((*j)->isDead())
  {
   (*j)->die();
   delete(*j);
   monsterProj.erase(j, i);
  }
 }
 for (i = items.begin(); i != items.end();)
 {
  j = i++;
  if ((*j)->isDead())
  {
   (*j)->die();
   delete(*j);
   items.erase(j, i);
  }
 }
 for (i = characterProj.begin(); i != characterProj.end();)
 {
  j = i++;
  if ((*j)->isDead())
  {
   (*j)->die();
   delete(*j);
   characterProj.erase(j, i);
  }
 }
 if (troll->isDead())
 {
  troll->die();
 }
}

