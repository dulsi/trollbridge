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
SpriteHandler TrollSpriteHandler;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  main - Main body of the program.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
int main(int argc, char **argv)
{
 IGraphicsStart("Troll Bridge");

 TrollGame game(argc, argv);

 game.run();
 IGraphicsEnd();
 return 0;
}

