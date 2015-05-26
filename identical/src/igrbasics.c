/*-------------------------------------------------------------------------*\
  <igrbasics.c> -- Identical Software's Primitive Graphic Routines
    (SDL version)

  Date      Programmer  Description
  10/09/99  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* Note: The SDL support for Identical Software's Graphics Library was
intended learn some of the library before the LokiHack contest.  As such
the code is not very efficient. */

#include <igrbasics.h>
#include <SDL.h>
#include <stdio.h>

#define MIN(x,y) (((x)<(y))?(x):(y))

IScreen IScreenMain;
IPalette IPaletteMain = NULL;
IUShort IXMult = 0;
IUShort IYMult = 0;

/* Extra global variables needed by SDL */
SDL_Surface *ISDLScreen;

void IGraphicsStart(const char *name, IUShort xMult, IUShort yMult,
                    IBool fullScreen)
{
 if (SDL_Init(SDL_INIT_VIDEO) < 0)
 {
  printf("Failed - SDL_Init\n");
  exit(0);
 }
 const SDL_VideoInfo *info = SDL_GetVideoInfo();
 int xFull = info->current_w;
 int yFull = info->current_h;
 if ((xMult == 0) || (yMult == 0))
 {
  xMult = (xFull - 10) / 320; // Allow for window decoration
  yMult = (yFull - 10) / 200; // Allow for window decoration
  if (xMult > yMult)
   xMult = yMult;
  else
   yMult = xMult;
 }
 IXMult = xMult;
 IYMult = yMult;
 ISDLScreen = SDL_SetVideoMode(320 * xMult, 200 * yMult, 8,
   SDL_SWSURFACE | (fullScreen ? SDL_FULLSCREEN : 0));
 if (ISDLScreen == NULL)
 {
  printf("Failed - SDL_SetVideoMode\n");
  exit(0);
 }
 SDL_WM_SetCaption(name, name);
 if ((1 == xMult) && (1 == yMult))
  IScreenMain = (IScreen)ISDLScreen->pixels;
 else
  IScreenMain = (IScreen)malloc(320 * 200 * sizeof(IPixel));
 IPaletteMain = (IPalette)ISDLScreen->format->palette;
}

void IGraphicsRefresh()
{
 if ((1 != IXMult) || (1 != IYMult))
 {
  int x, y, xMult, yMult;
  IPixel IFAR *curPos;
  IPixel IFAR *prevLine;
  IPixel IFAR *realPos;

  SDL_LockSurface(ISDLScreen);
  curPos = IScreenMain;
  realPos = ISDLScreen->pixels;
  for (y = 0; y < 200; ++y)
  {
   prevLine = curPos;
   for (yMult = 0; yMult < IYMult; ++yMult)
   {
    curPos = prevLine;
    for (x = 0; x < 320; ++x)
    {
     for (xMult = 0; xMult < IXMult; ++xMult)
     {
      *realPos = *curPos;
      ++realPos;
     }
     ++curPos;
    }
   }
  }
  SDL_UnlockSurface(ISDLScreen);
 }
 SDL_UpdateRect(ISDLScreen, 0, 0, 0, 0);
}

void IGraphicsEnd()
{
 if ((1 != IXMult) || (1 != IYMult))
  free(IScreenMain);
 SDL_Quit();
}

void IPaletteCopy(IPalette paldst, const IPalette palsrc)
{
 SDL_Color *sdlcolors;
 SDL_Color sdlcol[256];
 int color, rgb;

 if (palsrc == IPaletteMain)
 {
  sdlcolors = ISDLScreen->format->palette->colors;
  for (color = 0; color < 256; color++)
  {
   (*paldst)[color][0] = sdlcolors[color].r >> 2;
   (*paldst)[color][1] = sdlcolors[color].g >> 2;
   (*paldst)[color][2] = sdlcolors[color].b >> 2;
  }
 }
 else if (paldst == IPaletteMain) {
  for (color = 0; color < 256; color++)
  {
   sdlcol[color].r = (*palsrc)[color][0] << 2;
   sdlcol[color].g = (*palsrc)[color][1] << 2;
   sdlcol[color].b = (*palsrc)[color][2] << 2;
  }
  SDL_SetColors(ISDLScreen, sdlcol, 0, 256);
 }
 else
 {
  IMemcpy(paldst, palsrc, sizeof(IPaletteTable));
 }
}

void IPaletteSet(IPalette pal, IPixel c, IColor r, IColor g, IColor b)
{
 SDL_Color sdlcol;

 if (pal == IPaletteMain)
 {
  sdlcol.r = r << 2;
  sdlcol.g = g << 2;
  sdlcol.b = b << 2;
  SDL_SetColors(ISDLScreen, &sdlcol, c, 1);
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
 SDL_Color *sdlcolors;

 if (pal == IPaletteMain)
 {
  sdlcolors = ISDLScreen->format->palette->colors;
  *r = sdlcolors[c].r >> 2;
  *g = sdlcolors[c].g >> 2;
  *b = sdlcolors[c].b >> 2;
 }
 else {
  *r = (*pal)[c][0];
  *g = (*pal)[c][1];
  *b = (*pal)[c][2];
 }
}

