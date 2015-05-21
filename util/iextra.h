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
#include <igrpalette.h>

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

#ifdef __cplusplus
}
#endif

#endif

