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

TrollExit::TrollExit(IUShort ex, IUShort ey, const char *lvlNm /*= NULL*/,
  IUShort nsx /*= 0*/, IUShort nsy /*= 0*/, IUShort nx /*= 0*/,
  IUShort ny /*= 0*/)
{
 x = ex;
 if (ey > TROLL_BUFFER_Y)
 {
  y = ey;
 }
 else
 {
  y = TROLL_BUFFER_Y;
 }
 if (NULL == lvlNm)
 {
  levelName = new char[1];
  levelName[0] = 0;
 }
 else
 {
  levelName = new char[strlen(lvlNm) + 1];
  strcpy(levelName, lvlNm);
 }
 newScreenX = nsx;
 newScreenY = nsy;
 newX = nx;
 if (ny > TROLL_BUFFER_Y)
 {
  newY = ny;
 }
 else
 {
  newY = TROLL_BUFFER_Y;
 }
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

void TrollExit::write(BinaryWriteFile &f)
{
 f.writeUShort(x);
 f.writeUShort(y);
 f.writeByteArray(strlen(levelName) + 1, (IByte *)levelName);
 f.writeUShort(newScreenX);
 f.writeUShort(newScreenY);
 f.writeUShort(newX);
 f.writeUShort(newY);
}

void TrollExit::setX(IUShort val)
{
 x = val;
}

void TrollExit::setY(IUShort val)
{
 y = val;
}

void TrollExit::setLevelName(const char *val)
{
 delete [] levelName;
 levelName = new char[strlen(val) + 1];
 strcpy(levelName, val);
}

void TrollExit::setNewScreenX(IUShort val)
{
 newScreenX = val;
}

void TrollExit::setNewScreenY(IUShort val)
{
 newScreenY = val;
}

void TrollExit::setNewX(IUShort val)
{
 newX = val;
}

void TrollExit::setNewY(IUShort val)
{
 newY = val;
}

