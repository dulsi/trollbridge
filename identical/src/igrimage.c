/*-------------------------------------------------------------------------*\
  <igrimage.c> -- Identical Software's Image Routines

  Date      Programmer  Description
  02/28/97  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include <istdlib.h>
#include <igrbasics.h>
#include <igrimage.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>


IImage IImageCapture(const IScreen screen, IUShort x1, IUShort y1, IUShort x2,
                     IUShort y2)
{
 IImage img;
 IPixel IFAR *linesrc, IFAR *linedst;
 int i;

 img = (IImage)IMalloc(sizeof(struct IImageStruct));
 if (img == NULL) {
  return img;
 }
 img->x = x2 - x1 + 1;
 img->y = y2 - y1 + 1;
 img->pal = NULL;
 img->pic = (IPixel IFAR *)IMalloc(img->x * img->y);
 if (img->pic == NULL) {
  IFree(img);
  return NULL;
 }
 for (i = img->y, linesrc = screen + (y1 << 8) + (y1 << 6) + x1,
      linedst = img->pic; i > 0; i--, linesrc += 320, linedst += img->x) {
  IMemcpy(linedst, linesrc, img->x);
 }
 return img;
}

void IImageDraw(IScreen screen, IUShort x, IUShort y, IImage img)
{
 IPixel IFAR *linesrc, IFAR *linedst;
 int i;

 for (i = img->y, linedst = screen + (y << 8) + (y << 6) + x,
      linesrc = img->pic; i > 0; i--, linedst += 320, linesrc += img->x) {
  IMemcpy(linedst, linesrc, img->x);
 }
}

void IImagePaletteSet(IImage img, const IPalette pal)
{
 if (img->pal == NULL) {
  if (pal != NULL) {
   img->pal = IPaletteCreate();
   IPaletteCopy(img->pal, pal);
  }
 }
 else {
  if (pal == NULL) {
   IPaletteDestroy(img->pal);
   img->pal = NULL;
  }
  else {
   IPaletteCopy(img->pal, pal);
  }
 }
}

IImage IImageLoad(const char *filename)
{
 IImage img;
 SDL_Surface *surf = IMG_Load(filename);
 if (surf == NULL)
  return NULL;
 img = (IImage)IMalloc(sizeof(struct IImageStruct));
 img->x = surf->w;
 img->y = surf->h;
 img->pic = (IPixel IFAR *)IMalloc(sizeof(IPixel) * img->x * img->y);
 memcpy(img->pic, surf->pixels, sizeof(IPixel) * img->x * img->y);
 img->pal = (IPalette)IMalloc(sizeof(IPaletteTable));
 for (int i = 0; i < surf->format->palette->ncolors; i++)
 {
  IPaletteSet(img->pal, i, surf->format->palette->colors[i].r / 4, surf->format->palette->colors[i].g / 4, surf->format->palette->colors[i].b / 4);
 }
 return img;
}

IImage IImagePCXLoad(const char *filename)
{
 IImage img;
 FILE *pcxfile;
 IUShort bpl; /* Bytes per line */
 unsigned char c1, c2;
 int i, k, j;

 /* MSDOS: Medium and lower memory models can't pass far pointers to fread */
 pcxfile = fopen(filename, "rb");
 if (pcxfile == NULL) {
  return NULL;
 }
 if ((fgetc(pcxfile) != 0x0A) /* Manufacturer check */ ||
     (fgetc(pcxfile) != 0x05) /* Version check */ ||
     (fgetc(pcxfile) != 0x01) /* Encoding check */ ||
     (fgetc(pcxfile) != 0x08) /* Bits per pixel check */) {
  fclose(pcxfile);
  return NULL;
 }
 fseek(pcxfile, 65, SEEK_SET);
 if (fgetc(pcxfile) != 0x01) /* Number of planes check */ {
  fclose(pcxfile);
  return NULL;
 }
 img = (IImage)IMalloc(sizeof(struct IImageStruct));
 if (img == NULL) {
  fclose(pcxfile);
  return NULL;
 }
 /* BIG EIDIAN: Won't work with the following */
 fread(&bpl, sizeof(IUShort), 1, pcxfile);
 fseek(pcxfile, 8, SEEK_SET);
 fread(&img->x, sizeof(IUShort), 1, pcxfile);
 fread(&img->y, sizeof(IUShort), 1, pcxfile);
 img->x += 1;
 img->y += 1;
 img->pic = (IPixel IFAR *)IMalloc(sizeof(IPixel) * img->x * img->y);
 img->pal = (IPalette)IMalloc(sizeof(IPaletteTable));
 if ((img->pic == NULL) || (img->pal == NULL)) {
  if (img->pic != NULL) IFree(img->pic);
  if (img->pal != NULL) IFree(img->pal);
  IFree(img);
  fclose(pcxfile);
  return NULL;
 }
 fseek(pcxfile, 128, SEEK_SET);
 for (i = 0; i < img->y; i++) {
  for (k = 0; k < bpl; k++) {
   c1 = fgetc(pcxfile);
   if ((c1 & 0xC0) == 0xC0) {
    j = c1 - 0xC0 + k;
    c2 = fgetc(pcxfile);
    for ( ; k < j; k++) {
     if (k < img->x) {
      (img->pic)[i * img->x + k] = c2;
     }
    }
    k--;
   }
   else {
    if (k < img->x) {
     (img->pic)[i * img->x + k] = c1;
    }
   }
  }
 }
 fseek(pcxfile, -768, SEEK_END);
 fread(img->pal, 1, sizeof(IPaletteTable), pcxfile);
 for (i = 0; i < 256; i++) {
  for (k = 0; k < 3; k++) {
   (*img->pal)[i][k]/=4;
  }
 }
 fclose(pcxfile);
 return img;
}

