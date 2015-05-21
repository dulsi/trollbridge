#ifndef __IGRTEXT_H
#define __IGRTEXT_H
/*-------------------------------------------------------------------------*\
  <igrtext.h> -- Identical Software's Basic Text Writing

  Date      Programmer  Description
  04/05/97  Dennis      Created.
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
  ITextDraw - Draws a string on the screen.

    Parameters:
      screen       (In & Out) Screen to write to
      x            (In)  X coordinate
      y            (In)  Y coordinate
      c            (In)  Color
      s            (In)  String
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void ITextDraw(IScreen screen, IUShort x, IUShort y, IPixel c,
               const unsigned char *s);


#ifdef __cplusplus
}
#endif

#endif

