#ifndef __SPRITE_H
#define __SPRITE_H
/*-------------------------------------------------------------------------*\
  <sprite.h> -- Sprite and SpriteHandler class

  Date      Programmer  Description
  03/20/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include <istdlib.h>
#include <igrbasics.h>
#include "file.h"

class Sprite
{
 public:
  Sprite(BinaryReadFile &spritefile);
  ~Sprite();
  void draw(IScreen drawscreen, IUShort x, IUShort y, IUShort frame,
    IUShort facing, IUByte shift) const;
  IUShort getFacings() const;
  IUShort getFrames() const;
  IUShort getXSize() const;
  IUShort getYSize() const;

 private:
  IUShort facings;
  IUShort frames;
  IUShort xsize, ysize;
  IUByte **pictures;
};

class SpriteHandler
{
 public:
  SpriteHandler(char *filename);
  ~SpriteHandler();
  IUShort getNumber() const;
  const Sprite *getSprite(IUShort n) const;

 private:
  IUShort num;
  Sprite **sprites;
};

#endif

