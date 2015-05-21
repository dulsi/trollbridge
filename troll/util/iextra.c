/*-------------------------------------------------------------------------*\
  <iextra.c> -- Extra Routines for Identical Software's Library

  Date      Programmer  Description
  02/01/00  Dennis      Created.
\*-------------------------------------------------------------------------*/
#include <stdio.h>
#include <igrbasics.h>
#include <igrimage.h>
#include "iextra.h"

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
    fprintf(writeFile, "%3d =  %3d  %3d  %3d\n", i, r, g, b);
   }
  }
 }
 fclose(writeFile);
}

void IPaletteTextLoad(IPalette *pal, IPaletteName *palnm,
                      const char *filename)
{
 int i;
 FILE *readFile;
 int r, g, b;
 char line[255];
 char tmpName[255];

 *pal = NULL;
 *palnm = NULL;
 readFile = fopen(filename, "r");
 if (readFile == NULL)
 {
  return ;
 }
 *pal = IPaletteCreate();
 *palnm = IPaletteNameCreate();
 for (i = 0; (!feof(readFile)) && (i < 256); )
 {
  fgets(line, 255, readFile);
  if (strncmp(line, "GIMP Palette", 12) == 0)
  { /* Allows the reading of GIMP palette files. */
   continue;
  }
  if ((line[0]) && (line[0] != '#') && (line[0] != '\n'))
  {
   sscanf(line, "%d%d%d%s", &r, &g, &b, tmpName);
   IPaletteSet(*pal, i, r / 4, g / 4, b / 4);
   /* Error: Not correct for all DOS memory models */
   (*(*palnm))[i] = (IPaletteNameEntry)strdup(tmpName);
   i++;
  }
 }
 for ( ; i < 256; i++)
 {
  IPaletteSet(*pal, i, 0, 0, 0);
  (*(*palnm))[i] = NULL;
 }
 fclose(readFile);
}

IPixel IPaletteFind(const IPalette pal, IColor r, IColor g, IColor b)
{
 int i;

 for (i = 0; i < 256; i++)
 {
  if (((*pal)[i][0] == r) && ((*pal)[i][1] == g) && ((*pal)[i][2] == b))
  {
   return i;
  }
 }
 return 255;
}

IPaletteName IPaletteNameCreate()
{
 IPaletteName result;

 result = (IPaletteName)IMalloc(sizeof(IPaletteNameTable));
 IMemset(result, 0, sizeof(IPaletteNameTable));
 return result;
}

IPixel IPaletteNameFind(const IPaletteName palnm,
                        const IPaletteNameEntry name)
{
 int i;

 for (i = 0; (i < 256) && ((*palnm)[i]); i++)
 {
   /* Error: Not correct for all DOS memory models */
  if (strcmp((*palnm)[i], name) == 0)
  {
   return i;
  }
 }
 return 255;
}

void IPaletteNameDestroy(IPaletteName palnm)
{
 int i;

 for (i = 0; (i < 256) && ((*palnm)[i]); i++)
 {
  IFree((*palnm)[i]);
 }
 IFree(palnm);
}

