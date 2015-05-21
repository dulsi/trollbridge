#ifndef __IGRTIM_H
#define __IGRTIM_H
/*-------------------------------------------------------------------------*\
  <igrtim.h> -- Identical Software's Tim Routines

  Date      Programmer  Description
  10/23/98  Dennis      Created.
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
  Defines
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define ITIM_PAL_NONE 9999

#define ITIM_4BPP  0
#define ITIM_8BPP  1
#define ITIM_16BPP 2
#define ITIM_24BPP 3

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  New Types
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef struct ITimStruct {
 ILong flag;
 IUShort px, py, pw, ph;
 IUShort IFAR *pic;
 IUShort cx, cy, cw, ch;
 IUShort IFAR *clut;
} IFAR * ITim;


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IImage2Tim - Converts an image to a TIM.

    Parameters:
      img          (In)  Image
      bpp          (In)  Bits per pixel to use
      px           (In)  X position of TIM in video memory
      py           (In)  Y position of TIM in video memory
      cx           (In)  X position of color table in video memory
      cy           (In)  Y position of color table in video memory
      transparent  (In)  Set if a transparent color added
      r            (In)  Red value of tranparent color
      g            (In)  Green value of transparent color
      b            (In)  Blue value of transparent color
    Returns: The tim
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
ITim IImage2Tim(IImage img, IUShort bpp, IUShort px, IUShort py, IUShort cx,
  IUShort cy, IBool transparent, IColor r, IColor g, IColor b);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  ITim2Image - Converts a TIM to an image.

    Parameters:
      tim          (In)  Tim
      palnum       (In)  Palette number to use
      r            (In)  Red value of tranparent color
      g            (In)  Green value of transparent color
      b            (In)  Blue value of transparent color
    Returns: The image
    Note: Since images are only 8 bits per pixel a 24 or 16 bits per pixel
  tims cannot be converted at this time.  A palnum of ITIM_PAL_NONE will
  create an image without a palette.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
IImage ITim2Image(ITim tim, IUShort palnum, IColor r, IColor g, IColor b);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  ITimIs24bit - Tells if the TIM is in 24 bits per pixel mode.

    Parameters:
      tim          (In)  Tim
    Returns: True if the tim is 24 bits per pixel.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
IBool ITimIs24bit(ITim tim);                                               */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  ITimIs16bit - Tells if the TIM is in 16 bits per pixel mode.

    Parameters:
      tim          (In)  Tim
    Returns: True if the tim is 16 bits per pixel.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
IBool ITimIs16bit(ITim tim);                                               */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  ITimIs8bit - Tells if the TIM is in 8 bits per pixel mode.

    Parameters:
      tim          (In)  Tim
    Returns: True if the tim is 8 bits per pixel.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
IBool ITimIs8bit(ITim tim);                                                */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  ITimIs4bit - Tells if the TIM is in 4 bits per pixel mode.

    Parameters:
      tim          (In)  Tim
    Returns: True if the tim is 4 bits per pixel.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
IBool ITimIs4bit(ITim tim);                                                */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  ITimLoad - Load a TIM file.

    Parameters:
      filename     (In)  Name (and path) of the tim file
    Returns: The loaded tim
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
ITim ITimLoad(const char *filename);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  ITimSave - Save a TIM to a file.

    Parameters:
      tim          (In)  Tim
      filename     (In)  Name (and path) of the tim file
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void ITimSave(ITim tim, const char *filename);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  ITimDestroy - Destroys a tim.

    Parameters:
      tim          (In)  Tim to be destroyed
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void ITimDestroy(ITim tim);


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Macros and Machine specific information
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define ITimIs24bit(tim)                                                    \
 (((tim)->flag & 0x00000003) == ITIM_24BPP)

#define ITimIs16bit(tim)                                                    \
 (((tim)->flag & 0x00000003) == ITIM_16BPP)

#define ITimIs8bit(tim)                                                     \
 (((tim)->flag & 0x00000003) == ITIM_8BPP)

#define ITimIs4bit(tim)                                                     \
 (((tim)->flag & 0x00000003) == ITIM_4BPP)

#ifdef __cplusplus
}
#endif

#endif

