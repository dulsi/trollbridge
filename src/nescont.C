/********************************************************************
  <nescont.C> -- routines for dos and linux to emulate an nes controller

  Programmer -- Dennis Payne

  Date -- 5/26/96

  Modification History

  08/04/96 Added X window support.

  08/19/96 Added joystick support for linux.

  08/20/96 Added BIOS joystick support for dos.

  10/10/99 Added SDL support
********************************************************************/

#include "nescont.h"

#ifdef SDLLIB

#include <SDL.h>

SDL_Joystick *js = NULL;
unsigned int jsmask = 0;

#endif

#ifdef __MSDOS__

#ifdef ALLEGRO

#include <allegro.h>

#else

#include <dos.h>

#define INTR 0x15

#endif

#elif defined(__linux__)

#ifdef JOYSTICK

#include <linux/joystick.h>
#include <fcntl.h>
#include <unistd.h>

static struct JS_DATA_TYPE js;
static int jsdev;

#endif
#ifdef SVGALIB

#include <vgakeyboard.h>

#elif defined(XLIB)

#include <X11/Xlib.h>
#include <X11/keysym.h>

extern Display *IXDisplay;
extern Window IXWindow;

#endif
#endif

static unsigned short status_bits;
static unsigned short kbstatus_bits;
static unsigned short jsstatus_bits;

#ifdef SDLLIB

nes_controller::nes_controller()
{
 if ((SDL_InitSubSystem(SDL_INIT_JOYSTICK) == 0) && (SDL_NumJoysticks() > 0))
 {
  SDL_JoystickEventState(SDL_ENABLE);
  SDL_EventState(SDL_JOYBALLMOTION, SDL_IGNORE);
  SDL_EventState(SDL_JOYHATMOTION, SDL_IGNORE);
  js = SDL_JoystickOpen(0);
  jsstatus_bits = 0;
  if (js)
  {
   switch (SDL_JoystickNumButtons(js))
   {
    case 0:
     jsmask = 0x00FF;
     break;
    case 1:
     jsmask = 0x00FE;
     break;
    case 2:
     jsmask = 0x00FC;
     break;
    case 3:
     jsmask = 0x00F8;
     break;
    default:
     jsmask = 0x00F0;
     break;
   }
  }
 }
 SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
 SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
 SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
 SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
 SDL_EventState(SDL_QUIT, SDL_IGNORE);
 SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
}

nes_controller::~nes_controller()
{
 if (js)
 {
  SDL_JoystickClose(js);
  js = NULL;
  jsstatus_bits = 0;
 }
}

