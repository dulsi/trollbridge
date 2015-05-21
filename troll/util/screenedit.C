#include <trollconst.h>
#include <file.h>
#include <sprite.h>
#include <string.h>
#include <igrbasics.h>
#include <igrtext.h>
#include <ikbbuffer.h>
#include <itimer.h>
#include <igrgeometry.h>
#include <ctype.h>

class TrollEditExit
{
 public:
  TrollEditExit();
  ~TrollEditExit();
  IUShort location[2];
  char *newLevel;
  IUShort newScreen[2];
  IUShort newLocation[2];
};

class TrollEditSecret
{
 public:
  TrollEditSecret();
  ~TrollEditSecret();
  IUShort secretNum;
  IUShort unsetScriptSize;
  IUByte *unsetScript;
  IUShort setScriptSize;
  IUByte *setScript;
};

class TrollEditScreen
{
 public:
  TrollEditScreen(char *fname);
  void save();

  char *filename;
  IUShort backgroundPics[TROLL_SCREEN_X][TROLL_SCREEN_Y];
  IUByte backgroundShift[TROLL_SCREEN_X][TROLL_SCREEN_Y];
  IUShort passability[TROLL_SCREEN_X][TROLL_SCREEN_Y];
  IUShort exitsNum;
  TrollEditExit **exits;
  IUShort permMonsterNum;
  IUShort *permMonsters;
  IUShort respawnMonsterNum;
  IUShort *respawnMonsters;
  IUShort secretsNum;
  TrollEditSecret **secrets;
};

TrollEditExit::TrollEditExit()
{
 int i;
 // This should be dynamically allocated based on need.  As is it will crash
 // if level names are longer than 49 characters.
 newLevel = new char[50];
 newLevel[0] = 0;
 location[0] = newScreen[0] = newScreen[1] = newLocation[0] = 0;
 location[1] = newLocation[1] = TROLL_BUFFER_Y;
};

TrollEditExit::~TrollEditExit()
{
 delete [] newLevel;
};

TrollEditSecret::TrollEditSecret()
{
 unsetScriptSize = 0;
 unsetScript = (IUByte *)IMalloc(0);
 setScriptSize = 0;
 setScript = (IUByte *)IMalloc(0);
}

TrollEditSecret::~TrollEditSecret()
{
 IFree(unsetScript);
 IFree(setScript);
}

TrollEditScreen::TrollEditScreen(char *fname)
{
 int i, num;

 filename = strdup(fname);
 BinaryReadFile f(filename);
 f.readUShortArray(TROLL_SCREEN_X * TROLL_SCREEN_Y, (IUShort *)backgroundPics);
 f.readUByteArray(TROLL_SCREEN_X * TROLL_SCREEN_Y, (IUByte *)backgroundShift);
 f.readUShortArray(TROLL_SCREEN_X * TROLL_SCREEN_Y, (IUShort *)passability);
 f.readUShort(exitsNum);
 exits = (TrollEditExit **)malloc(exitsNum * sizeof(TrollEditExit *));
 for (i = 0; i < exitsNum; i++)
 {
  exits[i] = new TrollEditExit;
  f.readUShortArray(2, exits[i]->location);
  // This should be dynamically allocated based on need.  As is it will crash
  // if level names are longer than 49 characters.
//  exits[i]->newLevel = new char[50];
  for (f.readByte((signed char &)exits[i]->newLevel[num = 0]);
    exits[i]->newLevel[num];
    f.readByte((signed char &)exits[i]->newLevel[++num]))
  {
  }
  f.readUShortArray(2, exits[i]->newScreen);
  f.readUShortArray(2, exits[i]->newLocation);
 }
 f.readUShort(permMonsterNum);
 permMonsters = (IUShort *)malloc(permMonsterNum * sizeof(IUShort));
 f.readUShortArray(permMonsterNum, permMonsters);
 f.readUShort(respawnMonsterNum);
 respawnMonsters = (IUShort *)malloc(respawnMonsterNum * sizeof(IUShort));
 f.readUShortArray(respawnMonsterNum, respawnMonsters);
 f.readUShort(secretsNum);
 secrets = (TrollEditSecret **)malloc(secretsNum * sizeof(TrollEditSecret *));
 for (i = 0; i < secretsNum; i++)
 {
  secrets[i] = new TrollEditSecret;
  f.readUShort(secrets[i]->secretNum);
  f.readUShort(secrets[i]->unsetScriptSize);
  secrets[i]->unsetScript = (IUByte *)realloc(secrets[i]->unsetScript, secrets[i]->unsetScriptSize);
  f.readUByteArray(secrets[i]->unsetScriptSize, secrets[i]->unsetScript);
  f.readUShort(secrets[i]->setScriptSize);
  secrets[i]->setScript = (IUByte *)realloc(secrets[i]->setScript, secrets[i]->setScriptSize);
  f.readUByteArray(secrets[i]->setScriptSize, secrets[i]->setScript);
 }
}