void IImagePCXSave(IImage img, const char *filename)
{
 FILE *pcxfile;
 IUShort bpl; /* Bytes per line */
 IUShort dpi; /* Dots per inch */
 unsigned char c1, c2;
 int i, k;

 /* MSDOS: Medium and lower memory models can't pass far pointers to fread */
 /* BIG EIDIAN: Won't work with the save routine either */
 pcxfile = fopen(filename, "wb+");
 if (pcxfile == NULL) {
  return ;
 }
 bpl = (img->x + 1) & 0xFFFE; /* BPL must be even according to PCX specs */
 fputc(0x0A, pcxfile); /* Manufacturer */
 fputc(0x05, pcxfile); /* Version */
 fputc(0x01, pcxfile); /* Encoding */
 fputc(0x08, pcxfile); /* Bits per pixel */
 for (i = 0; i < 4; i++) {
  fputc(0x00, pcxfile); /* X & Y min left at zero */
 }
 img->x--;
 img->y--;
 fwrite(&img->x, 1, sizeof(img->x), pcxfile);
 fwrite(&img->y, 1, sizeof(img->y), pcxfile);
 img->x++;
 img->y++;
 dpi = 72;
 fwrite(&dpi, 1, sizeof(dpi), pcxfile); /* Horizontal DPI */
 fwrite(&dpi, 1, sizeof(dpi), pcxfile); /* Vertical DPI */
 for (i = 0; i < 49; i++) {
  fputc(0x00, pcxfile); /* Colormap and 1 reserved byte */
 }
 fputc(0x01, pcxfile); /* Number of planes */
 fwrite(&bpl, 1, sizeof(bpl), pcxfile); /* Bytes per line */
 for (i = 0; i < 60; i++) {
  fputc(0x00, pcxfile); /* Extra unnecessary header info */
 }
 for (i = 0; i < img->y; i++) { /* Compressed image */
  c1 = 1;
  c2 = (img->pic)[i * img->x];
  for (k = 1; k < img->x; k++) {
   if (c2 == (img->pic)[i * img->x + k]) {
    if (c1 == 63) {
     fputc(c1 | 0xC0, pcxfile);
     fputc(c2, pcxfile);
     c1 = 0;
    }
    c1++;
   }
   else {
    if ((c1 != 1) || (c2 & 0xC0)) {
     fputc(c1 | 0xC0, pcxfile);
     fputc(c2, pcxfile);
     c1 = 1;
    }
    else {
     fputc(c2, pcxfile);
    }
    c2 = (img->pic)[i * img->x + k];
   }
  }
  if (bpl != img->x) {
   c1++;
  }
  if ((c1 != 1) || (c2 & 0xC0)) {
   fputc(c1 | 0xC0, pcxfile);
   fputc(c2, pcxfile);
  }
  else {
   fputc(c2, pcxfile);
  }
 }
 fputc(0x0C, pcxfile); /* Identifier before 256-color palette */
 if (img->pal) { /* 256-color palette */
  for (i = 0; i < 256; i++) {
   for (k = 0; k < 3; k++) {
    (*img->pal)[i][k]*=4;
   }
  }
  fwrite(img->pal, 1, sizeof(IPaletteTable), pcxfile);
  for (i = 0; i < 256; i++) {
   for (k = 0; k < 3; k++) {
    (*img->pal)[i][k]/=4;
   }
  }
 }
 else {
  for (i = 0; i < 768; i++) {
   fputc(0x00, pcxfile);
  }
 }
 /* write palette */
 fclose(pcxfile);
}

void IImageDestroy(IImage img)
{
 if (img->pic) {
  IFree(img->pic);
 }
 if (img->pal) {
  IFree(img->pal);
 }
 IFree(img);
}

