/*-------------------------------------------------------------------------*\
  <igrbasics.c> -- Identical Software's Primitive Graphic Routines
    (DOS version)

  Date      Programmer  Description
  02/28/97  Dennis      Created.
\*-------------------------------------------------------------------------*/
#include <igrbasics.h>
#include <dos.h>

/* Newer versions of Borland C do not include the Turbo Assembler */
/*#pragma inline*/ 

IScreen IScreenMain;
IPalette IPaletteMain = NULL;

void IGraphicsStart(const char *name, IUShort xMult, IUShort yMult,
                    IBool fullScreen)
{
 asm {
  mov ax,0013h
  int 10h
 }
 IPaletteMain = (IPalette)IScreenMain = (IScreen)MK_FP(0xA000,0);
}

void IGraphicsRetrace()
{
 asm {
  mov dx, 03DAh
 }
l1:
 asm {
  in al, dx
  and al, 08h
  jnz l1
 }
l2:
 asm {
  in al, dx
  and al, 08h
  jz l2
 }
}

void IGraphicsEnd()
{
 asm {
  mov ax,0003h
  int 10h
 }
}

void IScreenClear(IScreen screen)
{
 asm {
  push es
  push di
  les di, screen
  mov cx, 32000
  xor ax,ax
  rep stosw
  pop di
  pop es
 }
}

void IScreenCopy(IScreen screen1, IScreen screen2)
{
 asm {
  push ds
  push es
  push di
  push si
  les di, screen1
  lds si, screen2
  mov cx, 32000
  rep movsw
  pop si
  pop di
  pop es
  pop ds
 }
}

void IPaletteCopy(IPalette paldst, IPalette palsrc)
{
 if (palsrc == IPaletteMain) {
  asm {
   push es
   push di
   mov cx, 768
   les di, paldst
   mov dx, 03C7h
   xor al,al
   out dx, al
   add dx, 2
   rep insb
   pop di
   pop es
  }
 }
 else if (paldst == IPaletteMain) {
  IGraphicsRetrace();
  asm {
   push ds
   push si
   mov cx, 768
   lds si, palsrc
/*   call IGraphicsRetrace*/ /* illegal without Turbo Assembler */
   mov dx, 03C8h
   xor al, al
   out dx, al
   inc dx
   rep outsb
   pop si
   pop ds
  }
 }
 else {
  IMemcpy(paldst, palsrc, sizeof(IPaletteTable));
 }
}

void IPaletteSet(IPalette pal, IPixel c, IColor r, IColor g, IColor b)
{
 if (pal == IPaletteMain) {
  IGraphicsRetrace();
  asm {
/*   call IGraphicsRetrace*/ /* illegal without Turbo Assembler */
   mov dx, 03C8h
   mov al, c
   out dx, al
   inc dx
   mov al, r
   out dx, al
   mov al, g
   out dx, al
   mov al, b
   out dx, al
  }
 }
 else {
  (*pal)[c][0] = r;
  (*pal)[c][1] = g;
  (*pal)[c][2] = b;
 }
}

void IPaletteGet(IPalette pal, IPixel c, IColor IFAR *r, IColor IFAR *g,
                 IColor IFAR *b)
{
 if (pal == IPaletteMain) {
  asm {
   push es
   push di
   mov dx, 03C7h
   mov al, c
   out dx, al
   add dx, 2
   les di, r
   insb
   les di, g
   insb
   les di, b
   insb
   pop di
   pop es
  }
 }
 else {
  *r = (*pal)[c][0];
  *g = (*pal)[c][1];
  *b = (*pal)[c][2];
 }
}

