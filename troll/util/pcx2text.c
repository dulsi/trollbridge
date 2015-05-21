#include <igrimage.h>
#include <stdio.h>
#include <unistd.h>

void ParseCommandLine(int argc, char *argv[], char *inname, char *outname);
void IImageTxtSave(IImage img, const char *filename);

int main(int argc, char *argv[])
{
 char inname[255];
 char outname[255];
 IImage img;

 ParseCommandLine(argc, argv, inname, outname);
 img = IImagePCXLoad(inname);
 IImageTxtSave(img, outname);
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
pcx2text inname[.pcx] [outname[.txt]]\n\
\n\
  inname[.pcx]     Name of input file\n\
  [outname[.txt]]  Name of output file (defaults to input file name)\n\
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

void IImageTxtSave(IImage img, const char *filename)
{
 ILong i, k;
 IPixel IFAR *curPixel;
 IColor r, g, b;
 FILE *writeFile;
 IUByte usedColor[255];

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
   fprintf(writeFile, "%3d =  %3d  %3d  %3d\n", i, r, g, b);
  }
 }
 fclose(writeFile);
}

