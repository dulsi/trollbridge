/********************************************************************
  <createmap.c> -- Creates a map file

  Programmer -- Dennis Payne

  Date -- 11/22/97
********************************************************************/

#include <typeinfo>
#include <string>
#include <map.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <istdlib.h>
#include <igrbasics.h>
#include <trollconst.h>
#include <unistd.h>
#include <getopt.h>
#include "iextra.h"

#define BUFFER_SIZE 1024

static unsigned char IFAR *buffer;

void ParseCommandLine(int argc,char *argv[],char *filename, IPalette *pal,
  IPaletteName *palnm);
char IFAR * IFAR *MapFileRead(char *filename, IPaletteName colornames);
void TrbFileWrite(char *filename, char IFAR * IFAR * screenfile);

IUShort xScreen;
IUShort yScreen;
IUShort xStart;
IUShort yStart;
IUByte mapInfo[TROLL_LEVEL_X][TROLL_LEVEL_Y];

main(int argc,char *argv[])
{
 char basename[255];
 char filename[255];
 char IFAR * IFAR * screenfile;
 IPalette pal = NULL;
 IPaletteName colornames = NULL;

 for (int x = 0; x < TROLL_LEVEL_X; x++)
  for (int y = 0; y < TROLL_LEVEL_Y; y++)
   mapInfo[x][y] = 0;
 ParseCommandLine(argc, argv, basename, &pal, &colornames);
 strcpy(filename, basename);
 strcat(filename,".map");
 screenfile = MapFileRead(filename, colornames);
 strcpy(filename, basename);
 strcat(filename,".trb");
 TrbFileWrite(filename, screenfile);
}

void ParseCommandLine(int argc,char *argv[],char *filename, IPalette *pal,
  IPaletteName *palnm)
{
 char opt;
 int i;
 char *defpal;
 static struct option long_options[] =
 {
  {"pal", 1, 0, 'p'},
  {0, 0, 0, 0}
 };

 while ((opt=getopt_long(argc,argv,"h?p:", long_options, NULL))!=EOF)
 {
  switch (opt)
  {
   case 'h':
   case '?':
    printf("\
Creates a map file\n\
\n\
createmap [options] filename[.map]\n\
\n\
  Options:\n\
    -p, --pal=TEXTPALETTE  Text palette for color names\n\
\n\
  filename[.map]  Name of map file\n\
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
 if (argc-optind != 1)
 {
  printf("Incorrect number of parameters\n");
  exit(4);
 }
 strcpy(filename,argv[optind]);
 if (((i = strlen(filename)) > 4) && (strcmp(filename + i - 4,".map") == 0))
  filename[i - 4] = 0;
}

char IFAR * IFAR *MapFileRead(char *filename, IPaletteName colornames)
{
 FILE *mapfile;
 char basename[255];
 char IFAR * IFAR *screenfile;
 int i, k;

 if ((mapfile = fopen(filename, "r")) == NULL)
 {
  printf("Error reading file -- %s\n", filename);
  exit(1);
 }
 fscanf(mapfile, "%s%d", basename, &xScreen);
 fscanf(mapfile, "%s%d", basename, &yScreen);
 fscanf(mapfile, "%s%d", basename, &xStart);
 fscanf(mapfile, "%s%d", basename, &yStart);
 /* Read and throw out column numbers */
 for (i = 0; i < TROLL_LEVEL_X; i++)
 {
  fscanf(mapfile, "%s", basename);
 }
 for (i = 0; i < TROLL_LEVEL_Y; i++)
 {
  /* Read and throw out row number */
  fscanf(mapfile, "%s", basename);
  for (k = 0; k < TROLL_LEVEL_X; k++)
  {
   fscanf(mapfile, "%s", basename);
   mapInfo[k][i] = IPaletteNameFind(colornames, basename);
  }
 }
 /* Read and throw out column numbers */
 for (i = 0; i < TROLL_LEVEL_X; i++)
 {
  fscanf(mapfile, "%s", basename);
 }
 screenfile =
   (char IFAR * IFAR *)IMalloc(TROLL_LEVEL_X * TROLL_LEVEL_Y * sizeof(char IFAR *));
 for (i = 0; i < TROLL_LEVEL_Y; i++)
 {
  /* Read and throw out row number */
  fscanf(mapfile, "%s", basename);
  for (k = 0; k < TROLL_LEVEL_X; k++)
  {
   fscanf(mapfile, "%s", basename);
   screenfile[k * TROLL_LEVEL_Y + i] = (char IFAR *)IMalloc(strlen(basename) + 5);
   strcpy(screenfile[k * TROLL_LEVEL_Y + i], basename);
   strcat(screenfile[k * TROLL_LEVEL_Y + i], ".scr");
  }
 }
 return screenfile;
}

class Entry
{
 public:
  Entry() : where(0), size(0) {};

  IULong where;
  IULong size;
};

void TrbFileWrite(char *filename, char IFAR * IFAR * screenfile)
{
 FILE *trbfile;
 FILE *input;
 long IFAR *header;
 long i, k;
 map<string, Entry, less<string> > describe;
 Entry cur;

 if ((trbfile = fopen(filename, "wb")) == NULL)
 {
  printf("Error writing file -- %s\n",filename);
  exit(1);
 }
 if ((buffer = (unsigned char IFAR *)IMalloc(BUFFER_SIZE)) == NULL)
 {
  printf("Falled to allocate memory\n");
  exit(2);
 }
 if ((header = (long IFAR *)IMalloc(sizeof(long)*TROLL_LEVEL_X*TROLL_LEVEL_Y*2)) == NULL)
 {
  printf("Falled to allocate memory\n");
  exit(2);
 }
 fwrite(&xScreen, 1, sizeof(IUShort), trbfile);
 fwrite(&yScreen, 1, sizeof(IUShort), trbfile);
 fwrite(&xStart, 1, sizeof(IUShort), trbfile);
 fwrite(&yStart, 1, sizeof(IUShort), trbfile);
 fwrite(mapInfo, 1, sizeof(IUByte) * TROLL_LEVEL_X * TROLL_LEVEL_Y, trbfile);
 fwrite(header, 1, sizeof(long) * TROLL_LEVEL_X * TROLL_LEVEL_Y * 2, trbfile);
 cur.where = sizeof(long) * TROLL_LEVEL_X * TROLL_LEVEL_Y * 2 +
     sizeof(IUByte) * TROLL_LEVEL_X * TROLL_LEVEL_Y + 4 * sizeof(IUShort);
 for (i = 0; i < TROLL_LEVEL_X * TROLL_LEVEL_Y; i++)
 {
  Entry theEntry = describe[screenfile[i]];
  if (theEntry.where != 0)
  {
   header[i * 2] = theEntry.where;
   header[i * 2 + 1] = theEntry.size;
  }
  else
  {
   header[i * 2] = cur.where;
   header[i * 2 + 1] = 0;
   input = fopen(screenfile[i], "rb");
   do
   {
    k = fread(buffer, 1, BUFFER_SIZE, input);
    fwrite(buffer, 1, k, trbfile);
    header[i * 2 + 1] += k;
   } while (k == BUFFER_SIZE);
   fclose(input);
   cur.size = header[i * 2 + 1];
   describe[screenfile[i]] = cur;
   cur.where += cur.size;
  }
 }
 fseek(trbfile, sizeof(IUByte) * TROLL_LEVEL_X * TROLL_LEVEL_Y + 4 * sizeof(IUShort), SEEK_SET);
 fwrite(header, 1, sizeof(long) * TROLL_LEVEL_X * TROLL_LEVEL_Y * 2, trbfile);
 fclose(trbfile);
}

