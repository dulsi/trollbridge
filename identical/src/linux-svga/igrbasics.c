/*-------------------------------------------------------------------------*\
  <igrbasics.c> -- Identical Software's Primitive Graphic Routines
    (Linux SVGALIB version)

  Date      Programmer  Description
  02/28/97  Dennis      Created.
\*-------------------------------------------------------------------------*/
#include <igrbasics.h>
#include <vga.h>
#include <termios.h>
#include <stdlib.h>

IScreen IScreenMain;
IPalette IPaletteMain = NULL;

void IGraphicsStart(const char *name, IUShort xMult, IUShort yMult,
                    IBool fullScreen)
{
 vga_init();
 vga_setmode(G320x200x256);
 IPaletteMain = (IPalette)IScreenMain = (IScreen)vga_getgraphmem();
}

void IGraphicsEnd()
{
 vga_setmode(TEXT);
}

void IPaletteCopy(IPalette paldst, const IPalette palsrc)
{
 int pal[256][3];
 int color, rgb;

 if (palsrc == IPaletteMain) {
  vga_getpalvec(0, 256, (int *)pal);
  for (color = 0; color < 256; color++) {
   for (rgb = 0; rgb < 3; rgb++) {
    (*paldst)[color][rgb] = pal[color][rgb];
   }
  }
 }
 else if (paldst == IPaletteMain) {
  for (color = 0; color < 256; color++) {
   for (rgb = 0; rgb < 3; rgb++) {
    pal[color][rgb] = (*palsrc)[color][rgb];
   }
  }
  vga_waitretrace();
  vga_setpalvec(0, 256, (int *)pal);
 }
 else {
  IMemcpy(paldst, palsrc, sizeof(IPaletteTable));
 }
}

void IPaletteSet(IPalette pal, IPixel c, IColor r, IColor g, IColor b)
{
 if (pal == IPaletteMain) {
  vga_waitretrace();
  vga_setpalette(c, r, g, b);
 }
 else {
  (*pal)[c][0] = r;
  (*pal)[c][1] = g;
  (*pal)[c][2] = b;
 }
}

void IPaletteGet(const IPalette pal, IPixel c, IColor IFAR *r, IColor IFAR *g,
                 IColor IFAR *b)
{
 int ir, ig, ib;

 if (pal == IPaletteMain) {
  vga_getpalette(c, &ir, &ig, &ib);
  *r = ir;
  *g = ig;
  *b = ib;
 }
 else {
  *r = (*pal)[c][0];
  *g = (*pal)[c][1];
  *b = (*pal)[c][2];
 }
}

