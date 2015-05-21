/*-------------------------------------------------------------------------*\
  <game.C> -- Implemention for TrollGame methods

  Date      Programmer  Description
  04/17/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "troll.h"

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Globals
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
SpriteHandler TrollSpriteHandler("troll.spr");

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  initpal - Loads the Troll Bridge palette and sets the display to use it.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void initpal()
{
 IPalette pal;
 FILE *palfile;

 if ((palfile = fopen("troll.pal", "rb"))==NULL) {
  exit(2);
 }
 pal = IPaletteCreate();
 fread(pal,1,3*256,palfile);
 fclose(palfile);
 IPaletteSet(pal, 255, 255, 255, 255);
 IPaletteCopy(IPaletteMain, pal);
 IPaletteDestroy(pal);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  main - Main body of the program.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
int main(int argc, char **argv)
{
 IGraphicsStart("Troll Bridge");
 initpal();

 TrollGame game(argc, argv);

 game.run();
 IGraphicsEnd();
 return 0;
}

