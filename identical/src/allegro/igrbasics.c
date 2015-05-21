/*-------------------------------------------------------------------------*\
  <igrbasics.c> -- Identical Software's Primitive Graphic Routines
    (ALLEGRO version)

  Date      Programmer  Description
  12/04/98  Dennis      Created.
\*-------------------------------------------------------------------------*/
#include <igrbasics.h>
#include <allegro.h>

IScreen IScreenMain;
IPalette IPaletteMain = NULL;
static BITMAP *buffer;

void IGraphicsStart(const char *name, IUShort xMult, IUShort yMult,
                    IBool fullScreen)
{
 unsigned long screen_base_addr;

 allegro_init();
 install_timer();
 set_gfx_mode(GFX_VGA, 320, 200, 0, 0);

 IPaletteMain = (IPalette)IScreenMain = IScreenCreate();
 buffer = create_bitmap(320, 200);
}

void IGraphicsEnd()
{
 set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
}

void IGraphicsRefresh()
{
 int i, j;

 for (i = 0; i < 200; i++)
  for (j = 0; j < 320; j++)
   _putpixel(buffer, j, i, IScreenMain[i * 320 + j]);
 blit(buffer, screen, 0, 0, 0, 0, 320, 200);
}

/*void IScreenClear(IScreen i_screen)
{
 int i;

 if (i_screen == IScreenMain)
 {
  clear(screen);
 }
 else
 {
  IMemset(i_screen, 0, 64000);
 }
}

void IScreenCopy(IScreen screendst, IScreen screensrc)
{
 int i, j;

 if (screendst == IScreenMain)
 {
  clear(buffer);
  for (i = 0; i < 200; i++)
   for (j = 0; j < 320; j++)
    _putpixel(buffer, j, i, screensrc[i * 320 + j]);
  blit(buffer, screen, 0, 0, 0, 0, 320, 200);
 }
 else if (screensrc == IScreenMain)
 {
  blit(screen, buffer, 0, 0, 0, 0, 320, 200);
  for (i = 0; i < 200; i++)
   for (j = 0; j < 320; j++)
    screendst[i * 320 + j] = getpixel(buffer, j, i);
 }
 else
 {
  IMemcpy(screendst, screensrc, 64000);
 }
}*/

void IPaletteCopy(IPalette paldst, IPalette palsrc)
{
 PALETTE allegroPal;
 int i;

 if (palsrc == IPaletteMain) {
  get_palette(allegroPal);
  for (i = 0; i < 256; i++)
  {
   (*paldst)[i][0] = allegroPal[i].r;
   (*paldst)[i][1] = allegroPal[i].g;
   (*paldst)[i][2] = allegroPal[i].b;
  }
 }
 else if (paldst == IPaletteMain) {
  for (i = 0; i < 256; i++)
  {
   allegroPal[i].r = (*palsrc)[i][0];
   allegroPal[i].g = (*palsrc)[i][1];
   allegroPal[i].b = (*palsrc)[i][2];
  }
  set_palette(allegroPal);
 }
 else {
  IMemcpy(paldst, palsrc, sizeof(IPaletteTable));
 }
}

void IPaletteSet(IPalette pal, IPixel c, IColor r, IColor g, IColor b)
{
 RGB allegroRgb;

 if (pal == IPaletteMain) {
  allegroRgb.r = r;
  allegroRgb.g = g;
  allegroRgb.b = b;
  vsync();
  set_color(c, &allegroRgb);
 }
 else {
  (*pal)[c][0] = r;
  (*pal)[c][1] = g;
  (*pal)[c][2] = b;
 }
}

void IPaletteGet(IPalette pal, IPixel c, IColor IFAR *r, IColor IFAR *g,
                 IColor IFAR *b)
{
 RGB allegroRgb;

 if (pal == IPaletteMain) {
  set_color(c, &allegroRgb);
  *r = allegroRgb.r;
  *g = allegroRgb.g;
  *b = allegroRgb.b;
 }
 else {
  *r = (*pal)[c][0];
  *g = (*pal)[c][1];
  *b = (*pal)[c][2];
 }
}