void TrollEditScreen::save()
{
 int i, num;

 BinaryWriteFile f(filename);
 f.writeUShortArray(TROLL_SCREEN_X * TROLL_SCREEN_Y, (IUShort *)backgroundPics);
 f.writeUByteArray(TROLL_SCREEN_X * TROLL_SCREEN_Y, (IUByte *)backgroundShift);
 f.writeUShortArray(TROLL_SCREEN_X * TROLL_SCREEN_Y, (IUShort *)passability);
 f.writeUShort(exitsNum);
 for (i = 0; i < exitsNum; i++)
 {
  f.writeUShortArray(2, exits[i]->location);
  f.writeByteArray(strlen(exits[i]->newLevel) + 1, (IByte *)exits[i]->newLevel);
  f.writeUShortArray(2, exits[i]->newScreen);
  f.writeUShortArray(2, exits[i]->newLocation);
 }
 f.writeUShort(permMonsterNum);
 f.writeUShortArray(permMonsterNum, permMonsters);
 f.writeUShort(respawnMonsterNum);
 f.writeUShortArray(respawnMonsterNum, respawnMonsters);
 f.writeUShort(secretsNum);
 for (i = 0; i < secretsNum; i++)
 {
  f.writeUShort(secrets[i]->secretNum);
  f.writeUShort(secrets[i]->unsetScriptSize);
  f.writeUByteArray(secrets[i]->unsetScriptSize, secrets[i]->unsetScript);
  f.writeUShort(secrets[i]->setScriptSize);
  f.writeUByteArray(secrets[i]->setScriptSize, secrets[i]->setScript);
 }
}

TrollEditScreen *editScreen = NULL;
SpriteHandler *sprites = NULL;
IScreen graphScreens[3];

void initpal()
{
 IPalette pal;
 FILE *palfile;

 if ((palfile = fopen("troll.pal", "rb"))==NULL) {
  exit(2);
 }
 pal = IPaletteCreate();
 fread(pal,1,3*256,palfile);
 fclose(palfile);
 IPaletteCopy(IPaletteMain, pal);
 IPaletteDestroy(pal);
}

// Macros to calculate draw position from the square position
#define TROLL_CALCULATE_X_POS(x) ((x) * TROLL_SQUARE_X)
#define TROLL_CALCULATE_Y_POS(y) ((y) * TROLL_SQUARE_Y + TROLL_BUFFER_Y)

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  setupBackgroundScreen - Loads all the images of the
    backgound onto one of the virtual screens.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void SetupBackgroundScreen()
{
 IUShort x, y;

 IScreenClear(graphScreens[0]);
 for (y = 0; y < TROLL_SCREEN_Y; y++)
 {
  for (x = 0; x < TROLL_SCREEN_X; x++)
  {
   const Sprite *pic =
     sprites->getSprite(editScreen->backgroundPics[x][y]);
   pic->draw(graphScreens[0], TROLL_CALCULATE_X_POS(x),
     TROLL_CALCULATE_Y_POS(y), 0, 0, editScreen->backgroundShift[x][y]);
  }
 }
}

void EditBackgrounds()
{
 IUShort sprite;
 IUByte shift;
 IUShort x, y;
 char text[6];
 bool stop=false;
 bool draw=false;
 x = y = shift = sprite = 0;
 while (!stop)
 {
  IScreenCopy(graphScreens[1], graphScreens[0]);
  ITextDraw(graphScreens[1], 10, 0, 255, "Sprite:");
  ITextDraw(graphScreens[1], 10, 10, 255, "Color Shift:");
  ITextDraw(graphScreens[1], 170, 0, 255, "X:");
  ITextDraw(graphScreens[1], 170, 10, 255, "Y:");
  sprintf(text, "%d", sprite);
  ITextDraw(graphScreens[1], 120, 0, 255, text);
  sprintf(text, "%d", shift);
  ITextDraw(graphScreens[1], 120, 10, 255, text);
  sprintf(text, "%d", x);
  ITextDraw(graphScreens[1], 250, 0, 255, text);
  sprintf(text, "%d", y);
  ITextDraw(graphScreens[1], 250, 10, 255, text);
  IScreenCopy(graphScreens[2], graphScreens[1]);
  const Sprite *pic =
    sprites->getSprite(sprite);
  pic->draw(graphScreens[1], TROLL_CALCULATE_X_POS(x),
    TROLL_CALCULATE_Y_POS(y), 0, 0, shift);
  int tick = 0;
  int cur = 1;
  IScreenCopy(IScreenMain, graphScreens[cur]);
  IGraphicsRefresh();
  while (!IKeybufferPress())
  {
   tick++;
   if (tick == 10)
   {
    cur %= 2;
    cur++;
    IScreenCopy(IScreenMain, graphScreens[cur]);
    IGraphicsRefresh();
    tick = 0;
   }
   ITimerWait();
  }
  unsigned char c = IKeybufferGet();
  switch (c)
  {
   case 0x20:
    draw = !draw;
    break;
   case 0x0D:
    editScreen->backgroundPics[x][y] = sprite;
    editScreen->backgroundShift[x][y] = shift;
    SetupBackgroundScreen();
    break;
   case '=':
    shift++;
    break;
   case '-':
    shift--;
    break;
   case ',':
    if (sprite)
    {
     sprite--;
    }
    else
    {
     sprite = sprites->getNumber() - 1;
    }
    break;
   case '.':
    sprite++;
    if (sprite == sprites->getNumber())
    {
     sprite = 0;
    }
    break;
   case 0xBD:
    if (y)
    {
     y--;
    }
    else
    {
     y = TROLL_SCREEN_Y - 1;
    }
    break;
   case 0xBF:
    if (x)
    {
     x--;
    }
    else
    {
     x = TROLL_SCREEN_X - 1;
    }
    break;
   case 0xC3:
    y++;
    if (y == TROLL_SCREEN_Y)
    {
     y = 0;
    }
    break;
   case 0xC1:
    x++;
    if (x == TROLL_SCREEN_X)
    {
     x = 0;
    }
    break;
   case 0x1B:
    stop = true;
    break;
   default:
    break;
  }
  if (draw)
  {
   editScreen->backgroundPics[x][y] = sprite;
   editScreen->backgroundShift[x][y] = shift;
   SetupBackgroundScreen();
  }
 }
}

