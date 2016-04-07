#include <igrimage.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <SDL.h>
#include "iextra.h"

void ParseCommandLine(int argc, char *argv[], char *inname, char *outname,
  IPalette *pal, IPaletteName *palnm);

int main(int argc, char *argv[])
{
 char inname[255];
 char outname[255];
 IImage img = NULL;
 IPalette pal = NULL;
 IPaletteName palnm = NULL;

 if (SDL_Init(SDL_INIT_VIDEO) < 0)
 {
  printf("Failed - SDL_Init\n");
  exit(0);
 }
 ParseCommandLine(argc, argv, inname, outname, &pal, &palnm);
 if (strcmp(inname + strlen(inname) - 4, ".pcx") == 0)
 {
  img = IImagePCXLoad(inname);
 }
 else
 {
  img = IImageLoad(inname);
 }
 IImageTextSave(img, outname, pal, palnm);
 IPaletteDestroy(pal);
 IPaletteNameDestroy(palnm);
 IImageDestroy(img);
}

void ParseCommandLine(int argc, char *argv[], char *inname, char *outname,
  IPalette *pal, IPaletteName *palnm)
{
 char opt;
 int i;
 char *defpal;
 static struct option long_options[] =
 {
  {"pal", 1, 0, 'p'},
  {0, 0, 0, 0}
 };

 while ((opt = getopt_long(argc, argv,"h?p:", long_options, NULL))!=EOF)
 {
  switch (opt)
  {
   case 'h':
   case '?':
    printf("\
Converts a pcx file into a text file\n\
\n\
pcx2text [options] inname[.pcx] [outname[.txt]]\n\
\n\
  Options:\n\
    -p, --pal=TEXTPALETTE    Text palette for color names\n\
\n\
  inname[.pcx]     Name of input file\n\
  [outname[.txt]]  Name of output file (defaults to input file name)\n\
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
   default:
    break;
  }
 }
 if (!(*pal))
 { /* Check environment variable for default text palette */
  if ((defpal = getenv("TEXT_PALETTE")) != NULL)
  {
   IPaletteTextLoad(pal, palnm, defpal);
  }
  if (!(*pal))
  { /* Create a blank palette and palette name */
   *pal = IPaletteCreate();
   *palnm = IPaletteNameCreate();
  }
 }
 if ((argc - optind < 1) || (argc - optind > 2))
 {
  printf("Incorrect number of parameters\n");
  exit(4);
 }
 strcpy(inname, argv[optind]);
 size_t len = strlen(inname);
 if ((len < 4) || ((strcmp(inname + len - 4, ".pcx") != 0) && (strcmp(inname + len - 4, ".png") != 0)))
 {
  strcat(inname, ".pcx");
 }
 if (argc - optind == 2)
 {
  strcpy(outname, argv[optind + 1]);
  if (strcmp(outname + strlen(outname) - 4, ".txt") != 0)
  {
   strcat(inname, ".txt");
  }
 }
 else
 {
  strcpy(outname, inname);
  strcpy(outname + strlen(outname) - 3, "txt");
 }
}

