#include <igrimage.h>
#include <stdio.h>
#include <unistd.h>

void ParseCommandLine(int argc, char *argv[], char *inname, char *outname);
IImage IImageTxtLoad(const char *filename);

int main(int argc, char *argv[])
{
 char inname[255];
 char outname[255];
 IImage img;

 ParseCommandLine(argc, argv, inname, outname);
 img = IImageTxtLoad(inname);
 IImagePCXSave(img, outname);
}

void ParseCommandLine(int argc, char *argv[], char *inname, char *outname)
{
 char opt;
 int i;

 while ((opt=getopt(argc,argv,"h?"))!=EOF) {
  switch (opt) {
   case 'h':
   case '?':
    printf("\
Converts a pcx file into a text file\n\
\n\
text2pcx inname[.txt] [outname[.pcx]]\n\
\n\
  inname[.txt]     Name of input file\n\
  [outname[.pcx]]  Name of output file (defaults to input file name)\n\
");
    exit(0);
    break;
   default:
    break;
  }
 }
 if ((argc - optind < 1) || (argc - optind > 2))
 {
  printf("Incorrect number of parameters\n");
  exit(4);
 }
 strcpy(inname, argv[optind]);
 if (strcmp(inname + strlen(inname) - 4, ".txt") != 0)
 {
  strcat(inname, ".txt");
 }
 if (argc - optind == 2)
 {
  strcpy(outname, argv[optind + 1]);
  if (strcmp(outname + strlen(outname) - 4, ".pcx") != 0)
  {
   strcat(inname, ".pcx");
  }
 }
 else
 {
  strcpy(outname, inname);
  strcpy(outname + strlen(outname) - 3, "pcx");
 }
}

IImage IImageTxtLoad(const char *filename)
{
 IPixel IFAR *curPixel;
 ILong i, k;
 char **colorNames;
 FILE *readFile;
 int r, g, b;
 char line[255];
 char tmpName[255];
 IPixel convert[255];
 IPalette pal;
 IImage img;

 readFile = fopen("../data/trollpal.txt", "r");
 if (readFile == NULL)
 {
  return NULL;
 }
 pal = IPaletteCreate();
 colorNames = (char **)IMalloc(sizeof(char *) * 255);
 for (i = 0; !feof(readFile);)
 {
  fgets(line, 255, readFile);
  if ((line[0]) && (line[0] != '#') && (line[0] != '\n'))
  {
   sscanf(line, "%d%d%d%s", &r, &g, &b, tmpName);
   IPaletteSet(pal, i, r / 4, g / 4, b / 4);
   colorNames[i] = strdup(tmpName);
   i++;
  }
 }
 for ( ; i < 255; i++)
 {
  IPaletteSet(pal, i, 0, 0, 0);
  colorNames[i] = NULL;
 }
 fclose(readFile);
 img = (IImage)IMalloc(sizeof(struct IImageStruct));
 readFile = fopen(filename, "r");
 if (readFile == NULL)
 {
  return NULL;
 }
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
 for (i = (img->x * img->y), curPixel = img->pic, k = 0, fgets(line, 255, readFile);
   i > 0; i--, curPixel++, k++)
 {
  if (k == 14)
  {
   fgets(line, 255, readFile);
   k = 0;
  }
  sscanf(line + k * 5, "%d", curPixel);
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
   for (k = 0;
     (k < 255) && ((!colorNames[k]) || (strcmp(tmpName, colorNames[k])));
     k++)
   {
   }
   convert[i] = k;
  }
 } while (!feof(readFile));
 for (i = (img->x * img->y), curPixel = img->pic; i > 0; i--, curPixel++)
 {
  if (*curPixel != 255)
  {
   *curPixel = convert[*curPixel];
  }
 }
 img->pal = pal;
 for (i = 0; i < 255; i++)
 {
  if (colorNames[i] != NULL)
   free(colorNames[i]);
 }
 IFree(colorNames);
/* fclose(readFile);*/
 return img;
}

