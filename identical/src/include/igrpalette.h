#ifndef __IGRPALETTE_H
#define __IGRPALETTE_H
/*-------------------------------------------------------------------------*\
  <igrpalette.h> -- Palette Routines for Identical Software's Library

  Date      Programmer  Description
  01/01/01  Dennis      Created.
\*-------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include <istdlib.h>
#include <igrbasics.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  New Types
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef unsigned char IFAR *IPaletteNameEntry;
typedef IPaletteNameEntry IPaletteNameTable[256];
typedef IPaletteNameTable IFAR *IPaletteName;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IPaletteFind - Finds a color in the palette.

    Parameters:
      pal          (In)  The palette
      r            (In)  Red value of the color
      g            (In)  Green value of the color
      b            (In)  Blue value of the color
    Returns: The position of the requested color or 255 if the color is not
  found
    Note: Does not currently support IPaletteMain.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IPixel IPaletteFind(const IPalette pal, IColor r, IColor g, IColor b);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IPaletteNameCreate - Creates a palette name.

    Returns: The new palette name
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IPaletteName IPaletteNameCreate();

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IPaletteNameGet - Gets a palette name entry.

    Parameters:
      palnm        (In)  The palette name
      color        (In)  Color
    Returns: Palette name of the color
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
const IPaletteNameEntry IPaletteNameGet(const IPaletteName palnm,
                                        IPixel color);                     */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IPaletteNameFind - Finds a color in the palette name.

    Parameters:
      palnm        (In)  The palette name
      name         (In)  Name to find
    Returns: The position of the requested color or 255 if the color is not
  found
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IPixel IPaletteNameFind(const IPaletteName palnm,
                        const IPaletteNameEntry name);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IPaletteNameDestroy - Destroys a palette name.

    Parameters:
      palnm        (In)  Palette name to be destroyed
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void IPaletteNameDestroy(IPaletteName palnm);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IPalettePalLoad - Load a palette file.

    Parameters:
      filename     (In)  Name (and path) of the palette file
    Returns: The palette
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IPalette IPalettePalLoad(const char *filename);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IPalettePalSave - Saves a palette file.

    Parameters:
      pal          (In)  Palette
      filename     (In)  Name (and path) of the palette file
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void IPalettePalSave(IPalette pal, const char *filename);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IPaletteTextLoad - Load a text palette file.

    Parameters:
      pal          (Out) The palette
      palnm        (Out) The palette name
      filename     (In)  Name (and path) of the text file
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void IPaletteTextLoad(IPalette *pal, IPaletteName *palnm,
                      const char *filename);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Macros and Machine specific information
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define IPaletteNameGet(palnm, color)                                       \
  (*palnm)[color]

#ifdef __cplusplus
}
#endif

#endif

