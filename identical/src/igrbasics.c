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
IUShort IXReal = 0;
IUShort IYReal = 0;

/* Extra global variables needed by SDL */
SDL_Window *ISDLMainWindow;
SDL_Renderer *ISDLMainRenderer;
SDL_Texture *ISDLMainTexture;
SDL_Surface *ISDLMainScreen;
SDL_Surface *ISDLScreen;

void IGraphicsStart(const char *name, IUShort xMult, IUShort yMult,
                    IBool fullScreen, IBool soft)
{
 if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
 {
  printf("Failed - SDL_Init\n");
  exit(0);
 }
 SDL_DisplayMode info;
 int success = SDL_GetCurrentDisplayMode(0, &info);
 if (success != 0)
 {
  printf("%s\n", SDL_GetError());
  exit(0);
 }
 int xFull = info.w;
 int yFull = info.h;
 if (fullScreen && soft)
 {
  IXReal = xFull;
  IYReal = yFull;
 }
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
 ISDLMainWindow = SDL_CreateWindow(name,
                           SDL_WINDOWPOS_UNDEFINED,
                           SDL_WINDOWPOS_UNDEFINED,
                           ((fullScreen && soft) ? IXReal : 320 * xMult), ((fullScreen && soft) ? IYReal : 200 * yMult),
                           (fullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));
 if (ISDLMainWindow == NULL)
 {
  printf("Failed - SDL_CreateWindow\n");
  exit(0);
 }

 ISDLMainRenderer = SDL_CreateRenderer(ISDLMainWindow, -1, ((fullScreen && soft) ? SDL_RENDERER_SOFTWARE : 0));
 if (ISDLMainRenderer == NULL)
 {
  printf("Failed - SDL_CreateRenderer\n");
  exit(0);
 }
 ISDLMainTexture = SDL_CreateTexture(ISDLMainRenderer,
                             SDL_PIXELFORMAT_ARGB8888,
                             SDL_TEXTUREACCESS_STREAMING,
                             ((fullScreen && soft) ? IXReal : 320 * xMult), ((fullScreen && soft) ? IYReal : 200 * yMult));
 if (ISDLMainTexture == NULL)
 {
  printf("Failed - SDL_CreateTexture\n");
  exit(0);
 }
 ISDLMainScreen = SDL_CreateRGBSurface(0, 320 * xMult, 200 * yMult, 32,
                                        0x00FF0000,
                                        0x0000FF00,
                                        0x000000FF,
                                        0xFF000000);
 ISDLScreen = SDL_CreateRGBSurface(0, 320 * xMult, 200 * yMult, 8, 0, 0, 0, 0);
 if (ISDLScreen == NULL)
 {
  printf("Failed - SDL_CreateRGBSurface\n");
  exit(0);
 }
 if ((1 == xMult) && (1 == yMult))
  IScreenMain = (IScreen)ISDLScreen->pixels;
 else
  IScreenMain = (IScreen)malloc(320 * 200 * sizeof(IPixel));
 IPaletteMain = (IPalette)ISDLScreen->format->palette;
}

void IGraphicsRefresh()
{
 void *pixels;
 int pitch;
 SDL_Rect dest;
 dest.x = 0;
 dest.y = 0;
 if (IXReal)
 {
  dest.w = IXReal;
  dest.h = IYReal;
 }
 else
 {
  dest.w = 320 * IXMult;
  dest.h = 200 * IYMult;
 }
 SDL_LockTexture(ISDLMainTexture, &dest, &pixels, &pitch);
 if (IXReal)
 {
  double scaleWidth =  320.0 / (double)IXReal;
  double scaleHeight = 200.0 / (double)IYReal;
  double curX = 0;
  double curY = 0;
  Uint8 *realLine;
  Uint8 *realPos;
  int cy;
  int cx;

  realPos = (Uint8 *)pixels;
  for (cy = 0; cy < IYReal; cy++, curY += scaleHeight)
  {
   curX = 0;
   realLine = realPos;
   for (cx = 0; cx < IXReal; cx++, curX += scaleWidth)
   {
    SDL_GetRGB(IScreenMain[((int)curY) * 320 + ((int)curX)], ISDLScreen->format, realPos + 2, realPos + 1, realPos);
    realPos += 4;
   }
   realPos = realLine + pitch;
  }
 }
 else
 {
  int x, y, xMult, yMult;
  IPixel IFAR *curPos;
  IPixel IFAR *prevLine;
  Uint8 *realLine;
  Uint8 *realPos;

  curPos = IScreenMain;
  realPos = (Uint8 *)pixels;
  for (y = 0; y < 200; ++y)
  {
   prevLine = curPos;
   for (yMult = 0; yMult < IYMult; ++yMult)
   {
    curPos = prevLine;
    realLine = realPos;
    for (x = 0; x < 320; ++x)
    {
     for (xMult = 0; xMult < IXMult; ++xMult)
     {
      SDL_GetRGB(*curPos, ISDLScreen->format, realPos + 2, realPos + 1, realPos);
      realPos += 4;
     }
     ++curPos;
    }
    realPos = realLine + pitch;
   }
  }
 }
 SDL_UnlockTexture(ISDLMainTexture);
 SDL_RenderClear(ISDLMainRenderer);
 SDL_RenderCopy(ISDLMainRenderer, ISDLMainTexture, NULL, NULL);
 SDL_RenderPresent(ISDLMainRenderer);
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
  SDL_SetPaletteColors((SDL_Palette *)IPaletteMain, sdlcol, 0, 256);
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
  SDL_SetPaletteColors((SDL_Palette *)IPaletteMain, &sdlcol, c, 1);
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