void EditPassability()
{
 IUShort passability;
 IUShort x, y;
 char text[6];
 int i, k;
 bool stop=false;
 bool draw=false;
 x = y = passability = 0;
 while (!stop)
 {
  IScreenCopy(graphScreens[1], graphScreens[0]);
  ITextDraw(graphScreens[1], 10, 0, 255, "Passability:");
  ITextDraw(graphScreens[1], 170, 0, 255, "X:");
  ITextDraw(graphScreens[1], 170, 10, 255, "Y:");
  sprintf(text, "%d", passability);
  ITextDraw(graphScreens[1], 120, 0, 255, text);
  sprintf(text, "%d", x);
  ITextDraw(graphScreens[1], 250, 0, 255, text);
  sprintf(text, "%d", y);
  ITextDraw(graphScreens[1], 250, 10, 255, text);
  for (i = 0; i < TROLL_SCREEN_Y; i++)
  {
   for (k = 0; k < TROLL_SCREEN_X; k++)
   {
    sprintf(text, "%2d", editScreen->passability[k][i]);
    if ((i != y) || (k != x))
    {
     IBoxDraw(graphScreens[1], TROLL_CALCULATE_X_POS(k) + 1,
       TROLL_CALCULATE_Y_POS(i) + 3, TROLL_CALCULATE_X_POS(k) + 19,
       TROLL_CALCULATE_Y_POS(i) + 12, 0);
     ITextDraw(graphScreens[1], TROLL_CALCULATE_X_POS(k) + 2,
       TROLL_CALCULATE_Y_POS(i) + 4, 255, text);
    }
   }
  }
  IScreenCopy(graphScreens[2], graphScreens[1]);
  sprintf(text, "%2d", editScreen->passability[x][y]);
  IBoxDraw(graphScreens[1], TROLL_CALCULATE_X_POS(x) + 1,
    TROLL_CALCULATE_Y_POS(y) + 3, TROLL_CALCULATE_X_POS(x) + 19,
    TROLL_CALCULATE_Y_POS(y) + 12, 255);
  ITextDraw(graphScreens[1], TROLL_CALCULATE_X_POS(x) + 2,
    TROLL_CALCULATE_Y_POS(y) + 4, 0, text);
  IBoxDraw(graphScreens[2], TROLL_CALCULATE_X_POS(x) + 1,
    TROLL_CALCULATE_Y_POS(y) + 3, TROLL_CALCULATE_X_POS(x) + 19,
    TROLL_CALCULATE_Y_POS(y) + 12, 0);
  ITextDraw(graphScreens[2], TROLL_CALCULATE_X_POS(x) + 2,
    TROLL_CALCULATE_Y_POS(y) + 4, 255, text);
  int tick = 0;
  int cur = 1;
  IScreenCopy(IScreenMain, graphScreens[cur]);
  IGraphicsRefresh();
  while (!IKeybufferPress())
  {
   tick++;
   if (tick == 10)
   {
    cur %= 2;
    cur++;
    IScreenCopy(IScreenMain, graphScreens[cur]);
    IGraphicsRefresh();
    tick = 0;
   }
   ITimerWait();
  }
  unsigned char c = IKeybufferGet();
  switch (c)
  {
   case 0x20:
    draw = !draw;
    break;
   case 0x0D:
    editScreen->passability[x][y] = passability;
    break;
   case '=':
   case '.':
    passability++;
    passability %= 100;
    break;
   case '-':
   case ',':
    passability--;
    passability %= 100;
    break;
   case 0xBD:
    if (y)
    {
     y--;
    }
    else
    {
     y = TROLL_SCREEN_Y - 1;
    }
    break;
   case 0xBF:
    if (x)
    {
     x--;
    }
    else
    {
     x = TROLL_SCREEN_X - 1;
    }
    break;
   case 0xC3:
    y++;
    if (y == TROLL_SCREEN_Y)
    {
     y = 0;
    }
    break;
   case 0xC1:
    x++;
    if (x == TROLL_SCREEN_X)
    {
     x = 0;
    }
    break;
   case 0x1B:
    stop = true;
    break;
   default:
    break;
  }
  if (draw)
  {
   editScreen->passability[x][y] = passability;
  }
 }
}

