/*-------------------------------------------------------------------------*\
  <game.C> -- Implemention for TrollGame methods

  Date      Programmer  Description
  04/17/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include <itimer.h>
#include <igrgeometry.h>
#include <igrimage.h>
#include <igrpalette.h>
#include <igrtext.h>
#include <ikbbuffer.h>
#include <sys/stat.h>
#include <errno.h>
#include <getopt.h>
#include <boost/filesystem.hpp>
#include "troll.h"

#ifndef STATIC_LIBRARY
#include <dlfcn.h>
#else
extern "C" int TrollDllInit(TrollGame *game);
#endif

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::TrollGame - Constructor for the main game.

    Parameters:
      argc         (In)  Number of argument [not currently used]
      argv         (In)  Arguments [not currently used]
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollGame::TrollGame(int argc, char **argv)
: dataDir(DATA_DIR), libDir(LIBRARY_DIR), musicObj(NULL)
{
 int x,y;
 IUShort xMult, yMult;
 IBool fullScreen;
 IBool soft;
 char *file;
 char *defName = "troll.def";
 int opt;
 IPalette pal;
 static struct option long_options[] =
 {
  {"def", 1, 0, 'd'},
  {"lib", 1, 0, 'l'},
  {"data", 1, 0, 'a'},
  {0, 0, 0, 0}
 };

 xMult = 0;
 yMult = 0;
 fullScreen = IFALSE;
 soft = IFALSE;
 while ((opt = getopt_long(argc, argv, "h?d:x:y:fwa:l:", long_options, NULL)) != EOF)
 {
  switch (opt)
  {
   case 'h':
   case '?':
    printf("\
Starts a game of Troll Bridge\n\
\n\
troll [options]\n\
\n\
  Options:\n\
    -d, --def=FILENAME   World definition file (defaults to \"troll.def\")\n\
    -f                   Full screen mode\n\
    -x#                  Screen width multiplier\n\
    -y#                  Screen height multiplier\n\
");
    exit(0);
    break;
   case 'd':
    defName = strdup(optarg);
    if (strchr(defName, '/') != NULL)
    {
     printf("Definition files must be stored in %s\n", DATA_DIR);
     exit(-1);
    }
    break;
   case 'a':
    dataDir = optarg;
    break;
   case 'l':
    libDir = optarg;
    break;
   case 'x':
    sscanf(optarg, "%d", &x);
    xMult = x;
    break;
   case 'y':
    sscanf(optarg, "%u", &y);
    yMult = y;
    break;
   case 'f':
    fullScreen = ITRUE;
    break;
   case 'w':
    soft = ITRUE;
    break;
   default:
    break;
  }
 }

 IGraphicsStart("Troll Bridge", xMult, yMult, fullScreen, soft);
 Mix_Init(MIX_INIT_OGG);

 char *home = getenv("HOME");
 if (home)
 {
  int len = strlen(home);
  savePath = (char *)IMalloc(len + 12);
  strcpy(savePath, home);
  if (savePath[len - 1] != '/')
  {
   strcat(savePath, "/");
  }
 }
 else
 {
  savePath = (char *)IMalloc(12);
  savePath[0] = 0;
 }
#if defined(__MSDOS__) || defined(_WIN32)
 strcat(savePath, "troll.dot");
 boost::filesystem::create_directory(savePath);
#else
 strcat(savePath, ".troll");
 int err = mkdir(savePath, 0700);
 if ((-1 == err) && (EEXIST != errno))
 {
  fprintf(stderr, "Error creating directory %s\n", savePath);
  exit(2);
 }
#endif
 strcat(savePath, "/");

 // Run at 30 frames per second
 ITimerStart(30);

 file = buildFullPath(dataDir.c_str(), defName);
 definition = new TrollDefinition(file);
 delete[] file;
 file = buildFullPath(dataDir.c_str(), definition->getPaletteFile());
 pal = IPalettePalLoad(file);
 delete[] file;
 IPaletteSet(pal, 255, 255, 255, 255);
 IPaletteCopy(IPaletteMain, pal);
 IPaletteDestroy(pal);
 file = buildFullPath(dataDir.c_str(), definition->getSpriteFile());
 TrollSpriteHandler.load(file);
 delete[] file;
 // Load monster and items from dynamically linked library
 file = buildFullPath(libDir.c_str(), definition->getDllFile());
 loadLibrary(file);
 delete[] file;
 // Initialize variables to nothing
 extraScreen = NULL;
 for (x = 0; x < TROLL_LEVEL_X; x++)
 {
  for (y = 0; y < TROLL_LEVEL_Y; y++)
  {
   screen[x][y] = NULL;
  }
 }
 troll = NULL;
 control = new nes_controller();
 file = buildFullPath(dataDir.c_str(), definition->getTitleFile());
 titlePic = IImagePCXLoad(file);
 delete[] file;
 levelName = NULL;
 Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::~TrollGame - Destructor for the main game.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollGame::~TrollGame()
{
 if (musicObj)
 {
  Mix_HaltMusic();
  Mix_FreeMusic(musicObj);
 }
 IImageDestroy(titlePic);
 delete definition;
 delete[] savePath;
 ITimerEnd();
 Mix_Quit();
 IGraphicsEnd();
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::run - Runs all necessary tasks for the game.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollGame::run()
{
 char name[10]; // max of 8 characters but extra space just in case

 // display title screen
 while (titleScreen(name))
 {
  // load character
  troll = new TrollCharacter(this, name, definition->getStartFile(), control);

  // loop until the character dies
  while (!troll->isDead())
  {
   troll->display();
   turnCleanUp();
   ITimerWait();
   troll->getScreen()->update();
  }

  delete troll;
  troll = (TrollCharacter *)0;
 }

 delete control;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::addCarriedItem - Called by dynamically linked libraries to
    specify carried items defined in the library.

    Parameters:
      item         (In)  Actual item
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollGame::addCarriedItem(TrollCarriedItem *item)
{
 carriedItems[item->getItemNum()] = item;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::addItemType - Called by dynamically linked libraries to specify
    item types defined in the library.

    Parameters:
      num          (In)  Number of the item type
      construct    (In)  Function to create an instance of the item type
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollGame::addItemType(IUShort num, TrollItemConstructor construct)
{
 if (num == itemTypes.size())
  itemTypes.push_back(construct);
 else
 {
  for (int i = itemTypes.size(); i < num; ++i)
   itemTypes.push_back(NULL);
  itemTypes[num] = construct;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::addMonsterType - Called by dynamically linked libraries to
    specify monster types defined in the library.

    Parameters:
      num          (In)  Number of the moster type
      construct    (In)  Function to create an instance of the monster type
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollGame::addMonsterType(IUShort num, TrollMonsterConstructor construct)
{
 if (num == monsterTypes.size())
  monsterTypes.push_back(construct);
 else
 {
  for (int i = monsterTypes.size(); i < num; ++i)
   monsterTypes.push_back(NULL);
  monsterTypes[num] = construct;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::createItem - Gets the function to create the specified item.

    Parameters:
      itemNum      (In)  Number of the item

    Returns: The function to create the item
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollItemConstructor TrollGame::createItem(int itemNum)
{
 return(itemTypes[itemNum]);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::createMonster - Gets the function to create the specified
    monster.

    Parameters:
      monsterNum   (In)  Number of the monster

    Returns: The function to create the monster
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollMonsterConstructor TrollGame::createMonster(int monsterNum)
{
 return(monsterTypes[monsterNum]);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::getCarriedItem - Gets the carried item specified.

    Parameters:
      num          (In)  Number of the carried item

    Returns: The appropriate carried item
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollCarriedItem *TrollGame::getCarriedItem(IUShort num)
{
 return(carriedItems[num]);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::getLevelName - Gets the level name.

    Returns: The level name
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
const char *TrollGame::getLevelName()
{
 return(levelName);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::getMapInfo - Gets the map info.

    Parameters:
      x            (In)  X position on map
      y            (In)  Y position on map

    Returns: The map info
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IUByte TrollGame::getMapInfo(IUShort x, IUShort y)
{
 return(mapInfo[x][y]);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::getSavePath - Gets the save path.

    Returns: The save path
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
const char *TrollGame::getSavePath()
{
 return(savePath);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::getScreen - Gets the screen specified by x and y position.

    Parameters:
      x            (In)  X position
      y            (In)  Y position

    Returns: The appropriate screen
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollScreen *TrollGame::getScreen(IUShort x, IUShort y)
{
 return(screen[x][y]);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::getXScreen - Gets the x location of the starting level screen.

    Returns: The x location of the starting level screen
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IUShort TrollGame::getXScreen()
{
 return(xScreen);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::getXStart - Gets the x location on the starting screen.

    Returns: The x location on the starting screen
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IUShort TrollGame::getXStart()
{
 return(xStart);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::getYScreen - Gets the y location of the starting level screen.

    Returns: The y location of the starting level screen
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IUShort TrollGame::getYScreen()
{
 return(yScreen);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::getYStart - Gets the y location on the starting screen.

    Returns: The y location on the starting screen
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IUShort TrollGame::getYStart()
{
 return(yStart);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::loadLevel - Loads a level from a file.

    Parameters:
      filename     (In)  Name of the level file (including ".trb")
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollGame::loadLevel(const char *filename)
{
 int x, y;
 IULong *header;
 IUShort musicLen;
 char *file;

 file = buildFullPath(dataDir.c_str(), filename);

 BinaryReadFile levelFile(file);

 delete[] file;
 if (levelName)
 {
  delete[] levelName;
 }
 levelName = new char[strlen(filename) + 1];
 strcpy(levelName, filename);

 // The current screen of the character can't be deleted until it finishes
 // processing.
 if (troll)
 {
  extraScreen = troll->getScreen();
 }

 // Read in the map info
 levelFile.readUShort(xScreen);
 levelFile.readUShort(yScreen);
 levelFile.readUShort(xStart);
 levelFile.readUShort(yStart);
 levelFile.readUShort(musicLen);
 char *musictmp = new char[musicLen + 1];
 musictmp[musicLen] = 0;
 levelFile.readUByteArray(musicLen, (IUByte *)musictmp);
 setMusic(musictmp);
 delete musictmp;
 levelFile.readUByteArray(TROLL_LEVEL_X * TROLL_LEVEL_Y, (IUByte *)mapInfo);
/* for (x = 0; x < TROLL_LEVEL_X; x++)
 {
  for (y = 0; y < TROLL_LEVEL_Y; y++)
  {
   mapInfo[x][y] = TROLL_LIGHTGRAY;
  }
 }*/

 // Read in the header that stores location and size of each screen
 header = new IULong[TROLL_LEVEL_X * TROLL_LEVEL_Y * 2];
 levelFile.readULongArray(TROLL_LEVEL_X * TROLL_LEVEL_Y * 2, header);
 // Read in all the screens
 for (x = 0; x < TROLL_LEVEL_X; x++)
 {
  for (y = 0; y < TROLL_LEVEL_Y; y++)
  {
   if ((screen[x][y]) && (extraScreen != screen[x][y]))
   {
    delete screen[x][y];
   }
   levelFile.seek(header[(x * TROLL_LEVEL_Y + y) * 2]);
   screen[x][y] = new TrollScreen(this, levelFile);
  }
 }
 // Delete the header
 delete [] header;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::buildFullPath - Builds a complete file path.

    Returns: Complete file path (caller must delete when finished).
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
char *TrollGame::buildFullPath(const char *path, const char *file)
{
 char *fullPath;
 int pathLen = strlen(path);

 fullPath = new char[pathLen + strlen(file) + 2];
 strcpy(fullPath, path);
 if ('/' != fullPath[pathLen - 1])
 {
  strcat(fullPath, "/");
 }
 strcat(fullPath, file);
 return fullPath;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::loadLibrary - Loads a library and runs TrollDllInit.

    Parameters:
      filename     (In)  Name of the library (including ".dll")
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollGame::loadLibrary(const char *filename)
{
#ifndef STATIC_LIBRARY
 void *lib;
 int (*libinit)(TrollGame *);

 // Note: Loading of dynamic libraries is system specific.

 lib = dlopen(filename, RTLD_NOW);
 if (lib == (void *)0)
 {
  fprintf(stderr, "Error loading library %s\n", filename);
  exit(1);
 }
 libinit = (int (*)(TrollGame *))dlsym(lib, "TrollDllInit");
 if (libinit == (int (*)(TrollGame *))0)
 {
  fprintf(stderr, "Error finding initialization function for library %s\n",
    filename);
  exit(1);
 }
 if (libinit(this))
 {
  fprintf(stderr, "Initialization function for library %s failed\n",
    filename);
  exit(1);
 }
#else
 if (TrollDllInit(this))
 {
  fprintf(stderr, "Initialization function for library %s failed\n",
    filename);
  exit(1);
 }
#endif
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::selectName - Allows the player to select the character's name.

    Parameters:
      name         (Out) Name of the character (already allocated max 8 chars)
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollGame::selectName(char *name)
{
 signed char d, oldD, r, oldR;
 unsigned char a, b, start;
 IScreen bufScreen[2];
 char c[2];
 int x, y;
 int num, len;

 // Some of the data from the nes controller is ignored.
 unsigned char ignore_2;

 // Quick color hack
 IPalette pal;
 pal = IPaletteCreate();
 IPaletteCopy(pal, IPaletteMain);

 // Intialize everything
 bufScreen[0] = IScreenCreate();
 bufScreen[1] = IScreenCreate();
 IPaletteCopy(IPaletteMain, IImagePaletteGet(titlePic));

 IImageDraw(bufScreen[0], 0, 0, titlePic);
 IBoxDraw(bufScreen[0], 35, 45, 285, 151, 0);
 num = 0;
 c[1] = 0;
 for (y = 0; y < 4; y++)
 {
  for (x = 0; x < 19; x ++)
  {
   if (num < 10)
   {
    c[0] = '0' + num;
   }
   else if (num < 36)
   {
    c[0] = 'A' - 10 + num;
   }
   else if (num < 62)
   {
    c[0] = 'a' - 36 + num;
   }
   else
   {
    c[0] = 0;
   }
   ITextDraw(bufScreen[0], 49 + x * 12, 92 + y * 12, 255, c);
   num++;
  }
 }
 name[0] = 0;
 len = 0;
 x = y = 0;
 for (;;)
 {
  IScreenCopy(bufScreen[1], bufScreen[0]);
  ITextDraw(bufScreen[1], 128, 57, 255, name);
  IRectangleDraw(bufScreen[1], 47 + x * 12, 90 + y * 12, 47 + x * 12 + 11,
    90 + y * 12 + 11, TROLL_LIGHTGRAY);
  IRectangleDraw(bufScreen[1], 48 + x * 12, 91 + y * 12, 48 + x * 12 + 9,
    91 + y * 12 + 9, TROLL_LIGHTGRAY);
  IRectangleDraw(bufScreen[1], 51 + x * 12, 90 + y * 12, 51 + x * 12 + 3,
    91 + y * 12, 0);
  IRectangleDraw(bufScreen[1], 51 + x * 12, 100 + y * 12, 51 + x * 12 + 3,
    101 + y * 12, 0);
  IScreenCopy(IScreenMain, bufScreen[1]);
  IGraphicsRefresh();
  for (a = b = 0, oldD = d, oldR = r, num = 0;
    (!a) && (!b) && ((!d) || (d == oldD)) && ((!r) || (r == oldR));
    control->status(d, r, a, b, start, ignore_2), num++)
  {
   if (num == 5)
   {
    num = 0;
    oldR = 0;
    oldD = 0;
   }
   else
   {
    oldR = r;
    oldD = d;
   }
   if ((start) && (name[0]))
   {
    // Quick color hack
    IScreenClear(IScreenMain);
    IGraphicsRefresh();
    IPaletteCopy(IPaletteMain, pal);
    IPaletteDestroy(pal);

    // Free allocated memory
    IScreenDestroy(bufScreen[0]);
    IScreenDestroy(bufScreen[1]);
    return ;
   }
   ITimerWait();
  }
  if (d == -1)
  {
   if (y)
   {
    y--;
   }
   else
   {
    y = (x > 4) ? 2 : 3;
   }
  }
  else if (d == 1)
  {
   y++;
   y %= (x > 4) ? 3 : 4;
  }
  if (r == -1)
  {
   if (x)
   {
    x--;
   }
   else
   {
    x = (y == 3) ? 4 : 18;
   }
  }
  else if (r == 1)
  {
   x++;
   x %= (y == 3) ? 5 : 19;
  }
  if ((a) && (len < 8))
  {
   num = y * 19 + x;
   if (num < 10)
   {
    name[len] = '0' + num;
   }
   else if (num < 36)
   {
    name[len] = 'A' - 10 + num;
   }
   else if (num < 62)
   {
    name[len] = 'a' - 36 + num;
   }
   name[++len] = 0;
  }
  if ((b) && (len))
  {
   name[--len] = 0;
  }
 }
}

void TrollGame::setMusic(const std::string &m)
{
 if (music == m)
  return;
 if (musicObj)
 {
  Mix_HaltMusic();
  Mix_FreeMusic(musicObj);
  musicObj = NULL;
 }
 music = m;
 SDL_RWops *musicFile;
 char *file;

 file = buildFullPath(dataDir.c_str(), ("sounds/" + music).c_str());
 musicFile = SDL_RWFromFile(file, "rb");

 delete[] file;
 if (musicFile)
 {
  musicObj = Mix_LoadMUS_RW(musicFile, 0);
  if (musicObj)
   Mix_FadeInMusic(musicObj, -1, 1000);
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::titleScreen - Displays a simple little title screen until
    start is pressed.

    Parameters:
      name         (Out) Name of the character (already allocated max 8 chars)

    Returns: True if game should be started.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
bool TrollGame::titleScreen(char *name)
{
 IUShort top;
 IUShort where;
 IUShort turn;
 IScreen bufScreen;
 signed char d, oldD;
 unsigned char start;
 int i;
 int len = strlen(savePath);

 // Most of the data from the nes controller is ignored.
 signed char ignore_1;
 unsigned char ignore_2;

 // Troll sprite
 const Sprite *trollPic = TrollSpriteHandler.getSprite(TROLL_SPRITE);
 const Sprite *scrollbarPic =
   TrollSpriteHandler.getSprite(TROLL_SPRITE_SCROLLBAR);
#ifdef __linux__
 const Sprite *iconPic =
   TrollSpriteHandler.getSprite(TROLL_SPRITE_LINUX);
#elif defined(__MSDOS__)
 const Sprite *iconPic =
   TrollSpriteHandler.getSprite(TROLL_SPRITE_DOS);
#elif defined(_WIN32)
 const Sprite *iconPic =
   TrollSpriteHandler.getSprite(TROLL_SPRITE_DOS);
#endif

 // Quick color hack
 IPalette pal;
 pal = IPaletteCreate();
 IPaletteCopy(pal, IPaletteMain);

 // Intialize everything
 FileList files(savePath, TROLL_SAVE_EXT);
 top = 0;
 where = 0;
 bufScreen = IScreenCreate();
 IPaletteCopy(IPaletteMain, IImagePaletteGet(titlePic));

 setMusic("Boot_Up.ogg");
 // Run the title screen until start is pressed
 d = 0;
 for (;;)
 {
  IImageDraw(bufScreen, 0, 0, titlePic);
  IBoxDraw(bufScreen, 35, 45, 285, 151, 0);
  iconPic->draw(bufScreen, 315 - iconPic->getXSize(),
    195 - iconPic->getYSize(), 0, 0, 0);
  trollPic->draw(bufScreen, 40, where * 16 + 50, 0, TROLL_DOWN, 0);
  if (top)
  {
   scrollbarPic->draw(bufScreen, 270, 50, 0, TROLL_UP, 0);
  }
  if (top + 4 < files.length())
  {
   scrollbarPic->draw(bufScreen, 270, 136, 0, TROLL_DOWN, 0);
  }
  trollPic->draw(bufScreen, 40, where * 16 + 50, 0, TROLL_DOWN, 0);
  for (i = 0; i < 6; i++)
  {
   if (top + i == 0)
   {
    ITextDraw(bufScreen, 65, i * 16 + 54, 255, "New Character");
   }
   else if (top + i == files.length() + 1)
   {
    ITextDraw(bufScreen, 65, i * 16 + 54, 255, "Quit");
   }
   else if (top + i <= files.length())
   {
    memset(name, 0, 9);
    strncpy(name, files[top + i - 1] + len,
      strlen(files[top + i - 1] + len) - 4);
    ITextDraw(bufScreen, 65, i * 16 + 54, 255, name);
   }
  }
  IScreenCopy(IScreenMain, bufScreen);
  IGraphicsRefresh();
  for (start = 0, oldD = d, i = 0; (!start) && ((!d) || (d == oldD));
   control->status(d, ignore_1, ignore_2, ignore_2, start, ignore_2), i++)
  {
   if (i == 5)
   {
    oldD = 0;
   }
   else
   {
    oldD = d;
   }
   ITimerWait();
  }
  if (d == -1)
  {
   if ((where == 0) && (top))
   {
    top--;
   }
   else if (where == 0)
   {
    // go to bottom?
   }
   else
   {
    where--;
   }
  }
  else if (d == 1)
  {
   if (top + where == files.length() + 1)
   {
    // go to top?
   }
   else if (where == 5)
   {
    top++;
   }
   else
   {
    where++;
   }
  }
  if (start)
  {
   // Quick color hack
   IScreenClear(IScreenMain);
   IGraphicsRefresh();
   IPaletteCopy(IPaletteMain, pal);
   IPaletteDestroy(pal);

   // Free all allocated memory
   IScreenDestroy(bufScreen);

   if (top + where == 0)
   {
    selectName(name);
    return true;
   }
   else if (top + where <= files.length())
   {
    memset(name, 0, 9);
    strncpy(name, files[top + where - 1] + len,
      strlen(files[top + where - 1] + len) - 4);
    return true;
   }
   return false;
  }
 }
 // should never get here
 return false;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollGame::turnCleanUp - Do any necessary end of turn clean up.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollGame::turnCleanUp()
{
 if (extraScreen)
 {
  delete extraScreen;
  extraScreen = NULL;
 }
}

