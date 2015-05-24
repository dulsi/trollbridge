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
 try
 {
  TrollGame game(argc, argv);

  game.run();
 }
 catch (const std::exception &e)
 {
  printf("%s\n", e.what());
 }
 return 0;
}