struct menuItem
{
 char *text;
 bool isNum;
 IUShort min;
 IUShort max;
 IUShort inc;
};

#define EXIT_MENU_NUM 5
static struct menuItem exitMenu[EXIT_MENU_NUM] =
{
 {"Filename:", false, 0, 0, 0},
 {"Map X Position:", true, 0, TROLL_LEVEL_X, 1},
 {"Map Y Position:", true, 0, TROLL_LEVEL_Y, 1},
 {"Screen X Position:", true, 0, 320 - TROLL_SQUARE_X, TROLL_SQUARE_X / 4},
 {"Screen Y Position:", true, TROLL_BUFFER_Y, 200 - TROLL_SQUARE_Y, TROLL_SQUARE_Y / 4}
};

void EditExitSquare(IUShort x, IUShort y)
{
 int exitSquare;
 bool stop = false;
 int i, where;
 char text[10];
 void *menuVar[EXIT_MENU_NUM];

 for (exitSquare = 0; exitSquare < editScreen->exitsNum; exitSquare++)
 {
  if ((editScreen->exits[exitSquare]->location[0] == x) &&
    (editScreen->exits[exitSquare]->location[1] == y))
  {
   break;
  }
 }
 if (exitSquare == editScreen->exitsNum)
 {
  editScreen->exitsNum++;
  editScreen->exits = (TrollEditExit **)realloc(editScreen->exits, editScreen->exitsNum * sizeof(TrollEditExit *));
  editScreen->exits[exitSquare] = new TrollEditExit;
  editScreen->exits[exitSquare]->location[0] = x;
  editScreen->exits[exitSquare]->location[1] = y;
 }
 menuVar[0] = editScreen->exits[exitSquare]->newLevel;
 menuVar[1] = editScreen->exits[exitSquare]->newScreen;
 menuVar[2] = editScreen->exits[exitSquare]->newScreen + 1;
 menuVar[3] = editScreen->exits[exitSquare]->newLocation;
 menuVar[4] = editScreen->exits[exitSquare]->newLocation + 1;
 where = 0;
 while (!stop)
 {
  IScreenCopy(graphScreens[2], graphScreens[1]);
  for (i = 0; i < EXIT_MENU_NUM; i++)
  {
   IBoxDraw(graphScreens[2], 49, 44 + 15 * i, 250, 53 + 15 * i, (i == where) ? 255 : 0);
   ITextDraw(graphScreens[2], 50, 45 + 15 * i, (i == where) ? 0 : 255, exitMenu[i].text);
   if (exitMenu[i].isNum)
   {
    sprintf(text, "%d", *((IUShort *)menuVar[i]));
    ITextDraw(graphScreens[2], 200, 45 + 15 * i, (i == where) ? 0 : 255, text);
   }
   else
   {
    ITextDraw(graphScreens[2], 130, 45 + 15 * i, (i == where) ? 0 : 255, (unsigned char *)menuVar[i]);
   }
  }
  IScreenCopy(IScreenMain, graphScreens[2]);
  IGraphicsRefresh();
  unsigned char c = IKeybufferGet();
  switch (c)
  {
   case 0xBD:
    if (where)
    {
     where--;
    }
    else
    {
     where = EXIT_MENU_NUM - 1;
    }
    break;
   case 0x0D:
   case 0xC3:
    where++;
    if (where == EXIT_MENU_NUM)
    {
     where = 0;
    }
    break;
   case 0xBF:
    if (exitMenu[where].isNum)
    {
     if (*((IUShort *)menuVar[where]) == exitMenu[where].min)
     {
      *((IUShort *)menuVar[where]) = exitMenu[where].max;
     }
     else
     {
      *((IUShort *)menuVar[where]) -= exitMenu[where].inc;
     }
    }
    break;
   case 0xC1:
    if (exitMenu[where].isNum)
    {
     if (*((IUShort *)menuVar[where]) == exitMenu[where].max)
     {
      *((IUShort *)menuVar[where]) = exitMenu[where].min;
     }
     else
     {
      *((IUShort *)menuVar[where]) += exitMenu[where].inc;
     }
    }
    break;
   case 0x08:
    if (!exitMenu[where].isNum)
    {
     if (i = strlen((char *)menuVar[where]))
     {
      ((char *)menuVar[where])[i - 1] = 0;
     }
    }
    break;
   case 0x1B:
    stop = true;
    break;
   default:
    if ((!exitMenu[where].isNum) && ((isalnum(c)) || (c == '.')))
    {
     text[0] = c;
     text[1] = 0;
     strcat((char *)menuVar[where], text);
    }
    break;
  }
 }
}