void nes_controller::status(signed char &down,signed char &right,
  unsigned char &a,unsigned char &b,unsigned char &start,
  unsigned char &select)
{
 SDL_Event sdlevent;
 int avail;
 unsigned short mask;
 unsigned short usedb;

 for (avail = SDL_PollEvent(&sdlevent); avail;
   avail = SDL_PollEvent(&sdlevent))
 {
  switch (sdlevent.type)
  {
   case SDL_KEYDOWN:
   case SDL_KEYUP:
    switch (sdlevent.key.keysym.sym)
    {
     case SDLK_KP8:
     case SDLK_UP:
      mask=0x0200;
      break;
     case SDLK_KP6:
     case SDLK_RIGHT:
      mask=0x0400;
      break;
     case SDLK_KP4:
     case SDLK_LEFT:
      mask=0x0800;
      break;
     case SDLK_KP2:
     case SDLK_DOWN:
      mask=0x0100;
      break;
     case SDLK_RETURN:
      mask=0x0008;
      break;
     case SDLK_LSHIFT:
     case SDLK_RSHIFT:
      mask=0x0004;
      break;
     case SDLK_RCTRL:
     case SDLK_LCTRL:
      mask=0x0001;
      break;
     case SDLK_RALT:
     case SDLK_LALT:
      mask=0x0002;
      break;
     default:
      mask=0;
      break;
    }
    if (sdlevent.key.state == SDL_PRESSED) kbstatus_bits|=mask;
    else kbstatus_bits&=(~mask);
    break;
   case SDL_JOYAXISMOTION:
    if (0 == sdlevent.jaxis.which)
    {
      if (0 == sdlevent.jaxis.axis)
      {
       jsstatus_bits &= ~0x0C00;
       if (sdlevent.jaxis.value > 1000)
        jsstatus_bits |= 0x0400;
       else if (sdlevent.jaxis.value < -1000)
        jsstatus_bits |= 0x0800;
      }
      else if (1 == sdlevent.jaxis.axis)
      {
       jsstatus_bits &= ~0x0300;
       if (sdlevent.jaxis.value > 1000)
        jsstatus_bits |= 0x0100;
       else if (sdlevent.jaxis.value < -1000)
        jsstatus_bits |= 0x0200;
      }
    }
    break;
   case SDL_JOYBUTTONDOWN:
   case SDL_JOYBUTTONUP:
    if (0 == sdlevent.jbutton.which)
    {
     switch (sdlevent.jbutton.button)
     {
      case 0:
       mask = 0x0001;
       break;
      case 1:
       mask = 0x0002;
       break;
      case 2:
      case 8:
       mask = 0x0004;
       break;
      case 3:
      case 9:
       mask = 0x0008;
       break;
      default:
       mask = 0;
       break;
     }
     if (sdlevent.jbutton.state == SDL_PRESSED)
     {
      jsstatus_bits |= mask;
     }
     else
     {
      jsstatus_bits &= ~mask;
     }
    }
    break;
   default:
    break;
  }
 }
 if (jsstatus_bits)
  status_bits=jsstatus_bits+(kbstatus_bits & jsmask);
 else 
  status_bits=kbstatus_bits;
 usedb=(status_bits & 0x000F)<<4;
 a=(status_bits & 0x0010)?0:(status_bits & 0x0001);
 b=(status_bits & 0x0020)?0:(status_bits & 0x0002);
 select=(status_bits & 0x0040)?0:(status_bits & 0x0004);
 start=(status_bits & 0x0080)?0:(status_bits & 0x0008);
 down=(status_bits & 0x0100)?1:((status_bits &0x0200)?-1:0);
 right=(status_bits & 0x0400)?1:((status_bits &0x0800)?-1:0);
 kbstatus_bits&=0xFF0F;
 kbstatus_bits|=usedb;
}

#elif defined(__MSDOS__)

#ifdef ALLEGRO

nes_controller::nes_controller()
{
 install_keyboard();
}

nes_controller::~nes_controller()
{
 remove_keyboard();
}

void nes_controller::status(signed char &down,signed char &right,
  unsigned char &a,unsigned char &b,unsigned char &start,
  unsigned char &select)
{
 unsigned short usedb;

#ifdef JOYSTICK
 unsigned short jsx,jsy;
 unsigned char jsbuttons;

 asm{
  mov ah,84h
  mov dx,0000h
  int 61h
  mov jsbuttons,al
  mov ah,84h
  mov dx,0001h
  int 61h
  mov jsx,ax
  mov jsy,bx
 }
 jsstatus_bits=0;
 if (js.buttons & 16) jsstatus_bits|=0x0002;
 if (js.buttons & 32) jsstatus_bits|=0x0001;
 if (js.x<30) jsstatus_bits|=0x0800;
 if (js.x>70) jsstatus_bits|=0x0400;
 if (js.y<30) jsstatus_bits|=0x0200;
 if (js.y>70) jsstatus_bits|=0x0100;
 if (jsstatus_bits) status_bits=jsstatus_bits+(kbstatus_bits & 0x00FC);
 else 
#endif
 if (key[KEY_UP])
  kbstatus_bits|=0x0200;
 else
  kbstatus_bits&=(~0x0200);
 if (key[KEY_RIGHT])
  kbstatus_bits|=0x0400;
 else
  kbstatus_bits&=(~0x0400);
 if (key[KEY_LEFT])
  kbstatus_bits|=0x0800;
 else
  kbstatus_bits&=(~0x0800);
 if (key[KEY_DOWN])
  kbstatus_bits|=0x0100;
 else
  kbstatus_bits&=(~0x0100);
 if (key[KEY_ENTER])
  kbstatus_bits|=0x0008;
 else
  kbstatus_bits&=(~0x0008);
 if ((key[KEY_RSHIFT]) || (key[KEY_LSHIFT]))
  kbstatus_bits|=0x0004;
 else
  kbstatus_bits&=(~0x0004);
 if ((key[KEY_LCONTROL]) || (key[KEY_RCONTROL]))
  kbstatus_bits|=0x0001;
 else
  kbstatus_bits&=(~0x0001);
 if (key[KEY_ALT])
  kbstatus_bits|=0x0002;
 else
  kbstatus_bits&=(~0x0002);
 status_bits=kbstatus_bits;
 usedb=(status_bits & 0x000F)<<4;
 a=(status_bits & 0x0010)?0:(status_bits & 0x0001);
 b=(status_bits & 0x0020)?0:(status_bits & 0x0002);
 select=(status_bits & 0x0040)?0:(status_bits & 0x0004);
 start=(status_bits & 0x0080)?0:(status_bits & 0x0008);
 down=(status_bits & 0x0100)?1:((status_bits &0x0200)?-1:0);
 right=(status_bits & 0x0400)?1:((status_bits &0x0800)?-1:0);
 kbstatus_bits&=0xFF0F;
 kbstatus_bits|=usedb;
}

