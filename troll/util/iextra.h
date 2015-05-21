#ifndef __IEXTRA_H
#define __IEXTRA_H
/*-------------------------------------------------------------------------*\
  <iextra.h> -- Extra Routines for Identical Software's Library

  Date      Programmer  Description
  02/01/00  Dennis      Created.
\*-------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include <istdlib.h>
#include <igrbasics.h>
#include <igrimage.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  New Types
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef unsigned char IFAR *IPaletteNameEntry;
typedef IPaletteNameEntry IPaletteNameTable[256];
typedef IPaletteNameTable IFAR *IPaletteName;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IImageSubCapture - Gets an image from the image.

    Parameters:
      img          (In)  Image
      x1           (In)  X coordinate of upper-left corner
      y1           (In)  Y coordinate of upper-left corner
      x1           (In)  X coordinate of lower-right corner
      y1           (In)  Y coordinate of lower-right corner
    Returns: The requested image
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IImage IImageSubCapture(IImage img, IUShort x1, IUShort y1, IUShort x2,
                        IUShort y2);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IImageTextLoad - Load an image from a text file.

    Parameters:
      filename     (In)  Name (and path) of the text file
      pal          (In)  Palette for the image
      palnm        (In)  Names for the palette
    Returns: The loaded image
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IImage IImageTextLoad(const char *filename, IPalette pal, IPaletteName palnm);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IImageTextSave - Save an image from a text file.

    Parameters:
      img          (In)  Image
      filename     (In)  Name (and path) of the text file
      pal          (In)  Palette from a text palette file
      palnm        (In)  Names for the palette
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void IImageTextSave(IImage img, const char *filename, IPalette pal,
                    IPaletteName palnm);

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
  Macros and Machine specific information
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define IPaletteNameGet(palnm, color)                                       \
  (*palnm)[color]

#ifdef __cplusplus
}
#endif

#endif