void DeleteExitSquare(IUShort x, IUShort y)
{
 int i;
 for (i = 0; i < editScreen->exitsNum; i++)
 {
  if ((editScreen->exits[i]->location[0] == x) &&
    (editScreen->exits[i]->location[1] == y))
  {
   break;
  }
 }
 if (i < editScreen->exitsNum)
 {
  delete editScreen->exits[i];
  editScreen->exitsNum--;
  for (; i < editScreen->exitsNum; i++)
  {
   editScreen->exits[i] = editScreen->exits[i + 1];
  }
 }
}

void EditExits()
{
 IUShort x, y;
 char text[6];
 int i, k;
 bool stop=false;

 x = 0;
 y = TROLL_BUFFER_Y;
 while (!stop)
 {
  IScreenCopy(graphScreens[1], graphScreens[0]);
  for (i = 0; i < editScreen->exitsNum; i++)
  {
   IRectangleDraw(graphScreens[1], editScreen->exits[i]->location[0],
     editScreen->exits[i]->location[1],
     editScreen->exits[i]->location[0] + TROLL_SQUARE_X - 1,
     editScreen->exits[i]->location[1] + TROLL_SQUARE_Y - 1, 0);
   IRectangleDraw(graphScreens[1], editScreen->exits[i]->location[0] + 1,
     editScreen->exits[i]->location[1] + 1,
     editScreen->exits[i]->location[0] + TROLL_SQUARE_X - 2,
     editScreen->exits[i]->location[1] + TROLL_SQUARE_Y - 2, 255);
  }
  ITextDraw(graphScreens[1], 170, 0, 255, "X:");
  ITextDraw(graphScreens[1], 170, 10, 255, "Y:");
  sprintf(text, "%d", x);
  ITextDraw(graphScreens[1], 250, 0, 255, text);
  sprintf(text, "%d", y);
  ITextDraw(graphScreens[1], 250, 10, 255, text);
  IScreenCopy(graphScreens[2], graphScreens[1]);
   IRectangleDraw(graphScreens[1], x, y, x + TROLL_SQUARE_X - 1,
     y + TROLL_SQUARE_Y - 1, 255);
   IRectangleDraw(graphScreens[1], x + 1, y + 1, x + TROLL_SQUARE_X - 2,
     y + TROLL_SQUARE_Y - 2, 0);
  int tick = 0;
  int cur = 1;
  IScreenCopy(IScreenMain, graphScreens[cur]);
  IGraphicsRefresh();
  while (!IKeybufferPress())
  {
   tick++;
   if (tick == 10)
   {
    cur %= 2;
    cur++;
    IScreenCopy(IScreenMain, graphScreens[cur]);
    IGraphicsRefresh();
    tick = 0;
   }
   ITimerWait();
  }
  unsigned char c = IKeybufferGet();
  switch (c)
  {
   case 0x20:
   case 0x0D:
    EditExitSquare(x,y);
    break;
   case 'd':
    DeleteExitSquare(x,y);
    break;
   case 0xBD:
    y -= TROLL_SQUARE_Y / 4;
    if (y < TROLL_BUFFER_Y)
    {
     y = 200 - TROLL_SQUARE_Y;
    }
    break;
   case 0xBF:
    if (x)
    {
     x -= TROLL_SQUARE_X / 4;
    }
    else
    {
     x = 320 - TROLL_SQUARE_X;
    }
    break;
   case 0xC3:
    y += TROLL_SQUARE_Y / 4;
    if (y > 200 - TROLL_SQUARE_Y)
    {
     y = TROLL_BUFFER_Y;
    }
    break;
   case 0xC1:
    x += TROLL_SQUARE_X / 4;
    if (x > 320 - TROLL_SQUARE_X)
    {
     x = 0;
    }
    break;
   case 0x1B:
    stop = true;
    break;
   default:
    break;
  }
 }
}

