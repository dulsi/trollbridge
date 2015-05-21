#ifndef __TROLLCONST_H
#define __TROLLCONST_H
/*-------------------------------------------------------------------------*\
  <trollconst.h> -- Constants for Troll Bridge

  Date      Programmer  Description
  04/17/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* Size of the levels */
#define TROLL_LEVEL_Y 8
#define TROLL_LEVEL_X 16

/* Size of the screens */
#define TROLL_SCREEN_Y 10
#define TROLL_SCREEN_X 16

/* Buffer zone at the top of the screen for character status */
#define TROLL_BUFFER_Y 40
/* Size of the squares */
#define TROLL_SQUARE_Y 16
#define TROLL_SQUARE_X 20

/* Sizes of various fixed sized arrays for class TrollGame */
#define TROLL_MONSTER_TYPES 22
#define TROLL_ITEM_TYPES    13
#define TROLL_CARRIED_ITEMS 6

/* Size of the secret array.  Since each secret is one bit, there are eight
 times as many secrets as the array size. */
#define TROLL_SECRETS_SIZE 20

/* Number of different types of carried items */
#define TROLL_CHARACTER_CARRIED_ITEMS 5

/* Array indices into TrollCharacter's screens array */
#define TROLL_BACKGROUND_SCREEN 0
#define TROLL_TEMPORARY_SCREEN1 1
#define TROLL_TEMPORARY_SCREEN2 2

/* Passibility constants [not fully implement] */
#define TROLL_OPEN      0
#define TROLL_NOMONSTER 1
#define TROLL_WATER     2
#define TROLL_BLOCKED   3

/* Direction constants */
#define TROLL_UP    0
#define TROLL_DOWN  1
#define TROLL_LEFT  2
#define TROLL_RIGHT 3

/* Time length for various computer controlled actions */
#define TROLL_INVINCIBLE_TIME 15
#define TROLL_FLYBACK_TIME    6

/* Randomly generate X and / or Y position */
#define TROLL_XYRANDOM 65000

/* Troll sprite picture */
#define TROLL_SPRITE 28
#define TROLL_SPRITE_SCROLLBAR 25
#define TROLL_SPRITE_DOS 26
#define TROLL_SPRITE_LINUX 27

/* Some colors for status display */
#define TROLL_LIGHTGRAY 24
#define TROLL_DARKGRAY  25

/* Save file extension */
#define TROLL_SAVE_EXT ".trs"

// Macros to calculate draw position from the square position
#define TROLL_CALCULATE_X_POS(x) ((x) * TROLL_SQUARE_X)
#define TROLL_CALCULATE_Y_POS(y) ((y) * TROLL_SQUARE_Y + TROLL_BUFFER_Y)

// Note: The graphics library is still missing some functionality.  This
// macro is a quick fix.
#define IBoxDraw(screen, x1, y1, x2, y2, clr) \
  for (int ii = (y1); ii <= (y2); ii++) \
    memset((screen) + (ii * 320) + (x1), (clr), (x2) - (x1) + 1);

#endif

