#ifndef __IGRGEOMETRY_H
#define __IGRGEOMETRY_H
/*-------------------------------------------------------------------------*\
  <igrgeometry.h> -- Identical Software's Standard Geometry Routines

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
  ILineDraw - Draws a line to the screen.

    Parameters:
      screen       (In & Out) Screen to draw to
      x1           (In)  X starting coordinate
      y1           (In)  Y starting coordinate
      x2           (In)  X ending coordinate
      y2           (In)  Y ending coordinate
      c            (In)  Color
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void ILineDraw(IScreen screen, IUShort x1, IUShort y1, IUShort x2,
               IUShort y2, IPixel c);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  ICircleDraw - Draws a circle to the screen.

    Parameters:
      screen       (In & Out) Screen to draw to
      x            (In)  X coordinate of circle's center
      y            (In)  Y coordinate of circle's center
      r            (In)  Radius
      c            (In)  Color
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void ICircleDraw(IScreen screen, IUShort x, IUShort y, IUShort r, IPixel c);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IRectangleDraw - Draws a rectangle to the screen.

    Parameters:
      screen       (In & Out) Screen to draw to
      x1           (In)  X starting coordinate
      y1           (In)  Y starting coordinate
      x2           (In)  X ending coordinate
      y2           (In)  Y ending coordinate
      c            (In)  Color
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void IRectangleDraw(IScreen screen, IUShort x1, IUShort y1, IUShort x2,
                    IUShort y2, IPixel c);

#ifdef __cplusplus
}
#endif

#endif