void EditMonsters(IUShort &monsterNum, IUShort *&monsters)
{
 int where;
 int top;
 int i;
 bool stop = false;
 char text[10];

 where = top = 0;
 while (!stop)
 {
  IScreenCopy(graphScreens[1], graphScreens[0]);
  IBoxDraw(graphScreens[1], 49, 44, 250, 193, 0);
  for (i = 0; (i + top < monsterNum) && (i < 15); i++)
  {
   sprintf(text, "%d", monsters[i + top]);
   if (i == where)
   {
    IBoxDraw(graphScreens[1], 49, 44 + 10 * i, 250, 53 + 10 * i, 255);
    ITextDraw(graphScreens[1], 50, 45 + 10 * i, 0, text);
   }
   else
   {
    ITextDraw(graphScreens[1], 50, 45 + 10 * i, 255, text);
   }
  }
  if (where == i)
  {
   // New monster
   IBoxDraw(graphScreens[1], 49, 44 + 10 * i, 250, 53 + 10 * i, 255);
  }
  IScreenCopy(IScreenMain, graphScreens[1]);
  IGraphicsRefresh();
  unsigned char c = IKeybufferGet();
  switch (c)
  {
   case 'd':
    if (where + top < monsterNum)
    {
     for (i = where + top + 1; i < monsterNum; i++)
      monsters[i - 1] = monsters[i];
     monsterNum--;
    }
    break;
   case 0xBD:
    if (where)
    {
     where--;
    }
    else if (top)
    {
     top--;
    }
    break;
   case 0xC3:
    if ((where < 14) && (where + top <monsterNum))
    {
     where++;
    }
    else if (where + top < monsterNum)
    {
     top++;
    }
    break;
   case 0xBF:
    if (where + top == monsterNum)
    {
     monsterNum++;
     monsters = (IUShort *)realloc(monsters,
       monsterNum * sizeof(IUShort));
     monsters[where + top] = 0;
    }
    else
     monsters[where + top]--;
    break;
   case 0xC1:
    if (where + top == monsterNum)
    {
     monsterNum++;
     monsters = (IUShort *)realloc(monsters,
       monsterNum * sizeof(IUShort));
     monsters[where + top] = 0;
    }
    else
     monsters[where + top]++;
    break;
   case 0x1B:
    stop = true;
    break;
   default:
    break;
  }
 }
}

bool FilenameGet(char *filename)
{
 char text[2];
 int i;

 filename[0] = 0;
 while (1)
 {
  IScreenCopy(graphScreens[2], graphScreens[0]);
  IBoxDraw(graphScreens[2], 49, 44, 250, 53, 0);
  ITextDraw(graphScreens[2], 50, 45, 255, "Filename:");
  ITextDraw(graphScreens[2], 130, 45, 255, filename);
  IScreenCopy(IScreenMain, graphScreens[2]);
  IGraphicsRefresh();
  unsigned char c = IKeybufferGet();
  switch (c)
  {
   case 0x0D:
    return true;
    break;
   case 0x08:
    if (i = strlen(filename))
    {
     filename[i - 1] = 0;
    }
    break;
   case 0x1B:
    return false;
    break;
   default:
    if ((isalnum(c)) || (c == '.'))
    {
     text[0] = c;
     text[1] = 0;
     strcat(filename, text);
    }
    break;
  }
 }
}

struct secretFunctions
{
 IUByte code;
 char *name;
 IUShort *argTypes;
};

#define SECRET_FUNCTION_NUM 5

#define TROLL_ARG_END    0
#define TROLL_ARG_BYTE   1
#define TROLL_ARG_SHORT  2
#define TROLL_ARG_STRING 3

IUShort CreateMonsterArguments[] =
{
 TROLL_ARG_SHORT, TROLL_ARG_SHORT, TROLL_ARG_END
};

IUShort CreateItemArguments[] =
{
 TROLL_ARG_SHORT, TROLL_ARG_SHORT, TROLL_ARG_SHORT, TROLL_ARG_SHORT,
 TROLL_ARG_END
};

IUShort SetBackgroundArguments[] =
{
 TROLL_ARG_SHORT, TROLL_ARG_SHORT, TROLL_ARG_SHORT, TROLL_ARG_BYTE,
 TROLL_ARG_SHORT, TROLL_ARG_END
};

IUShort CreateExitArguments[] =
{
 TROLL_ARG_SHORT, TROLL_ARG_SHORT, TROLL_ARG_STRING, TROLL_ARG_SHORT,
 TROLL_ARG_SHORT, TROLL_ARG_SHORT, TROLL_ARG_SHORT, TROLL_ARG_END
};

IUShort SetTextArguments[] =
{
 TROLL_ARG_STRING, TROLL_ARG_STRING, TROLL_ARG_END
};

struct secretFunctions SecretFunctions[SECRET_FUNCTION_NUM] =
{
 {0x01, "CreateMonster", CreateMonsterArguments},
 {0x02, "CreateItem", CreateItemArguments},
 {0x03, "SetBackground", SetBackgroundArguments},
 {0x04, "CreateExit", CreateExitArguments},
 {0x05, "SetText", SetTextArguments},
};