#else

#pragma inline

void interrupt neshandler(__CPPARGS);

nes_controller::nes_controller()
{
 oldhandler=getvect(INTR);
 setvect(INTR,&neshandler);
 kbstatus_bits=0;
#ifdef JOYSTICK
 setvect(0x61,oldhandler);
#endif
}

nes_controller::~nes_controller()
{
 setvect(INTR,oldhandler);
}

void nes_controller::status(signed char &down,signed char &right,
  unsigned char &a,unsigned char &b,unsigned char &start,
  unsigned char &select)
{
 unsigned short usedb;

#ifdef JOYSTICK
 unsigned short jsx,jsy;
 unsigned char jsbuttons;

 asm{
  mov ah,84h
  mov dx,0000h
  int 61h
  mov jsbuttons,al
  mov ah,84h
  mov dx,0001h
  int 61h
  mov jsx,ax
  mov jsy,bx
 }
 jsstatus_bits=0;
 if (js.buttons & 16) jsstatus_bits|=0x0002;
 if (js.buttons & 32) jsstatus_bits|=0x0001;
 if (js.x<30) jsstatus_bits|=0x0800;
 if (js.x>70) jsstatus_bits|=0x0400;
 if (js.y<30) jsstatus_bits|=0x0200;
 if (js.y>70) jsstatus_bits|=0x0100;
 if (jsstatus_bits) status_bits=jsstatus_bits+(kbstatus_bits & 0x00FC);
 else 
#endif
 status_bits=kbstatus_bits;
 usedb=(status_bits & 0x000F)<<4;
 a=(status_bits & 0x0010)?0:(status_bits & 0x0001);
 b=(status_bits & 0x0020)?0:(status_bits & 0x0002);
 select=(status_bits & 0x0040)?0:(status_bits & 0x0004);
 start=(status_bits & 0x0080)?0:(status_bits & 0x0008);
 down=(status_bits & 0x0100)?1:((status_bits &0x0200)?-1:0);
 right=(status_bits & 0x0400)?1:((status_bits &0x0800)?-1:0);
 kbstatus_bits&=0xFF0F;
 kbstatus_bits|=usedb;
}

