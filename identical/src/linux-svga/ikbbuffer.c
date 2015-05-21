/*-------------------------------------------------------------------------*\
  <ikbbuffer.c> -- Identical Software's Buffered Keyboard Routines
    (Linux SVGALIB version)

  Date      Programmer  Description
  03/22/97  Dennis      Created.
\*-------------------------------------------------------------------------*/
#include <ikbbuffer.h>
#include <vgakeyboard.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/* The buffer size must be one less than a multiple of two */
#define IBUFFERSIZE   15
#define ILEFTSHIFT    1
#define IRIGHTSHIFT   2
#define ILEFTCONTROL  4
#define IRIGHTCONTROL 8
#define ILEFTALT      16
#define IRIGHTALT     32

#ifndef SCANCODE_LEFTSHIFT
#define SCANCODE_LEFTSHIFT 42
#endif

#ifndef SCANCODE_RIGHTSHIFT
#define SCANCODE_RIGHTSHIFT 54
#endif

void IKeybufferHandler(int scan, int press);

unsigned char IKeybufferMap[4][110] = {
/*{ 0xFF, 0x1B, '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0' ,
    '-' , '=' , 0x08, 0x09, 'q' , 'w' , 'e' , 'r' , 't' , 'y' , 'u' , 'i' ,
    'o' , 'p' , '[' , ']' , 0x0D, 0xFF, 'a' , 's' , 'd' , 'f' , 'g' , 'h' ,
    'j' , 'k' , 'l' , ';' , '\'', '`' , 0xFF, '\\', 'z' , 'x' , 'c' , 'v' ,
    'b' , 'n' , 'm' , ',' , '.' , '/' , 0xFF, '*' , 0xFF, ' ' , 0xFF,  F1 ,
     F2 ,  F3 ,  F4 ,  F5 ,  F6 ,  F7 ,  F8 ,  F9 , F10 , 0xFF, 0xFF, HOME,
     UP , PGUP, '-' , LEFT, CNTR, RGHT, '+' , END , DOWN, PGDN, INS , DEL ,
    0xFF, 0xFF, 0xFF, F11 , F12 , 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x0D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  UP , 0xFF, LEFT, RGHT, 0xFF,
    DOWN, 0xFF }, */
 /* Normal */
 { 0xFF, 0x1B, '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , '0' ,
   '-' , '=' , 0x08, 0x09, 'q' , 'w' , 'e' , 'r' , 't' , 'y' , 'u' , 'i' ,
   'o' , 'p' , '[' , ']' , 0x0D, 0xFF, 'a' , 's' , 'd' , 'f' , 'g' , 'h' ,
   'j' , 'k' , 'l' , ';' , '\'', '`' , 0xFF, '\\', 'z' , 'x' , 'c' , 'v' ,
   'b' , 'n' , 'm' , ',' , '.' , '/' , 0xFF, 0xFF, 0xFF, ' ' , 0xFF, 0xAF,
   0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xFF, 0xFF, 0xBC,
   0xBD, 0xBE, '-' , 0xBF, 0xC0, 0xC1, '+' , 0xC2, 0xC3, 0xC4, 0xC5, 0xC6,
   0xFF, 0xFF, 0xFF, 0xB9, 0xBA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
   0x0D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBD, 0xFF, 0xBF, 0xC1, 0xFF,
   0xC3, 0xFF },
 /* Shift */
 { 0xFF, 0x1B, '!' , '@' , '#' , '$' , '%' , '^' , '&' , '*' , '(' , ')' ,
   '_' , '+' , 0x08, 0x09, 'Q' , 'W' , 'E' , 'R' , 'T' , 'Y' , 'U' , 'I' ,
   'O' , 'P' , '{' , '}' , 0x0D, 0xFF, 'A' , 'S' , 'D' , 'F' , 'G' , 'H' ,
   'J' , 'K' , 'L' , ':' , '"' , '~' , 0xFF, '|' , 'Z' , 'X' , 'C' , 'V' ,
   'B' , 'N' , 'M' , '<' , '>' , '?' , 0xFF, 0xFF, 0xFF, ' ' , 0xFF, 0xC7,
   0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xFF, 0xFF, 0xD3,
   0xD4, 0xD5, '-' , 0xD6, 0xD7, 0xD8, '+' , 0xD9, 0xDA, 0xDB, 0xDC, 0xDD,
   0xFF, 0xFF, 0xFF, 0xD1, 0xD2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
   0x0D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0xFF, 0xD6, 0xD8, 0xFF,
   0xDA, 0xFF },
 /* Control */
 { 0xFF, 0x1B, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88,
   0x89, 0x1F, 0x08, 0x09, 0x11, 0x17, 0x05, 0x12, 0x14, 0x19, 0x15, 0x09,
   0x0F, 0x10, 0x1E, 0x1D, 0x0D, 0xFF, 0x01, 0x13, 0x04, 0x06, 0x07, 0x08,
   0x0A, 0x0B, 0x0C, 0x8A, 0x1C, 0xFF, 0xFF, 0xFF, 0x1A, 0x18, 0x03, 0x16,
   0x02, 0x0E, 0x0D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, ' ' , 0xFF, 0xDE,
   0xDF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xFF, 0xFF, 0xEA,
   0xEB, 0xEC, '-' , 0xED, 0xEE, 0xEF, '+' , 0xF0, 0xF1, 0xF2, 0xF3, 0xF4,
   0xFF, 0xFF, 0xFF, 0xE8, 0xE9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
   0x0D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEB, 0xFF, 0xED, 0xEF, 0xFF,
   0xF1, 0xFF },
 /* Alt */
 { 0xFF, 0x1B, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
   0xFF, 0xFF, 0x08, 0x09, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C,
   0x9D, 0x9E, 0xFF, 0xFF, 0x0D, 0xFF, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4,
   0xA5, 0xA6, 0xA7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xA8, 0xA9, 0xAA, 0xAB,
   0xAC, 0xAD, 0xAE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, ' ' , 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF5,
   0xF6, 0xF7, '-' , 0xF8, 0xF9, 0xFA, '+' , 0xFB, 0xFC, 0xFD, 0xFF, 0xFF,
   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
   0x0D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF6, 0xFF, 0xF8, 0xFA, 0xFF,
   0xFC, 0xFF }
};

