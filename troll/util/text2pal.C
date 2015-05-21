#include <igrbasics.h>
#include <igrpalette.h>
#include <unistd.h>

void ParseCommandLine(int argc, char *argv[], char *inname, char *outname);

int main(int argc, char *argv[])
{
 char inname[255];
 char outname[255];
 IPalette pal;
 IPaletteName palnm;

 ParseCommandLine(argc, argv, inname, outname);
 IPaletteTextLoad(&pal, &palnm, inname);
 if (pal)
 {
  IPalettePalSave(pal, outname);
 }
}

void ParseCommandLine(int argc, char *argv[], char *inname, char *outname)
{
 char opt;
 int i;

 while ((opt=getopt(argc,argv,"h?")) != -1) {
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

