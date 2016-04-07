#include "trolledit.h"

TrollEditSecret::TrollEditSecret()
{
 secretNum = 0;
 unsetScriptSize = 0;
 unsetScript = (IUByte *)IMalloc(0);
 setScriptSize = 0;
 setScript = (IUByte *)IMalloc(0);
}

TrollEditSecret::TrollEditSecret(BinaryReadFile &f)
{
 f.readUShort(secretNum);
 f.readUShort(unsetScriptSize);
 unsetScript = (IUByte *)IMalloc(unsetScriptSize);
 f.readUByteArray(unsetScriptSize, unsetScript);
 f.readUShort(setScriptSize);
 setScript = (IUByte *)IMalloc(setScriptSize);
 f.readUByteArray(setScriptSize, setScript);
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
 f.readUShort(exitNum);
 exits = (TrollExit **)malloc(exitNum * sizeof(TrollExit *));
 for (i = 0; i < exitNum; i++)
 {
  exits[i] = new TrollExit(f);
 }
 f.readUShort(permMonsterNum);
 permMonsters = (IUShort *)malloc(permMonsterNum * sizeof(IUShort));
 f.readUShortArray(permMonsterNum, permMonsters);
 f.readUShort(respawnMonsterNum);
 respawnMonsters = (IUShort *)malloc(respawnMonsterNum * sizeof(IUShort));
 f.readUShortArray(respawnMonsterNum, respawnMonsters);
 f.readUShort(secretNum);
 secrets = (TrollEditSecret **)malloc(secretNum * sizeof(TrollEditSecret *));
 for (i = 0; i < secretNum; i++)
 {
  secrets[i] = new TrollEditSecret(f);
 }
}

TrollEditScreen::TrollEditScreen()
{
 filename = strdup("");
 memset(backgroundPics, TROLL_SPRITE_CLEAR, TROLL_SCREEN_X * TROLL_SCREEN_Y * sizeof(IUShort));
 memset(backgroundShift, 0, TROLL_SCREEN_X * TROLL_SCREEN_Y * sizeof(IUByte));
 memset(passability, 0, TROLL_SCREEN_X * TROLL_SCREEN_Y * sizeof(IUShort));
 exitNum = 0;
 exits = (TrollExit **)malloc(exitNum * sizeof(TrollExit *));
 permMonsterNum = 0;
 permMonsters = (IUShort *)malloc(permMonsterNum * sizeof(IUShort));
 respawnMonsterNum = 0;
 respawnMonsters = (IUShort *)malloc(respawnMonsterNum * sizeof(IUShort));
 secretNum = 0;
 secrets = (TrollEditSecret **)malloc(secretNum * sizeof(TrollEditSecret *));
}

TrollEditScreen::~TrollEditScreen()
{
 int i;

 free(filename);
 for (i = 0; i < exitNum; i++)
 {
  delete exits[i];
 }
 free(exits);
 free(permMonsters);
 free(respawnMonsters);
 for (i = 0; i < secretNum; i++)
 {
  delete secrets[i];
 }
 free(secrets);
}

void TrollEditScreen::clear()
{
 int i;

 free(filename);
 for (i = 0; i < exitNum; i++)
 {
  delete exits[i];
 }
 free(exits);
 for (i = 0; i < secretNum; i++)
 {
  delete secrets[i];
 }
 free(secrets);
 filename = strdup("");
 memset(backgroundPics, TROLL_SPRITE_CLEAR, TROLL_SCREEN_X * TROLL_SCREEN_Y * sizeof(IUShort));
 memset(backgroundShift, 0, TROLL_SCREEN_X * TROLL_SCREEN_Y * sizeof(IUByte));
 memset(passability, 0, TROLL_SCREEN_X * TROLL_SCREEN_Y * sizeof(IUShort));
 exitNum = 0;
 exits = (TrollExit **)malloc(exitNum * sizeof(TrollExit *));
 permMonsterNum = 0;
 permMonsters = (IUShort *)malloc(permMonsterNum * sizeof(IUShort));
 respawnMonsterNum = 0;
 respawnMonsters = (IUShort *)malloc(respawnMonsterNum * sizeof(IUShort));
 secretNum = 0;
 secrets = (TrollEditSecret **)malloc(secretNum * sizeof(TrollEditSecret *));
}

void TrollEditScreen::save()
{
 int i, num;

 BinaryWriteFile f(filename);
 f.writeUShortArray(TROLL_SCREEN_X * TROLL_SCREEN_Y, (IUShort *)backgroundPics);
 f.writeUByteArray(TROLL_SCREEN_X * TROLL_SCREEN_Y, (IUByte *)backgroundShift);
 f.writeUShortArray(TROLL_SCREEN_X * TROLL_SCREEN_Y, (IUShort *)passability);
 f.writeUShort(exitNum);
 for (i = 0; i < exitNum; i++)
 {
  exits[i]->write(f);
 }
 f.writeUShort(permMonsterNum);
 f.writeUShortArray(permMonsterNum, permMonsters);
 f.writeUShort(respawnMonsterNum);
 f.writeUShortArray(respawnMonsterNum, respawnMonsters);
 f.writeUShort(secretNum);
 for (i = 0; i < secretNum; i++)
 {
  f.writeUShort(secrets[i]->secretNum);
  f.writeUShort(secrets[i]->unsetScriptSize);
  f.writeUByteArray(secrets[i]->unsetScriptSize, secrets[i]->unsetScript);
  f.writeUShort(secrets[i]->setScriptSize);
  f.writeUByteArray(secrets[i]->setScriptSize, secrets[i]->setScript);
 }
}

