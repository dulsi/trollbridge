/********************************************************************
  <nescont.h> -- header file for nes controller emulation

  Programmer -- Dennis Payne

  Date -- 5/26/96
********************************************************************/

#ifndef __NESCONT_H
#define __NESCONT_H

#if defined(__MSDOS__) && !defined(ALLEGRO)
#ifdef __cplusplus
    #define __CPPARGS ...
#else
    #define __CPPARGS
#endif
#endif

class nes_controller {
public:
 nes_controller();
 ~nes_controller();

 /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
   nes_controller::status - Returns the current status of the controler.

     Parameters:
       down         (Out) Y status (-1 = up, 0 = center, 1 = down)
       right        (Out) X status (-1 = left, 0 = center, 1 = right)
       a            (Out) A button status
       b            (Out) B button status
       start        (Out) Start button status
       select       (Out) Select button status
 \* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
 void status(signed char &down,signed char &right,unsigned char &a,
   unsigned char &b,unsigned char &start, unsigned char &select);

protected:
#if defined(__MSDOS__) && !defined(ALLEGRO)
 void interrupt (*oldhandler)(__CPPARGS);
#endif
};

#endif