// Modified from Paul S. Reid's Space Invaders
void interrupt neshandler(__CPPARGS)
{
 // This should be in the code but it produces unpredictable results
/* asm {
  cmp ax,04Fh
  jnz InterruptEnd
 }*/
 asm {
  mov cl,al
  and cl,080h
  jz  NotFlip1:
  mov bx,kbstatus_bits
  not bx
  mov kbstatus_bits,bx
  and al,07Fh
 }
NotFlip1:
 // Check left arrow
 asm {
  cmp al,04Bh
  jnz NoLeftOn
  or  kbstatus_bits,00800h
 }
NoLeftOn:
 // Check right arrow
 asm {
  cmp al,04Dh
  jnz NoRightOn
  or  kbstatus_bits,00400h
 }
NoRightOn:
 // Check up arrow
 asm {
  cmp al,048h
  jnz NoUpOn
  or  kbstatus_bits,00200h
 }
NoUpOn:
 // Check down arrow
 asm {
  cmp al,050h
  jnz NoDownOn
  or  kbstatus_bits,00100h
 }
NoDownOn:
 // Check A (Ctrl key)
 asm {
  cmp al,01Dh
  jnz NoAOn
  or  kbstatus_bits,00001h
 }
NoAOn:
 // Check B (Alt key)
 asm {
  cmp al,038h
  jnz NoBOn
  or  kbstatus_bits,00002h
 }
NoBOn:
 // Check Select (Right Shift key)
 asm {
  cmp al,036h
  jnz NoSelectOn
  or  kbstatus_bits,0004h
 }
 // Check Select (Left Shift key)
 asm {
  cmp al,02Ah
  jnz NoSelectOn
  or  kbstatus_bits,0004h
 }
NoSelectOn:
 // Check Start (Return key)
 asm {
  cmp al,01Ch
  jnz NoStartOn
  or  kbstatus_bits,0008h
 }
NoStartOn:
 asm {
  cmp cl,080h
  jnz NotFlip2:
  mov bx,kbstatus_bits
  not bx
  mov kbstatus_bits,bx
 }
NotFlip2:
 // Clear keyboard buffer (BIOS routine is still called, and it annoyingly
 // beeps when it's buffer is full)
 pokeb(0,0x41a,0x001e);
 pokeb(0,0x41c,0x001e);
 // Been told you could replace it with the following but it doesn't work
/* asm {
   clc
  }*/
 // You could just change al to 0x80 but tc adds a push ax and pop ax so
 // you have to produce assembly output and change that.
InterruptEnd:
}
#endif

#elif defined(__linux__)
#ifdef SVGALIB

void neshandler(int scan,int press);

nes_controller::nes_controller()
{
#ifdef JOYSTICK
 jsdev=open("/dev/js0",O_RDONLY);
#endif
 keyboard_init();
 keyboard_seteventhandler(&neshandler);
 kbstatus_bits=0;
}

nes_controller::~nes_controller()
{
 keyboard_close();
}

void nes_controller::status(signed char &down,signed char &right,
  unsigned char &a,unsigned char &b,unsigned char &start,
  unsigned char &select)
{
 unsigned short usedb;

 keyboard_update();
#ifdef JOYSTICK
 read(jsdev,&js,JS_RETURN);
 jsstatus_bits=0;
 if (js.buttons & 1) jsstatus_bits|=0x0002;
 if (js.buttons & 2) jsstatus_bits|=0x0001;
 if (js.x<500) jsstatus_bits|=0x0800;
 if (js.x>1000) jsstatus_bits|=0x0400;
 if (js.y<500) jsstatus_bits|=0x0200;
 if (js.y>1000) jsstatus_bits|=0x0100;
 if (jsstatus_bits) status_bits=jsstatus_bits+(kbstatus_bits & 0x00FC);
 else 
#endif
 status_bits=kbstatus_bits;
 usedb=(status_bits & 0x000F)<<4;
 a=(status_bits & 0x0010)?0:(status_bits & 0x0001);
 b=(status_bits & 0x0020)?0:(status_bits & 0x0002);
 select=(status_bits & 0x0040)?0:(status_bits & 0x0004);
 start=(status_bits & 0x0080)?0:(status_bits & 0x0008);
 down=(status_bits & 0x0100)?1:((status_bits &0x0200)?-1:0);
 right=(status_bits & 0x0400)?1:((status_bits &0x0800)?-1:0);
 kbstatus_bits&=0xFF0F;
 kbstatus_bits|=usedb;
}