void SecretScriptWrite(FILE *f, IUByte *script)
{
 int j, k, l;

 for (k = 0; script[k];)
 {
  for (j = 0; j < SECRET_FUNCTION_NUM; j++)
  {
   if (SecretFunctions[j].code == script[k])
   {
    break;
   }
  }
  if (j == SECRET_FUNCTION_NUM)
  {
   return;
  }
  fprintf(f, " %s(", SecretFunctions[j].name);
  k++;
  for (l = 0; SecretFunctions[j].argTypes[l] != TROLL_ARG_END; l++)
  {
   if (l)
   {
    fprintf(f, ", ");
   }
   switch (SecretFunctions[j].argTypes[l])
   {
    case TROLL_ARG_SHORT:
     fprintf(f, "%d", *((IUShort *)(script + k)));
     k += 2;
     break;
    case TROLL_ARG_BYTE:
     fprintf(f, "%d", *((IUByte *)(script + k)));
     k++;
     break;
    case TROLL_ARG_STRING:
     fprintf(f, "\"%s\"", (char *)(script + k));
     k += strlen((char *)(script + k)) + 1;
     break;
    default:
     break;
   }
  }
  fprintf(f, ");\n");
 }
}

void ExportSecrets(char *filename)
{
 int i;
 FILE *f;

 if ((f = fopen(filename, "w")) != NULL)
 {
  for (i = 0; i < editScreen->secretsNum; i++)
  {
   fprintf(f, "unset%d()\n{\n", editScreen->secrets[i]->secretNum);
   if (editScreen->secrets[i]->unsetScriptSize)
   {
    SecretScriptWrite(f, editScreen->secrets[i]->unsetScript);
   }
   fprintf(f, "}\n\nset%d()\n{\n", editScreen->secrets[i]->secretNum);
   if (editScreen->secrets[i]->setScriptSize)
   {
    SecretScriptWrite(f, editScreen->secrets[i]->setScript);
   }
   fprintf(f, "}\n\n");
  }
  fclose(f);
 }
}

void ImportSecrets(char *filename)
{
 int i,j,k,m;
 int type;
 FILE *f;
 char s[256];
 IUShort zzz;
 IUShort curSecretNum = 0;
 IUShort ttlSecretNum = 0;
 TrollEditSecret **secrets;
 IUByte **script;
 IUShort *scriptSize;

 secrets = (TrollEditSecret **)malloc(ttlSecretNum * sizeof(TrollEditSecret *));
 if ((f = fopen(filename, "r")) != NULL)
 {
  s[0] = 0;
  while (!feof(f))
  {
   while (fgets(s, 255, f))
   {
    if (strncmp("unset", s, 5) == 0)
    {
     type = 1;
     curSecretNum = atoi(s + 5);
     break;
    }
    else if (strncmp("set", s, 3) == 0)
    {
     type=2;
     curSecretNum = atoi(s + 3);
     break;
    }
    s[0] = 0;
   }
   if (s[0])
   {
    fgets(s, 255, f);
    if (strcmp(s, "{\n"))
     return;
    for (i = 0; i < ttlSecretNum; i++)
    {
     if (secrets[i]->secretNum == curSecretNum)
     {
      break;
     }
    }
    if (i == ttlSecretNum)
    {
     ttlSecretNum++;
     secrets = (TrollEditSecret **)realloc(secrets, ttlSecretNum * sizeof(TrollEditSecret *));
     secrets[i] = new TrollEditSecret;
     secrets[i]->secretNum = curSecretNum;
    }
    if (type == 1)
    {
     script = &secrets[i]->unsetScript;
     scriptSize = &secrets[i]->unsetScriptSize;
    }
    else
    {
     script = &secrets[i]->setScript;
     scriptSize = &secrets[i]->setScriptSize;
    }
    *scriptSize = 0;
    *script = (IUByte *)realloc(*script, 1000);
    while (fgets(s, 255, f))
    {
     if (strcmp(s, "}\n") == 0)
     {
      if (*scriptSize)
      {
       (*script)[*scriptSize] = 0;
       (*scriptSize)++;
      }
      break;
     }
     for (k = 0; isspace(s[k]); k++) ;
     if (s[k])
     {
      for (j = 0; j < SECRET_FUNCTION_NUM; j++)
      {
       if (strncmp(SecretFunctions[j].name, s+k, strlen(SecretFunctions[j].name)) == 0)
       {
        break;
       }
      }
      if (j < SECRET_FUNCTION_NUM)
      {
       k += strlen(SecretFunctions[j].name) + 1;
       (*script)[*scriptSize] = SecretFunctions[j].code;
       (*scriptSize)++;
       for (m = 0; SecretFunctions[j].argTypes[m] != TROLL_ARG_END; m++)
       {
        switch (SecretFunctions[j].argTypes[m])
        {
         case TROLL_ARG_SHORT:
          for (; isspace(s[k]); k++) ;
          zzz = atoi(s+k);
          memcpy((*script) + (*scriptSize), &zzz, sizeof(IUShort));
          *scriptSize += 2;
          for (; (s[k] != ',') && (s[k] != ')'); k++) ;
          k++;
          break;
         case TROLL_ARG_BYTE:
          IUByte zzz2;
          for (; isspace(s[k]); k++) ;
          zzz2 = atoi(s+k);
          memcpy((*script) + (*scriptSize), &zzz2, sizeof(IUByte));
          *scriptSize += 1;
          for (; (s[k] != ',') && (s[k] != ')'); k++) ;
          k++;
          break;
         case TROLL_ARG_STRING:
          for (; isspace(s[k]); k++) ;
          if (s[k] != '\"')
           return;
          k++;
          for (zzz = k; s[zzz] != '\"'; zzz++)
           if (s[zzz] == 0) return;
          memcpy((*script) + (*scriptSize), s + k, zzz - k);
          *scriptSize += zzz - k + 1;
          *((*script) + (*scriptSize) - 1) = 0;
          for (k = zzz + 1; (s[k] != ',') && (s[k] != ')'); k++) ;
          k++;
          break;
         default:
          break;
        }
       }
      }
     }
    }
   }
  }
  fclose(f);
  editScreen->secretsNum = ttlSecretNum;
  editScreen->secrets = secrets;
//  printf("s - %d\n",ttlSecretNum);
 }
}

