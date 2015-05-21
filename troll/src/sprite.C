#include <string.h>
#include "sprite.h"

#define IHorizontalLineDraw(screen, x1, x2, y, clr) \
  memset((screen) + ((y) * 320) + (x1), (clr), (x2) - (x1) + 1);

Sprite::Sprite(BinaryReadFile &spritefile)
{
 IUShort *szpic;
 int i;

 spritefile.readUShort(xsize);
 spritefile.readUShort(ysize);
 spritefile.readUShort(facings);
 spritefile.readUShort(frames);
 szpic = (IUShort *)malloc(sizeof(IUShort) * facings * frames);
 pictures = (IUByte **)malloc(sizeof(IUByte *) * facings * frames);
 spritefile.readUShortArray(facings * frames, szpic);
 for (i = 0; i < facings * frames; i++)
 {
  pictures[i] = (IUByte *)malloc(sizeof(IUByte) * szpic[i]);
  spritefile.readUByteArray(szpic[i], pictures[i]);
 }
 free(szpic);
}

Sprite::~Sprite()
{
 int i;

 for (i = 0; i < facings * frames; i++)
 {
  free(pictures[i]);
 }
 free(pictures);
}

void Sprite::draw(IScreen drawscreen, IUShort x, IUShort y, IUShort frame,
    IUShort facing, IUByte shift) const
{
 IShort i, j, k, l, where;

 where = 0;
 for (i = 0; i < ysize; i++)
 {
  where++;
  if ((y + i < 0) || (y + i > 199))
   where+=(pictures[facing*frames+frame])[where-1];
  else
  {
   for (j = 0; j < xsize; where++)
   {
    if (((pictures[facing * frames + frame])[where] != 255) &&
      ((pictures[facing * frames + frame])[where] & 128))
    {
     k = ((pictures[facing * frames + frame])[where] & 0x7F) + 1;
     l = 0;
     if (x + j < 0)
     {
      k -= (x + j) * -1;
      if (k > 0)
      {
       j = -x;
      }
      else
      {
       j += ((pictures[facing * frames + frame])[where] & 0x7F) + 1;
       k = 0;
      }
     }
     else if (x + j + k > 320)
     {
      k = 320 - x - j;
      l = ((pictures[facing * frames + frame])[where] & 0x7F) + 1 - k;
     }
     if ((k > 0) && ((pictures[facing * frames + frame])[where + 1] != 255))
     {
      IHorizontalLineDraw(drawscreen, x + j, x + j + k - 1, y + i,
        (pictures[facing * frames + frame])[where + 1] + shift);
     }
     j += k + l;
     where++;
    }
    else if (!((x + j < 0) || (x + j > 319)))
    {
     if ((pictures[facing * frames + frame])[where] != 255)
     {
      IPixelDraw(drawscreen, x + j, y + i,
        (pictures[facing * frames + frame])[where] + shift);
     }
     j++;
    }
    else j++;
   }
  }
 }
}

IUShort Sprite::getFacings() const
{
 return facings;
}

IUShort Sprite::getFrames() const
{
 return frames;
}

IUShort Sprite::getXSize() const
{
 return xsize;
}

IUShort Sprite::getYSize() const
{
 return ysize;
}

SpriteHandler::SpriteHandler(char *filename)
{
 int i;
 BinaryReadFile f(filename);

 f.readUShort(num);
 sprites = (Sprite **)malloc(num * sizeof(Sprite *));
 for (i = 0; i < num; i++)
 {
  sprites[i] = new Sprite(f);
 }
}

SpriteHandler::~SpriteHandler()
{
 int i;

 for (i = 0; i < num; i++)
 {
  delete sprites[i];
 }
 free(sprites);
}

IUShort SpriteHandler::getNumber() const
{
 return num;
}

const Sprite *SpriteHandler::getSprite(IUShort n) const
{
 if (n < num)
 {
  return(sprites[n]);
 }
 else
 {
  return((Sprite *)0);
 }
}

