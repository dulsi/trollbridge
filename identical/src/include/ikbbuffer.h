#ifndef __IKBBUFFER_H
#define __IKBBUFFER_H
/*-------------------------------------------------------------------------*\
  <ikbbuffer.h> -- Identical Software's Buffered Keyboard Routines

  Date      Programmer  Description
  03/22/97  Dennis      Created.
\*-------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IKeybufferStart - Initializes buffered keyboard mode.

    Note: Some systems (windowing systems in particular) may not be able to
  start the keyboard buffer until graphic initialization is done.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void IKeybufferStart();

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IKeybufferGet - Gets a key press (will wait until a key is pressed).

    Returns: The pressed key
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
unsigned char IKeybufferGet();

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IKeybufferPress - Checks to see if a key is pressed.

    Returns: 0 (no keys pressed) or 1 (key press available)
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
unsigned char IKeybufferPress();

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IKeybufferEnd - Shutsdown buffered keyboard mode.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
void IKeybufferEnd();                                                      */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Macros and Machine specific information
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if defined(__MSDOS__) || (defined(__linux__) && defined(SVGALIB))

void IKeybufferEnd();

#else

#define IKeybufferEnd()

#endif

#ifdef __cplusplus
}
#endif

#endif

