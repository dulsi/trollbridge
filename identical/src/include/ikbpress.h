#ifndef __IKBPRESS_H
#define __IKBPRESS_H
/*-------------------------------------------------------------------------*\
  <ikbpress.h> -- Identical Software's Pressed Keyboard Routines

  Date      Programmer  Description
  04/12/97  Dennis      Created.
\*-------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IKeypressStart - Initializes pressed keyboard mode.

    Note: Some systems (windowing systems in particular) may not be able to
  start the keyboard buffer until graphic initialization is done.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void IKeypressStart();

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IKeypressGet - Gets a key status.

    Parameters:
      key          (In)  Key to be checked
    Returns: 0 (not pressed), 1 (currently pressed), 2 (not pressed but has
  been since last check), 3 (currently pressed and has been pressed since
  last check)
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
unsigned char IKeypressGet(unsigned char key);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  IKeypressEnd - Shutsdown pressed keyboard mode.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
void IKeybufferEnd();                                                      */


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Macros and Machine specific information
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#if (defined(__MSDOS)__) || (defined(__linux__) && defined(SVGALIB))

void IKeybufferEnd();

#else

#define IKeybufferEnd()

#endif

#ifdef __cplusplus
}
#endif

#endif

