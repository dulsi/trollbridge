#include <igrbasics.h>
#include <stdio.h>
#include <unistd.h>
#include "file.h"

void ParseCommandLine(int argc, char *argv[], char *inname, char *outname);
IPalette IPaletteTextLoad(const char *filename);
void IPalettePalSave(IPalette pal, const char *filename);

int main(int argc, char *argv[])
{
 char inname[255];
 char outname[255];
 IPalette pal;

 ParseCommandLine(argc, argv, inname, outname);
 pal = IPaletteTextLoad(inname);
 IPalettePalSave(pal, outname);
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
Converts a text file into a palette file\n\
\n\
text2pal inname[.txt] [outname[.pal]]\n\
\n\
  inname[.txt]     Name of input file\n\
  [outname[.pal]]  Name of output file (defaults to input file name)\n\
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
  if (strcmp(outname + strlen(outname) - 4, ".pal") != 0)
  {
   strcat(inname, ".pal");
  }
 }
 else
 {
  strcpy(outname, inname);
  strcpy(outname + strlen(outname) - 3, "pal");
 }
}

IPalette IPaletteTextLoad(const char *filename)
{
 FILE *readFile;
 int r, g, b, i;
 char line[255];
 IPalette pal;
 
 readFile = fopen(filename, "r");
 if (readFile == NULL)
 {
  return NULL;
 }
 pal = IPaletteCreate();
 for (i = 0; !feof(readFile);)
 {
  fgets(line, 255, readFile);
  if ((line[0]) && (line[0] != '#') && (line[0] != '\n'))
  {
   sscanf(line, "%d%d%d", &r, &g, &b);
   IPaletteSet(pal, i, r / 4, g / 4, b / 4);
   i++;
  }
 }
 for ( ; i < 256; i++)
 {
  IPaletteSet(pal, i, 0, 0, 0);
 }
 return pal;
}

void IPalettePalSave(IPalette pal, const char *filename)
{
 IColor r, g, b;
 int i;
 BinaryWriteFile writeFile(filename);

 for (i = 0; i < 256; i++)
 {
  IPaletteGet(pal, i, &r, &g, &b);
  writeFile.writeByte(r);
  writeFile.writeByte(g);
  writeFile.writeByte(b);
 }
}

