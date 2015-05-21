/*-------------------------------------------------------------------------*\
  <iextra.c> -- Extra Routines for Identical Software's Library

  Date      Programmer  Description
  02/01/00  Dennis      Created.
\*-------------------------------------------------------------------------*/
#include <stdio.h>
#include <igrbasics.h>
#include <igrimage.h>
#include "iextra.h"

IImage IImageSubCapture(const IImage img, IUShort x1, IUShort y1, IUShort x2,
                        IUShort y2)
{
 IImage imgsub;
 IPixel IFAR *linesrc, IFAR *linedst;
 int i;

 imgsub = (IImage)IMalloc(sizeof(struct IImageStruct));
 if (imgsub == NULL)
 {
  return imgsub;
 }
 if (x2 >= imgsub->x)
 {
  x2 = imgsub->x - 1;
 }
 imgsub->x = x2 - x1 + 1;
 imgsub->y = y2 - y1 + 1;
 imgsub->pal = IPaletteCreate();
 IPaletteCopy(imgsub->pal, img->pal);
 imgsub->pic = (IPixel IFAR *)IMalloc(imgsub->x * imgsub->y);
 if (imgsub->pic == NULL)
 {
  IFree(imgsub);
  return NULL;
 }
 for (i = imgsub->y, linesrc = img->pic + (y1 * img->x) + x1,
   linedst = imgsub->pic; i > 0; i--, linesrc += img->x, linedst += imgsub->x)
 {
  IMemcpy(linedst, linesrc, imgsub->x);
 }
 return imgsub;
}

IImage IImageTextLoad(const char *filename, IPalette pal, IPaletteName palnm)
{
 IPixel IFAR *curPixel;
 ILong i, k;
 FILE *readFile;
 int r, g, b;
 char line[255];
 unsigned char tmpName[255];
 IPixel convert[255];
 IImage img;
 int tmpPixel;

 readFile = fopen(filename, "r");
 if (readFile == NULL)
 {
  return NULL;
 }
 img = (IImage)IMalloc(sizeof(struct IImageStruct));
 fgets(line, 255, readFile);
 if (strncmp("[XSize]  ", line, 9) != 0)
 {
  return NULL;
 }
 sscanf(line + 9, "%d", &(img->x));
 fgets(line, 255, readFile);
 if (strncmp("[YSize]  ", line, 9) != 0)
 {
  return NULL;
 }
 sscanf(line + 9, "%d", &(img->y));
 do
 {
  fgets(line, 255, readFile);
  if (feof(readFile))
  {
   return NULL;
  }
 } while (strncmp("[Picture]", line, 9));
 img->pic = (IPixel IFAR *)IMalloc(sizeof(IPixel) * img->x * img->y);
 img->pal = NULL;
 for (i = (img->x * img->y), curPixel = img->pic, k = 0, fgets(line, 255, readFile);
   i > 0; i--, curPixel++, k++)
 {
  if (k == 14)
  {
   fgets(line, 255, readFile);
   k = 0;
  }
  /* Cannot sscanf directly into curPixel because it is a byte and %d scans
     an integer */
  sscanf(line + k * 5, "%d", &tmpPixel);
  *curPixel = tmpPixel;
 }
 do
 {
  fgets(line, 255, readFile);
  if (feof(readFile))
  {
   return NULL;
  }
 } while (strncmp("[Palette]", line, 9));
 do
 {
  fgets(line, 255, readFile);
  if ((line[0]) && (line[0] != '#') && (line[0] != '\n'))
  {
   sscanf(line, "%d", &i);
   sscanf(line + 5, "%s", tmpName);
   convert[i] = IPaletteNameFind(palnm, tmpName);
  }
 } while (!feof(readFile));
 for (i = (img->x * img->y), curPixel = img->pic; i > 0; i--, curPixel++)
 {
  if (*curPixel != 255)
  {
   *curPixel = convert[*curPixel];
  }
 }
 IImagePaletteSet(img, pal);
 fclose(readFile);
 return img;
}

void IImageTextSave(IImage img, const char *filename, IPalette pal,
                    IPaletteName palnm)
{
 ILong i, k;
 IPixel IFAR *curPixel;
 IColor r, g, b;
 FILE *writeFile;
 IUByte usedColor[255];
 IPixel textPixel;
 IPaletteNameEntry palEntry;

 memset(usedColor, 0, 255);
 writeFile = fopen(filename, "w");
 if (writeFile == NULL)
 {
  return ;
 }
 fprintf(writeFile, "[XSize]  %d\n", img->x);
 fprintf(writeFile, "[YSize]  %d\n\n[Picture]\n", img->y);
 for (i = (img->x * img->y), curPixel = img->pic, k = 0; i > 0;
   i--, curPixel++, k++)
 {
  if (k == 14)
  {
   fprintf(writeFile, "\n");
   k = 0;
  }
  fprintf(writeFile, "  %3d", *curPixel);
  if (*curPixel != 255)
  {
   usedColor[*curPixel] = 1;
  }
 }
 if (k != 14)
  fprintf(writeFile, "\n");
 fprintf(writeFile, "\n[Palette]\n");
 for (i = 0; i < 255; i++)
 {
  if (usedColor[i])
  {
   IPaletteGet(IImagePaletteGet(img), i, &r, &g, &b);
   textPixel = IPaletteFind(pal, r, g, b);
   if ((textPixel != 255) && (palEntry = IPaletteNameGet(palnm, textPixel)))
   {
    fprintf(writeFile, "%3d =  %s\n", i, palEntry);
   }
   else
   {
    fprintf(writeFile, "%3d =  %3d  %3d  %3d\n", i, r * 4, g * 4, b * 4);
   }
  }
 }
 fclose(writeFile);
}

