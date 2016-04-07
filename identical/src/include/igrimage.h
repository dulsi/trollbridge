#ifndef __IGRIMAGE_H
#define __IGRIMAGE_H
/*-------------------------------------------------------------------------*\
  <igrimage.h> -- Identical Software's Image Routines

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
#include <igrbasics.h>

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  New Types
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef struct IImageStruct {
 IUShort x, y;
 IPixel IFAR *pic;
 IPalette pal;
} IFAR *IImage;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IImageCapture - Gets an image from the screen.

    Parameters:
      screen       (In) Screen to read from
      x1           (In)  X coordinate of upper-left corner
      y1           (In)  Y coordinate of upper-left corner
      x1           (In)  X coordinate of lower-right corner
      y1           (In)  Y coordinate of lower-right corner
    Returns: The requested image
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IImage IImageCapture(const IScreen screen, IUShort x1, IUShort y1, IUShort x2,
                     IUShort y2);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IImageDraw - Draws an image to the screen.

    Parameters:
      screen       (In & Out) Screen to draw to
      x            (In)  X coordinate
      y            (In)  Y coordinate
      img          (In)  Image to draw
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void IImageDraw(IScreen screen, IUShort x, IUShort y, IImage img);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IImagePaletteGet - Gets the palette used for the image.

    Parameters:
      img          (In)  Image
    Returns: The palette used for the image (Modifications do affect the
  image)
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
const IPalette IImagePaletteGet(const IImage img);                         */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IImagePaletteSet - Sets the palette used for the image.

    Parameters:
      img          (In & Out)  Image
      pal          (In)  New palette
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void IImagePaletteSet(IImage img, const IPalette pal);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IImageLoad - Load an image from a file.

    Parameters:
      filename     (In)  Name (and path) of the pcx file
    Returns: The loaded image
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IImage IImageLoad(const char *filename);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IImagePCXLoad - Load an image from a PCX file.

    Parameters:
      filename     (In)  Name (and path) of the pcx file
    Returns: The loaded image
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IImage IImagePCXLoad(const char *filename);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IImagePCXSave - Save an image to a PCX file.

    Parameters:
      img          (In)  Image
      filename     (In)  Name (and path) of the pcx file
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void IImagePCXSave(IImage img, const char *filename);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IImageDestroy - Destroys an image.

    Parameters:
      img          (In)  Image to be destroyed
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void IImageDestroy(IImage img);


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Macros and Machine specific information
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define IImagePaletteGet(x)                                                  \
  (const IPalette)(x)->pal

#ifdef __cplusplus
}
#endif

#endif