unsigned char IKeybufferBuffer[IBUFFERSIZE+1];
int IKeybufferFirst, IKeybufferLast;
int IKeybufferModify;

void IKeybufferStart()
{
 keyboard_init();
 keyboard_seteventhandler(IKeybufferHandler);
 IKeybufferFirst = IKeybufferLast = IKeybufferModify = 0;
}

unsigned char IKeybufferGet()
{
 fd_set fds;
 unsigned char c;

 /* The ALT - F# keys do not currently function under Linux */
 while (IKeybufferFirst == IKeybufferLast) {
  /* if no characters in the buffer wait until there are */
  /* keyboard_waitforUpdate(); */ /* not suggested in SVGALIB 1.2.9 */
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  select(STDIN_FILENO + 1, &fds, NULL, NULL, NULL);
  keyboard_update();
 }
 c = IKeybufferBuffer[IKeybufferFirst];
 IKeybufferFirst++;
 IKeybufferFirst &= IBUFFERSIZE;
 return c;
}

void IKeybufferHandler(int scan, int press)
{
 int mask;

 switch (scan) {
  case SCANCODE_LEFTCONTROL:
   mask = ILEFTCONTROL;
   break;
  case SCANCODE_RIGHTCONTROL:
   mask = IRIGHTCONTROL;
   break;
  case SCANCODE_LEFTALT:
   mask = ILEFTALT;
   break;
  case SCANCODE_RIGHTALT:
   mask = IRIGHTALT;
   break;
  case SCANCODE_LEFTSHIFT:
   mask = ILEFTSHIFT;
   break;
  case SCANCODE_RIGHTSHIFT:
   mask = IRIGHTSHIFT;
   break;
  default:
   if (press && (((IKeybufferFirst + 1) & IBUFFERSIZE) != IKeybufferLast)) {
    if ((IKeybufferModify & IRIGHTALT) || (IKeybufferModify & ILEFTALT)) {
     mask = 3;
    }
    else if ((IKeybufferModify & IRIGHTCONTROL) ||
             (IKeybufferModify & ILEFTCONTROL)) {
     mask = 2;
    }
    else if ((IKeybufferModify & IRIGHTSHIFT) ||
             (IKeybufferModify & ILEFTSHIFT)) {
     mask = 1;
    }
    else {
     mask = 0;
    }
    if ((scan < 110) && (IKeybufferMap[mask][scan] != 0xFF)) {
     IKeybufferBuffer[IKeybufferLast] = IKeybufferMap[mask][scan];
     IKeybufferLast++;
     IKeybufferLast &= IBUFFERSIZE;
    }
   }
   return ;
   break;
 }
 if (press) {
  IKeybufferModify |= mask;
 }
 else {
  IKeybufferModify &= ~mask;
 }
}

unsigned char IKeybufferPress()
{
 keyboard_update();
 return (IKeybufferFirst != IKeybufferLast);
}

void IKeybufferEnd()
{
 keyboard_update();
 keyboard_close();
}

