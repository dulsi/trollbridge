#include <igrimage.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include "iextra.h"

IPixel transparent[3] = {255, 0, 181};

void ParseCommandLine(int argc, char *argv[], char *inname, char *outname,
  IPalette *pal, IPaletteName *palnm, int &shift);

int main(int argc, char *argv[])
{
 char inname[255];
 char outname[255];
 IImage img = NULL;
 IPalette pal = NULL;
 IPaletteName palnm = NULL;
 int shift = 0;

 ParseCommandLine(argc, argv, inname, outname, &pal, &palnm, shift);
 IPaletteSet(pal, 255, transparent[0], transparent[1], transparent[2]);
 img = IImageTextLoad(inname, pal, palnm);
 for (int i = 0; i < img->x * img->y; i++)
 {
  if (img->pic[i] != 255)
   img->pic[i] += shift;
 }
 IImagePCXSave(img, outname);
 IImageDestroy(img);
 IPaletteDestroy(pal);
 IPaletteNameDestroy(palnm);
}

void ParseCommandLine(int argc, char *argv[], char *inname, char *outname,
  IPalette *pal, IPaletteName *palnm, int &shift)
{
 char opt;
 int i;
 char *defpal;
 static struct option long_options[] =
 {
  {"pal", 1, 0, 'p'},
  {"shift", 1, 0, 's'},
  {0, 0, 0, 0}
 };

 while ((opt = getopt_long(argc,argv,"h?p:s:", long_options, NULL)) != EOF)
 {
  switch (opt)
  {
   case 'h':
   case '?':
    printf("\
Converts a pcx file into a text file\n\
\n\
text2pcx [options] inname[.txt] [outname[.pcx]]\n\
\n\
  Options:\n\
    -p, --pal=TEXTPALETTE  Text palette for color names\n\
    -s, --shift=COLORSHIFT  Shifts the colors of the image\n\
\n\
  inname[.txt]             Name of input file\n\
  [outname[.pcx]]          Name of output file (defaults to input file name)\n\
\n\
  Environment:\n\
    TEXT_PALETTE           Default text palette\n\
");
    exit(0);
    break;
   case 'p':
    if (*pal)
    {
     printf("Only one palette may be specified.\n");
     exit(5);
    }
    if (optarg)
    {
     IPaletteTextLoad(pal, palnm, optarg);
     if (!(*pal))
     {
      printf("Error reading text palette file: %s\n", optarg);
      exit(6);
     }
    }
    break;
   case 's':
    if (optarg)
    {
     shift = atoi(optarg);
    }
    break;
   default:
    break;
  }
 }
 if (!(*pal))
 { /* Check environment variable for default text palette */
  if ((defpal = getenv("TEXT_PALETTE")) == NULL)
  {
   printf("Error no palette specified.\n");
   exit(7);
  }
  IPaletteTextLoad(pal, palnm, defpal);
  if (!(*pal))
  {
   printf("Error reading text palette file: %s\n", optarg);
   exit(6);
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

