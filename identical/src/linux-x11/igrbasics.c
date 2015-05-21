/*-------------------------------------------------------------------------*\
  <igrbasics.c> -- Identical Software's Primitive Graphic Routines
    (Linux X11 version)

  Date      Programmer  Description
  08/28/97  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* Note: The X11 support for Identical Software's Graphics Library was
intended for debugging since debugging SVGALIB is difficult.  As such the
code is not very efficient. */

#include <igrbasics.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#include <stdio.h> /* X has a lot of unrecoverable errors */

#define MIN(x,y) (((x)<(y))?(x):(y))

IScreen IScreenMain;
IPalette IPaletteMain = NULL;

/* Extra global variables needed by X */
Display *IXDisplay;
Window IXWindow;
XShmSegmentInfo IXShminfo;
XImage *IXImage;
GC IXGc;
Colormap IXCmap;

void IGraphicsStart(const char *name)
{
 Screen *xscreen;
 XSizeHints xsizehints;
 XWMHints xwmhints;

 IXDisplay = XOpenDisplay(NULL);
 if (!IXDisplay)
 {
  printf("Failed - XOpenDisplay\n");
  exit(0);
 }
 xscreen = DefaultScreenOfDisplay(IXDisplay);
 IXGc = DefaultGCOfScreen(xscreen);
 IXWindow = XCreateSimpleWindow(IXDisplay, RootWindowOfScreen(xscreen), 0, 0,
   320, 200, 0, WhitePixelOfScreen(xscreen), BlackPixelOfScreen(xscreen));
 if (!IXWindow)
 {
  printf("Failed - XCreateSimpleWindow\n");
  exit(0);
 }
 XStoreName(IXDisplay, IXWindow, name);
 xsizehints.flags = PSize | PMinSize | PMaxSize;
 xsizehints.min_width = xsizehints.max_width = xsizehints.base_width = 320;
 xsizehints.min_height = xsizehints.max_height = xsizehints.base_height = 200;
 XSetWMNormalHints(IXDisplay, IXWindow, &xsizehints);
 xwmhints.input = True;
 xwmhints.flags = InputHint;
 XSetWMHints(IXDisplay, IXWindow, &xwmhints);
 XMapRaised(IXDisplay, IXWindow);
 IXImage = XShmCreateImage(IXDisplay, DefaultVisualOfScreen(xscreen), 8,
   ZPixmap, NULL, &IXShminfo, 320, 200);
 if (!IXImage)
 {
  printf("Failed - XShmCreateImage\n");
  exit(0);
 }
 IXShminfo.shmid = shmget(IPC_PRIVATE,
   IXImage->bytes_per_line * IXImage->height, IPC_CREAT | 0777);
 if (IXShminfo.shmid < 0)
 {
  printf("Failed - shmget\n");
  exit(0);
 }
 IXShminfo.shmaddr = IXImage->data = shmat(IXShminfo.shmid, 0, 0);
 IPaletteMain = (IPalette)IScreenMain = (IScreen) IXShminfo.shmaddr;
 if (!IScreenMain)
 {
  printf("Failed - shmat\n");
  exit(0);
 }
 IXShminfo.readOnly=False;
 if (!XShmAttach(IXDisplay, &IXShminfo))
 {
  printf("Failed - XShmAttach\n");
  exit(0);
 }
 shmctl(IXShminfo.shmid, IPC_RMID, 0); /* done in dosemu not sure why */
 IXCmap = XCreateColormap(IXDisplay, RootWindowOfScreen(xscreen),
   DefaultVisualOfScreen(xscreen), AllocAll);
 XSetWindowColormap(IXDisplay, IXWindow, IXCmap);
 XSync(IXDisplay, False);
}

void IGraphicsRefresh()
{
 XShmPutImage(IXDisplay, IXWindow, IXGc, IXImage, 0, 0, 0, 0, 320, 200,
   False);
 /* Doesn't refresh for some reason */
 /*XFlush(XIDisplay);*/
 XSync(IXDisplay, False);
}

void IGraphicsEnd()
{
 XShmDetach(IXDisplay, &IXShminfo);
 XDestroyImage(IXImage);
 if (IXShminfo.shmaddr)
 {
  shmdt(IXShminfo.shmaddr);
 }
 /* Crashes with this but was listed in some documention don't know why */
 /*if (IXShminfo.shmid >= 0)
 {
  shmctl(IXShminfo.shmid, IPC_RMID, 0);
 }*/
 XUninstallColormap(IXDisplay, IXCmap);
 XFreeColormap(IXDisplay, IXCmap);
 XCloseDisplay(IXDisplay);
}

void IPaletteCopy(IPalette paldst, const IPalette palsrc)
{
 XColor xcol[256];
 int color, rgb;

 if (palsrc == IPaletteMain) {
  for (color = 0; color < 256; color++) {
   xcol[color].pixel = color;
  }
  XQueryColors(IXDisplay, IXCmap, xcol, 256);
  for (color = 0; color < 256; color++) {
   (*paldst)[color][0] = (xcol[color].red * 63) / 65535;
   (*paldst)[color][1] = (xcol[color].green * 63) / 65535;
   (*paldst)[color][2] = (xcol[color].blue * 63) / 65535;
  }
 }
 else if (paldst == IPaletteMain) {
  for (color = 0; color < 256; color++) {
   xcol[color].flags = DoRed | DoGreen | DoBlue;
   xcol[color].pixel = color;
   xcol[color].red = (MIN((*palsrc)[color][0], 63) * 65535) / 63;
   xcol[color].green = (MIN((*palsrc)[color][1], 63) * 65535) / 63;
   xcol[color].blue = (MIN((*palsrc)[color][2], 63) * 65535) / 63;
  }
  XStoreColors(IXDisplay, IXCmap, xcol, 256);
 }
 else {
  IMemcpy(paldst, palsrc, sizeof(IPaletteTable));
 }
}

void IPaletteSet(IPalette pal, IPixel c, IColor r, IColor g, IColor b)
{
 XColor xcol;

 if (pal == IPaletteMain) {
  xcol.flags = DoRed | DoGreen | DoBlue;
  xcol.pixel = c;
  xcol.red = (MIN(r, 63) * 65535) / 63;
  xcol.green = (MIN(g, 63) * 65535) / 63;
  xcol.blue = (MIN(b, 63) * 65535) / 63;
  XStoreColor(IXDisplay, IXCmap, &xcol);
 }
 else {
  (*pal)[c][0] = r;
  (*pal)[c][1] = g;
  (*pal)[c][2] = b;
 }
}

void IPaletteGet(const IPalette pal, IPixel c, IColor IFAR *r, IColor IFAR *g,
                 IColor IFAR *b)
{
 XColor xcol;

 if (pal == IPaletteMain) {
  xcol.pixel = c;
  XQueryColor(IXDisplay, IXCmap, &xcol);
  *r = (xcol.red * 63) / 65535;
  *g = (xcol.green * 63) / 65535;
  *b = (xcol.blue * 63) / 65535;
 }
 else {
  *r = (*pal)[c][0];
  *g = (*pal)[c][1];
  *b = (*pal)[c][2];
 }
}

