/*-------------------------------------------------------------------------*\
  <kbbufexm.c> -- Example of Identical Software's Keyboard Buffer Routines

  Date      Programmer  Description
  02/08/98  Dennis      Created.
\*-------------------------------------------------------------------------*/
#include <igrbasics.h>
#include <ikbbuffer.h>
#include <igrtext.h>

void ITestTextDraw();

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  main - Main body of the program.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
int main()
{
 /* Initialize graphics and keyboard */
 IGraphicsStart("Basic Example", 1, 1, IFALSE);
 IKeybufferStart();

 /* Setup palette */
 IPaletteSet(IPaletteMain, 0, 0, 0, 0);
 IPaletteSet(IPaletteMain, 255, 255, 255, 255);

 /* Run drawing text test */
 ITestTextDraw();

 /* Shutdown graphics and keyboard */
 IKeybufferEnd();
 IGraphicsEnd();
 return 0;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  ITestTextDraw - Draws whatever the use types.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void ITestTextDraw()
{
 IUShort x;
 unsigned char key[2];

 x = 0;
 key[1] = 0;
 /* Continue getting keyboard input until ESC is hit */
 while ((key[0] = IKeybufferGet()) != 0x1B)
 {
  if (key[0] == 0x0D)
  {
   /* Clear the screen if return is pressed */
   x = 0;
   IScreenClear(IScreenMain);
  }
  else if ((isalnum(key[0])) && (x < 310))
  {
   /* Draw all alphanumeric character */
   ITextDraw(IScreenMain, x, 50, 255, key);
   x += 10;
  }
  else if (key[0] == ' ')
  {
   /* Increment drawing location if a space is pressed */
   x += 10;
  }
  IGraphicsRefresh();
 }
}

