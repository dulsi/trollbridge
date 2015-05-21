#ifndef __IGRBASICS_H
#define __IGRBASICS_H
/*-------------------------------------------------------------------------*\
  <igrbasics.h> -- Identical Software's Primitive Graphic Routines

  Date      Programmer  Description
  02/28/97  Dennis      Created.
\*-------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include <istdlib.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  New Types
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef unsigned char IPixel;
typedef unsigned char IColor;
typedef IColor IPaletteTable[256][3];
typedef IPaletteTable IFAR *IPalette;
typedef IPixel IFAR *IScreen;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Global variables
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
extern IScreen IScreenMain;
extern IPalette IPaletteMain;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IGraphicsStart - Initializes graphics mode.

    Parameters:
      name         (In)  Name of the program (displays as window title)
      xMult        (In)  X multiplier
      yMult        (In)  Y multiplier
      fullScreen   (In)  Use full screen
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void IGraphicsStart(const char *name, IUShort xMult, IUShort yMult,
                    IBool fullScreen);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IGraphicsRefresh - Refreshes of the screen

    Note: Some systems (windowing systems in particular) may not
  automatically redraw screen changes and require a refresh.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
void IGraphicsRefresh();                                                   */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IGraphicsEnd - Shutsdown the graphics screen.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void IGraphicsEnd();

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IScreenCreate - Creates a virtual screen.

    Returns: The new virtual screen
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
IScreen IScreenCreate();                                                   */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IScreenClear - Clears a virtual screen.

    Parameters:
      screen       (In)  Virtual screen to be cleared
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
void IScreenClear(IScreen screen);                                         */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IScreenCopy - Copy a virtual screen to another one.

    Parameters:
      screendst    (Out) Destination virtual screen
      screensrc    (In)  Source virtual screen
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
void IScreenCopy(IScreen screendst, const IScreen screensrc);              */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IScreenDestroy - Destroys a virtual screen.

    Parameters:
      screen       (In)  Virtual screen to be destroyed
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
void IScreenDestroy(IScreen screen);                                       */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IPaletteCreate - Creates a palette.

    Returns: The new palette
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
IPalette IPaletteCreate();                                                 */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IPaletteCopy - Copy one palette to another.

    Parameters:
      paldst       (Out) Destination palette
      palsrc       (In)  Source palette
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void IPaletteCopy(IPalette paldst, const IPalette palsrc);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IPaletteSet - Sets a palette entry.

    Parameters:
      pal          (In & Out)  The palette
      c            (In)  Color to change
      r            (In)  New red value
      g            (In)  New green value
      b            (In)  New blue value
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void IPaletteSet(IPalette pal, IPixel c, IColor r, IColor g, IColor b);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IPaletteGet - Gets a palette entry.

    Parameters:
      pal          (In)  The palette
      c            (In)  Color
      r            (Out) Red value
      g            (Out) Green value
      b            (Out) Blue value
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void IPaletteGet(const IPalette pal, IPixel c, IColor IFAR *r,
                 IColor IFAR *g, IColor IFAR *b);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IPaletteDestroy - Destroys a palette.

    Parameters:
      pal          (In)  Palette to be destroyed
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
void IPaletteDestroy(IPalette screen);                                     */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IPixelCapture - Get a pixel value from the screen.

    Parameters:
      screen       (In) Screen to read from
      x            (In)  X coordinate
      y            (In)  Y coordinate
    Returns: The value of the requested position
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
IPixel IPixelCapture(const IScreen screen, IUShort x, IUShort y);          */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IPixelDraw - Draws a pixel on the screen.

    Parameters:
      screen       (In & Out) Screen to write to
      x            (In)  X coordinate
      y            (In)  Y coordinate
      c            (In)  New color
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
void IPixelDraw(IScreen screen, IUShort x, IUShort y, IPixel c);           */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Macros and Machine specific information
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if (defined(__MSDOS__) && !defined(ALLEGRO)) || (defined(__linux__) && defined(SVGALIB))

#define IGraphicsRefresh()

#else

void IGraphicsRefresh();

#endif

#if defined(__MSDOS__) && !defined(ALLEGRO)

void IScreenClear(IScreen screen);
void IScreenCopy(IScreen screendst, IScreen screensrc);

#else

#define IScreenClear(screen)                                                \
 IMemset(screen, 0, 64000)

#define IScreenCopy(screendst, screensrc)                                   \
 IMemcpy(screendst, screensrc, 64000)

#endif

#define IScreenCreate()                                                     \
 (IScreen)IMalloc(64000)

#define IScreenDestroy(screen)                                              \
 IFree(screen)

#define IPaletteCreate()                                                    \
 (IPalette)IMalloc(sizeof(IPaletteTable))

#define IPaletteDestroy(pal)                                                \
 IFree(pal)

#define IPixelCapture(screen, x, y)                                         \
 screen[x + ((y) << 8) + ((y) << 6)]

#define IPixelDraw(screen, x, y, c)                                         \
 screen[x + ((y) << 8) + ((y) << 6)] = c

#ifdef __cplusplus
}
#endif

#endif

