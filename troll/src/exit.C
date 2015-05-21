/*-------------------------------------------------------------------------*\
  <exit.C> -- Implemention for TrollExit methods

  Date      Programmer  Description
  04/17/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "troll.h"

TrollExit::TrollExit(BinaryReadFile &f)
{
 int num;

 // This should be dynamically allocated based on need.  As is it will crash
 // if level names are longer than 49 characters.
 levelName = new char[50];

 f.readUShort(x);
 f.readUShort(y);
 for (f.readByte((signed char &)levelName[num = 0]); levelName[num];
   f.readByte((signed char &)levelName[++num]))
 {
 }
 f.readUShort(newScreenX);
 f.readUShort(newScreenY);
 f.readUShort(newX);
 f.readUShort(newY);
}

TrollExit::TrollExit(IUShort ex, IUShort ey, char *lvlNm, IUShort nsx,
  IUShort nsy, IUShort nx, IUShort ny)
{
 x = ex;
 y = ey;
 levelName = new char[strlen(lvlNm) + 1];
 strcpy(levelName, lvlNm);
 newScreenX = nsx;
 newScreenY = nsy;
 newX = nx;
 newY = ny;
}

TrollExit::~TrollExit()
{
 delete [] levelName;
}

IUShort TrollExit::getX() const
{
 return(x);
}

IUShort TrollExit::getY() const
{
 return(y);
}

const char *TrollExit::getLevelName() const
{
 return(levelName);
}

IUShort TrollExit::getNewScreenX() const
{
 return(newScreenX);
}

IUShort TrollExit::getNewScreenY() const
{
 return(newScreenY);
}

IUShort TrollExit::getNewX() const
{
 return(newX);
}

IUShort TrollExit::getNewY() const
{
 return(newY);
}

