/*-------------------------------------------------------------------------*\
  <secret.C> -- Implemention for TrollSecret methods

  Date      Programmer  Description
  04/17/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSecret::TrollSecret - Constructor for a secret.

    Parameters:
      gm           (In)  Game
      scr          (In)  Screen
      f            (In)  A file pointing to where the secret's information is
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollSecret::TrollSecret(TrollGame *gm, TrollScreen *scr, BinaryReadFile &f)
{
 // Note: Currently the size of the scripts are not remembered.  This
 // prevents bounds checking and therefore may be a bad idea.
 IUShort size;

 game = gm;
 screen = scr;
 // read in the number of the secret
 f.readUShort(number);
 // read in the unset script
 f.readUShort(size);
 if (size != 0)
 {
  unsetScript = new IUByte[size];
  f.readUByteArray(size, unsetScript);
 }
 else
 {
  unsetScript = NULL;
 }
 // read in the set script
 f.readUShort(size);
 if (size != 0)
 {
  setScript = new IUByte[size];
  f.readUByteArray(size, setScript);
 }
 else
 {
  setScript = NULL;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSecret::~TrollSecret - Destructor for a secret.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
TrollSecret::~TrollSecret()
{
 if (setScript)
 {
  delete [] setScript;
 }
 if (unsetScript)
 {
  delete [] unsetScript;
 }
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSecret::executeSet - Executes the set script.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollSecret::executeSet()
{
 execute(setScript);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSecret::executeUnset - Executes the unset script.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollSecret::executeUnset()
{
 execute(unsetScript);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSecret::getNumber - Gets the number of the secret.

    Returns: The number of the secret
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IUShort TrollSecret::getNumber()
{
 return(number);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  TrollSecret::execute - Runs a script.

    Parameters:
      script       (In)  Script to run
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void TrollSecret::execute(IUByte *script)
{
 // Note: This code casts IUByte* into IUShort*.  This will cause
 // misalignment which generates a bus error on some machines.

 // Note: The IUShorts are also still in little endian.

 IUByte *ip; // Instruction pointer
 IUShort done;
 IUShort i;

 if (script)
 {
  done = 0;
  ip = script;
  while (!done)
  {
   switch (*ip)
   {
    case 0x00: // end of secret
     done = 1;
     break;
    case 0x01: // create monster
     screen->addMonster(
       (game->createMonster(*((IUShort *)(ip + 1))))(screen,
         TROLL_XYRANDOM, TROLL_XYRANDOM, *((IUShort *)(ip + 3))));
     ip += 4;
     break;
    case 0x02: // create item
     screen->addItem(
       (game->createItem(*((IUShort *)(ip + 1))))(screen,
         *((IUShort *)(ip + 3)),*((IUShort *)(ip + 5)),*((IUShort *)(ip + 7))));
     ip += 8;
     break;
    case 0x03: // set background
     screen->setBackground(*((IUShort *)(ip + 1)), *((IUShort *)(ip + 3)),
       *((IUShort *)(ip + 5)), *((IUByte *)(ip + 7)));
     screen->setPassability(*((IUShort *)(ip + 1)), *((IUShort *)(ip + 3)),
       *((IUShort *)(ip + 8)));
     ip += 9;
     break;
    case 0x04: // create exit
     i = strlen((char *)(ip + 5)) + 1;
     screen->addExit(new TrollExit(*((IUShort *)(ip + 1)),
       *((IUShort *)(ip + 3)), (char *)(ip + 5),
       *((IUShort *)(ip + 5 + i)), *((IUShort *)(ip + 7 + i)),
       *((IUShort *)(ip + 9 + i)), *((IUShort *)(ip + 11 + i))));
     ip += 12 + i;
     break;
    case 0x05: // set text
     i = strlen((char *)(ip + 1)) + 1;
     screen->setText((char *)(ip + 1), (char *)(ip + i + 1));
     ip += i + strlen((char *)(ip + i + 1)) + 1;
     break;
    case 0x06: // create monster with position
     screen->addMonster(
       (game->createMonster(*((IUShort *)(ip + 1))))(screen,
         *((IUShort *)(ip + 3)), *((IUShort *)(ip + 5)),
         *((IUShort *)(ip + 7))));
     ip += 8;
     break;
    default: // unknown instruction
     break;
   }
   ip++;
  }
 }
}

