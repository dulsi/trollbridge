/*-------------------------------------------------------------------------*\
  <basicexm.c> -- Example of Identical Software's Basic Graphic Routines

  Date      Programmer  Description
  03/22/97  Dennis      Created.
\*-------------------------------------------------------------------------*/
#include <igrbasics.h>
#include <ikbbuffer.h>
#include <itimer.h>

void ITestScreen();
void ITestPaletteDisplay();
void ITestPaletteCycle();

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  main - Main body of the program.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
int main()
{
 /* Initialize graphics and keyboard */
 IGraphicsStart("Basic Example");
 IKeybufferStart();

 /* Run virtual screen tests */
 ITestScreen();

 /* Display the palette */
 ITestPaletteDisplay();

 /* Cycle the palette */
 ITestPaletteCycle();

 /* Shutdown graphics and keyboard */
 IKeybufferEnd();
 IGraphicsEnd();
 return 0;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  ITestScreen - Moves a line on the screen using a virtual screen.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void ITestScreen()
{
 IScreen s;
 int start, loop;

 /* First create the virtual screen */
 s = IScreenCreate();
 start = 0;
 while (!IKeybufferPress()) {
  /* Clear the virtual screen */
  IScreenClear(s);
  /* Draw a line across the virtual screen */
  for (loop = 0; loop < 200; loop++) {
   IPixelDraw(s, ((start + loop) % 320), loop, 5);
  }
  /* Copy the virtual screen to the real screen */
  IScreenCopy(IScreenMain, s);
  /* Some systems don't automatically refresh the screen */
  IGraphicsRefresh();
  /* Increment the lines starting position (wrapping if necessary) */
  start++;
  if (start == 320) {
   start = 0;
  }
 }
 IKeybufferGet();
 /* Always remember to destroy virtual screens when done with them */
 IScreenDestroy(s);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  ITestPaletteDisplay - Draws a box of every color.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void ITestPaletteDisplay()
{
 IScreen s;
 IUShort x,y;
 IPixel pix;

 s = IScreenCreate();
 IScreenClear(s);
 /* Draw boxes for all pixel colors (except 0 because the screen is cleared
   to 0) */
 for (pix = 1; pix; pix++) {
  /* Two loops to draw a 10x10 box of color pix */
  for (y = 0 ; y < 10; y++) {
   for (x = 0 ; x < 10; x++) {
    IPixelDraw(s, x + (pix % 30 * 10), y + (pix / 30 * 10), pix);
   }
  }
 }
 IScreenCopy(IScreenMain, s);
 /* Some systems don't automatically refresh the screen */
 IGraphicsRefresh();
 IScreenDestroy(s);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  ITestPaletteCycle - Cycles the palette.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void ITestPaletteCycle()
{
 IPalette pal;
 IColor r, g, b, orgr, orgg, orgb;
 IPixel pixcolor;

 /* Slows the computer to 20 ticks per second */
 ITimerStart(20);
 /* Create a virtual palette */
 pal = IPaletteCreate();
 /* Copy the real palette to the virtual one */
 IPaletteCopy(pal, IPaletteMain);
 while (!IKeybufferPress()) {
  /* Wait two clock ticks (bug in DOS timer code prevents 10 tps) */
  ITimerWait();
  ITimerWait();
  /* Shift all the colors one farther in the virtual palette */
  IPaletteGet(pal, 255, &orgr, &orgg, &orgb);
  for (pixcolor = 255; pixcolor; pixcolor--) {
   IPaletteGet(pal, pixcolor - 1, &r, &g, &b);
   IPaletteSet(pal, pixcolor, r, g, b);
  }
  IPaletteSet(pal, 0, orgr, orgg, orgb);
  /* Copy the virtual palette to the real one */
  IPaletteCopy(IPaletteMain, pal);
 }
 /* Always remember to destroy virtual palettes when finished with them */
 IPaletteDestroy(pal);
 /* Shutdown timer */
 ITimerEnd();
}

