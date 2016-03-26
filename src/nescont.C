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

#include <SDL.h>

SDL_Joystick *js = NULL;
unsigned int jsmask = 0;

static unsigned short status_bits;
static unsigned short kbstatus_bits;
static unsigned short jsstatus_bits;

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
   case SDL_QUIT:
   {
    SDL_Quit();
    exit(0);
   }
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