static char *secretMenu[] =
{
 "E - Export Secrets",
 "I - Import Secrets",
 "   ESC - Quit",
 NULL
};

void EditSecrets()
{
 int i;
 bool stop = false;
 char filename[255];

 for (i = 0; i < 3; i++)
 {
  graphScreens[i] = IScreenCreate();
 }
 SetupBackgroundScreen();
 while (!stop)
 {
  IScreenCopy(graphScreens[1], graphScreens[0]);
  for (i = 0; secretMenu[i]; i++)
  {
   IBoxDraw(graphScreens[1], 49, 44 + 15 * i, 250, 53 + 15 * i, 0);
   ITextDraw(graphScreens[1], 50, 45 + 15 * i, 255, secretMenu[i]);
  }
  IScreenCopy(IScreenMain, graphScreens[1]);
  IGraphicsRefresh();
  switch (IKeybufferGet())
  {
   case 'e':
    if (FilenameGet(filename))
    {
     ExportSecrets(filename);
    }
    break;
   case 'i':
    if (FilenameGet(filename))
    {
     ImportSecrets(filename);
    }
    break;
   case 0x1B:
    stop = true;
    break;
   default:
    break;
  }
 }
}

static char *editMenu[]=
{
 "B - Backgrounds",
 "P - Passability",
 "E - Exits",
 "M - Permanent Monsters",
 "R - Respawn Monsters",
 "C - Secrets",
 "S - Save",
 "   ESC - Quit",
 NULL
};

void Edit()
{
 int i;
 bool stop = false;
 for (i = 0; i < 3; i++)
 {
  graphScreens[i] = IScreenCreate();
 }
 SetupBackgroundScreen();
 while (!stop)
 {
  IScreenCopy(graphScreens[1], graphScreens[0]);
  for (i = 0; editMenu[i]; i++)
  {
   IBoxDraw(graphScreens[1], 49, 44 + 15 * i, 250, 53 + 15 * i, 0);
   ITextDraw(graphScreens[1], 50, 45 + 15 * i, 255, editMenu[i]);
  }
  IScreenCopy(IScreenMain, graphScreens[1]);
  IGraphicsRefresh();
  switch (IKeybufferGet())
  {
   case 'b':
    EditBackgrounds();
    break;
   case 'p':
    EditPassability();
    break;
   case 'e':
    EditExits();
    break;
   case 'm':
    EditMonsters(editScreen->permMonsterNum, editScreen->permMonsters);
    break;
   case 'r':
    EditMonsters(editScreen->respawnMonsterNum, editScreen->respawnMonsters);
    break;
   case 'c':
    EditSecrets();
    break;
   case 's':
    editScreen->save();
    break;
   case 0x1B:
    stop = true;
    break;
   default:
    break;
  }
 }
}

int main(int argc, char *argv[])
{
 if (argc == 2)
 {
  editScreen = new TrollEditScreen(argv[1]);
  sprites = new SpriteHandler("troll.spr");
  IGraphicsStart("ScreenEdit");
  IKeybufferStart();
  initpal();
  ITimerStart(30);
  Edit();
  ITimerEnd();
  IKeybufferEnd();
  IGraphicsEnd();
 }
 return 0;
}

