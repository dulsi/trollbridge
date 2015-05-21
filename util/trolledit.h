#ifndef __TROLLEDIT_H

#include "troll.h"

class TrollEditSecret
{
 public:
  TrollEditSecret();
  TrollEditSecret(BinaryReadFile &f);
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
  TrollEditScreen();
  ~TrollEditScreen();
  void clear();
  void save();

  char *filename;
  IUShort backgroundPics[TROLL_SCREEN_X][TROLL_SCREEN_Y];
  IUByte backgroundShift[TROLL_SCREEN_X][TROLL_SCREEN_Y];
  IUShort passability[TROLL_SCREEN_X][TROLL_SCREEN_Y];
  IUShort exitNum;
  TrollExit **exits;
  IUShort permMonsterNum;
  IUShort *permMonsters;
  IUShort respawnMonsterNum;
  IUShort *respawnMonsters;
  IUShort secretNum;
  TrollEditSecret **secrets;
};

#endif