void neshandler(int scan,int press)
{
 unsigned short mask;

 switch (scan) {
  case SCANCODE_KEYPAD8:
  case SCANCODE_CURSORBLOCKUP:
   mask=0x0200;
   break;
  case SCANCODE_KEYPAD6:
  case SCANCODE_CURSORBLOCKRIGHT:
   mask=0x0400;
   break;
  case SCANCODE_KEYPAD4:
  case SCANCODE_CURSORBLOCKLEFT:
   mask=0x0800;
   break;
  case SCANCODE_KEYPAD2:
  case SCANCODE_CURSORBLOCKDOWN:
   mask=0x0100;
   break;
  case SCANCODE_ENTER:
   mask=0x0008;
   break;
  case SCANCODE_LEFTSHIFT:
  case 54:
   mask=0x0004;
   break;
  case SCANCODE_RIGHTCONTROL:
  case SCANCODE_LEFTCONTROL:
   mask=0x0001;
   break;
  case SCANCODE_RIGHTALT:
  case SCANCODE_LEFTALT:
   mask=0x0002;
   break;
  default:
   mask=0;
   break;
 }
 if (press) kbstatus_bits|=mask;
 else kbstatus_bits&=(~mask);
}

#elif defined(XLIB)

nes_controller::nes_controller()
{
#ifdef JOYSTICK
 jsdev=open("/dev/js0",O_RDONLY);
#endif
 XSelectInput(IXDisplay,IXWindow,FocusChangeMask|KeyPressMask|KeyReleaseMask);
 XAutoRepeatOff(IXDisplay);
 kbstatus_bits=0;
}

nes_controller::~nes_controller()
{
 XAutoRepeatOn(IXDisplay);
}

void nes_controller::status(signed char &down,signed char &right,
  unsigned char &a,unsigned char &b,unsigned char &start,
  unsigned char &select)
{
 XEvent xevent;
 unsigned short i;
 unsigned short mask;
 unsigned short usedb;

 if(XCheckWindowEvent(IXDisplay,IXWindow,KeyPressMask|KeyReleaseMask,&xevent)) {
  i=XLookupKeysym((XKeyEvent *)&xevent,0);
  switch(i) {
   case XK_Return:
    mask=0x0008;
    break;
   case XK_Down:
   case XK_KP_Down:
    mask=0x0100;
    break;
   case XK_Up:
   case XK_KP_Up:
    mask=0x0200;
    break;
   case XK_Left:
   case XK_KP_Left:
    mask=0x0800;
    break;
   case XK_Right:
   case XK_KP_Right:
    mask=0x0400;
    break;
   case XK_Alt_L:
   case XK_Alt_R:
    mask=0x0002;
    break;
   case XK_Control_L:
   case XK_Control_R:
    mask=0x0001;
    break;
   case XK_Shift_R:
   case XK_Shift_L:
    mask=0x0004;
    break;
   default:
    mask = 0;
    break;
  }
  if(xevent.type==KeyPress) kbstatus_bits|=mask;
  else kbstatus_bits&=(~mask);
 }
#ifdef JOYSTICK
 read(jsdev,&js,JS_RETURN);
 jsstatus_bits=0;
 if (js.buttons & 1) jsstatus_bits|=0x0002;
 if (js.buttons & 2) jsstatus_bits|=0x0001;
 if (js.x<500) jsstatus_bits|=0x0800;
 if (js.x>700) jsstatus_bits|=0x0400;
 if (js.y<500) jsstatus_bits|=0x0200;
 if (js.y>700) jsstatus_bits|=0x0100;
 if (jsstatus_bits) status_bits=jsstatus_bits+(kbstatus_bits & 0x00FC);
 else
#endif
 status_bits=kbstatus_bits;
 for(i=0;XCheckWindowEvent(IXDisplay,IXWindow,FocusChangeMask,&xevent);)
 if (xevent.type==FocusOut) {
  XAutoRepeatOn(IXDisplay);
  while(!XCheckWindowEvent(IXDisplay,IXWindow,FocusChangeMask,&xevent));
  XAutoRepeatOff(IXDisplay);
 }
 usedb=(status_bits & 0x000F)<<4;
 a=(status_bits & 0x0010)?0:(status_bits & 0x0001);
 b=(status_bits & 0x0020)?0:(status_bits & 0x0002);
 select=(status_bits & 0x0040)?0:(status_bits & 0x0004);
 start=(status_bits & 0x0080)?0:(status_bits & 0x0008);
 down=(status_bits & 0x0100)?1:((status_bits &0x0200)?-1:0);
 right=(status_bits & 0x0400)?1:((status_bits &0x0800)?-1:0);
 kbstatus_bits&=0xFF0F;
 kbstatus_bits|=usedb;
}

#endif
#endif

