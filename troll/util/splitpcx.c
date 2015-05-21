#include <igrbasics.h>
#include <igrimage.h>
#include <trollconst.h>
#include <stdio.h>
#include <getopt.h>
#include "iextra.h"

void ParseCommandLine(int argc, char *argv[], char *inname, char *outname);

int main(int argc, char **argv)
{
 char inname[255];
 char outname[255];
 IImage img;
 IImage img2;
 int i,k,n;
 char filename[80];

 ParseCommandLine(argc, argv, inname, outname);
 img = IImagePCXLoad(inname);
 for (i = 0, n = 0; i < img->y; i += TROLL_SQUARE_Y)
 {
  for (k = 0; k < img->x; k += TROLL_SQUARE_X, n++)
  {
   sprintf(filename, "%s%d.pcx", outname, n);
   img2 = IImageSubCapture(img, k, i, k + TROLL_SQUARE_X - 1,
     i + TROLL_SQUARE_Y - 1);
   IImagePaletteSet(img2, IImagePaletteGet(img));
   IImagePCXSave(img2, filename);
   IImageDestroy(img2);
  }
 }
 IImageDestroy(img);
 return 0;
}

void ParseCommandLine(int argc, char *argv[], char *inname, char *outname)
{
 char opt;
 int i;
 static struct option long_options[] =
 {
  {0, 0, 0, 0}
 };

 while ((opt = getopt_long(argc, argv,"h?", long_options, NULL))!=EOF)
 {
  switch (opt)
  {
   case 'h':
   case '?':
    printf("\
Splits a pcx file into several pcx files\n\
\n\
splitpcx inname[.pcx] [outprefix]]\n\
\n\
  inname[.pcx]     Name of input file\n\
  [outprefix]      Name of prefix for output files\n\
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
 if (strcmp(inname + strlen(inname) - 4, ".pcx") != 0)
 {
  strcat(inname, ".pcx");
 }
 if (argc - optind == 2)
 {
  strcpy(outname, argv[optind + 1]);
 }
 else
 {
  strcpy(outname, inname);
  outname[strlen(outname) - 4] = 0;
 }
}

