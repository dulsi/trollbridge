/*-------------------------------------------------------------------------*\
  <igrgeometry.c> -- Identical Software's Standard Geometry Routines

  Date      Programmer  Description
  02/28/97  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include <istdlib.h>
#include <igrbasics.h>
#include <igrgeometry.h>

void ILineDraw(IScreen screen, IUShort x1, IUShort y1, IUShort x2,
               IUShort y2, IPixel c)
{
 /* Bresenham's line algorithm (assembly version for dos should be written) */
 IShort dx, dy, incr;
 int d;

 dx = abs(x2 - x1);
 dy = abs(y2 - y1);
 if (dx > dy) {
  d = (dy << 1) - dx;
  incr = (y1 < y2) ? 1 : -1;
  if (x2 < x1) {
   y1 = y2;
   y2 = x1;
   x1 = x2;
   x2 = y2;
  }
  for (; x1 <= x2; x1++) {
   IPixelDraw(screen, x1, y1, c);
   if (d < 0) {
    d += (dy << 1);
   }
   else {
    d += ((dy - dx) << 1);
    y1 += incr;
   }
  }
 }
 else {
  d = (dx << 1) - dy;
  incr = (x1 < x2) ? 1 : -1;
  if (y2 < y1) {
   x1 = x2;
   x2 = y1;
   y1 = y2;
   y2 = x2;
  }
  for (; y1 <= y2; y1++) {
   IPixelDraw(screen, x1, y1, c);
   if (d < 0) {
    d += (dx << 1);
   }
   else {
    d += ((dx - dy) << 1);
    x1 += incr;
   }
  }
 }
}

void ICircleDraw(IScreen screen, IUShort x, IUShort y, IUShort r, IPixel c)
{
 /* Bresenham's circle algorithm */
 IUShort dx, dy;
 int d;

 d = 3 - (r << 1);
 dy = r;
 for (dx = 0; dx <= dy; dx++) {
  IPixelDraw(screen, x + dx, y + dy, c);
  IPixelDraw(screen, x + dx, y - dy, c);
  IPixelDraw(screen, x - dx, y + dy, c);
  IPixelDraw(screen, x - dx, y - dy, c);
  IPixelDraw(screen, x + dy, y + dx, c);
  IPixelDraw(screen, x + dy, y - dx, c);
  IPixelDraw(screen, x - dy, y + dx, c);
  IPixelDraw(screen, x - dy, y - dx, c);
  if (d < 0) {
   d += (dx << 2) +6;
  }
  else {
   d += ((dx - dy) << 2) + 10;
   dy--;
  }
 }
}

void IRectangleDraw(IScreen screen, IUShort x1, IUShort y1, IUShort x2,
                    IUShort y2, IPixel c)
{
 IUShort i;

 if (x2 < x1) {
  i = x2;
  x2 = x1;
  x1 = i;
 }
 if (y2 < y1) {
  i = y2;
  y2 = y1;
  y1 = i;
 }
 IMemset(screen + x1 + (y1 << 8) + (y1 << 6), c, x2 - x1 + 1);
 IMemset(screen + x1 + (y2 << 8) + (y2 << 6), c, x2 - x1 + 1);
 for (i = y1; i < y2; i++) {
  IPixelDraw(screen, x1, i, c);
  IPixelDraw(screen, x2, i, c);
 }
}

