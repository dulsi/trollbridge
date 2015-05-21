/********************************************************************
  <mergemap.c> -- merges a map file

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
#include "unistd.h"

#define BUFFER_SIZE 1024

static unsigned char IFAR *buffer;

void ParseCommandLine(int argc,char *argv[],char *filename);
char **TextPalFileRead(char *filename);
char IFAR * IFAR *MapFileRead(char *filename, char **colornames);
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
 char **colornames;

 for (int x = 0; x < TROLL_LEVEL_X; x++)
  for (int y = 0; y < TROLL_LEVEL_Y; y++)
   mapInfo[x][y] = 0;
 ParseCommandLine(argc, argv, basename);
 strcpy(filename, basename);
 strcat(filename,".map");
 colornames = TextPalFileRead("/proj/troll/data/trollpal.txt");
 screenfile = MapFileRead(filename, colornames);
 strcpy(filename, basename);
 strcat(filename,".trb");
 TrbFileWrite(filename, screenfile);
}

void ParseCommandLine(int argc,char *argv[],char *filename)
{
 char opt;
 int i;

 while ((opt=getopt(argc,argv,"h?"))!=EOF) {
  switch (opt) {
   case 'h':
   case '?':
    printf("\
Creates a map file\n\
\n\
createmap filename[.map]\n\
\n\
  filename[.map]  Name of map file\n\
");
    exit(0);
    break;
   default:
    break;
  }
 }
 if (argc-optind!=1) {
  printf("Incorrect number of parameters\n");
  exit(4);
 }
 strcpy(filename,argv[optind]);
 if (((i=strlen(filename))>4) && (strcmp(filename+i-4,".map")==0))
  filename[i-4]=0;
}

char **TextPalFileRead(char *filename)
{
 ILong i, k;
 char **colorNames;
 FILE *readFile;
 int r, g, b;
 char line[255];
 char tmpName[255];
 IPalette pal;

 readFile = fopen(filename, "r");
 if (readFile == NULL)
 {
  readFile = fopen("trollpal.txt", "r");
  if (readFile == NULL)
  {
   return NULL;
  }
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
 IPaletteDestroy(pal);
 return colorNames;
}

char IFAR * IFAR *MapFileRead(char *filename, char **colornames)
{
 FILE *mapfile;
 char basename[255];
 char IFAR * IFAR *screenfile;
 int i, k, j;

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
   for (j = 0; j < 256; j++)
   {
    if (!colornames[j])
    {
     j = 256;
    }
    else if (strcmp(colornames[j], basename) == 0)
    {
     mapInfo[k][i] = j;
     j = 256;
    }
   }
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

