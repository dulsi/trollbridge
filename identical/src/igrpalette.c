/*-------------------------------------------------------------------------*\
  <igrpalette.c> -- Palette Routines for Identical Software's Library

  Date      Programmer  Description
  01/01/01  Dennis      Created.
\*-------------------------------------------------------------------------*/
#include <stdio.h>
#include <igrbasics.h>
#include <igrimage.h>
#include <igrpalette.h>

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

IPalette IPalettePalLoad(const char *filename)
{
 IPalette pal;
 FILE *palfile;

 if ((palfile = fopen(filename, "rb"))==NULL)
 {
  exit(2);
 }
 pal = IPaletteCreate();
 fread(pal, 1, 3 * 256, palfile);
 fclose(palfile);
 return pal;
}

void IPalettePalSave(IPalette pal, const char *filename)
{
 FILE *palfile;

 if ((palfile = fopen(filename, "wb"))==NULL)
 {
  exit(2);
 }
 fwrite(pal, 1, 3 * 256, palfile);
 fclose(palfile);
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
  if ((strncmp(line, "GIMP Palette", 12) == 0) || (strncmp(line, "Name:", 5) == 0) || (strncmp(line, "Columns:", 8) == 0))
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

