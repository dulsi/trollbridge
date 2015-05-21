/********************************************************************
  <pcx2zsp.c> -- converts a collection of pcx files into a zelda sprite

  Programmer -- Dennis Payne

  Date -- 1/10/96

  Modification History
  
  2/27/96 Implemented support for new compressed zelda sprites.

  4/30/96 Modularized it somewhat but broke dos version

  3/13/99 Moved to new Identical Graphics Library
********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "unistd.h"
#include "igrimage.h"

void parsecmdl(int argc,char *argv[],char *filename,unsigned short *fc,unsigned short *fr);
int fputzimg(FILE *zspfile,unsigned short x,unsigned short y,unsigned char IFAR *image);

main(int argc,char *argv[])
{
 int i,k;
 IImage image;
 IUShort x,y,facing,frames;
 char filename[255];
 char tmpname[255];
 FILE *zspfile;
 IUShort *szpic;

 parsecmdl(argc,argv,filename,&facing,&frames);
 sprintf(tmpname,"%s.zsp",filename);
 if ((zspfile=fopen(tmpname,"wb+"))==NULL) {
  printf("Error creating file -- %s\n",tmpname);
  exit(1);
 }
 fwrite(&x,1,sizeof(x),zspfile);
 fwrite(&y,1,sizeof(y),zspfile);
 fwrite(&facing,1,sizeof(facing),zspfile);
 fwrite(&frames,1,sizeof(frames),zspfile);
 szpic = (IUShort *)IMalloc(facing * frames * sizeof(IUShort));
 for (i = 0; i < facing * frames; i++)
 {
  fwrite(&szpic[i],sizeof(szpic[i]),1,zspfile);
 }
 for (i=0;i<facing;i++)
  for (k=0;k<frames;k++)
  {
   sprintf(tmpname,"%s%c%d.pcx",filename,i+'a',k);
   if ((image = IImagePCXLoad(tmpname)) == NULL)
   {
    printf("Error loading %s\n",tmpname);
    exit(1);
   }
   szpic[i*frames+k]=fputzimg(zspfile,image->x,image->y,image->pic);
   x = image->x;
   y = image->y;
   IImageDestroy(image);
  }
 fseek(zspfile,0,SEEK_SET);
 fwrite(&x,1,sizeof(x),zspfile);
 fwrite(&y,1,sizeof(y),zspfile);
 fwrite(&facing,1,sizeof(facing),zspfile);
 fwrite(&frames,1,sizeof(frames),zspfile);
 for (i=0;i<facing*frames;i++)
 {
  fwrite(&szpic[i],sizeof(szpic[i]),1,zspfile);
 } 
 fclose(zspfile);
 return 0;
}

void parsecmdl(int argc,char *argv[],char *filename,unsigned short *fc,unsigned short *fr)
{
 char opt;
 int i;

 while ((opt=getopt(argc,argv,"h?"))!=EOF)
 {
  switch (opt)
  {
   case 'h':
   case '?':
    printf("\
Converts a collection of pcx files to a zelda sprite file\n\
\n\
pcx2zsp filename[.zsp] facings frames\n\
\n\
  filename[.zsp]  Name of output file\n\
  facings         Number of facings\n\
  frames          Number of frames\n\
");
    exit(0);
    break;
   default:
    break;
  }
 }
 if (argc-optind!=3)
 {
  printf("Incorrect number of parameters\n");
  exit(4);
 }
 strcpy(filename,argv[optind]);
 if (((i=strlen(filename))>4) && (strcmp(filename+i-4,".zsp")==0))
  filename[i-4]=0;
 *fc=atoi(argv[optind+1]);
 *fr=atoi(argv[optind+2]);
}

int fputzimg(FILE *zspfile,unsigned short x,unsigned short y,unsigned char IFAR *image)
{
 int xs,ys,szpic;
 unsigned char cur,nxt,num,sz;

 szpic=0;
 for (ys=0;ys<y;ys++)
 {
  fputc(0,zspfile);
  num=sz=0;
  cur=image[ys*x];
  for (xs=1;xs<x;xs++)
  {
   nxt=image[ys*x+xs];
   if (nxt==cur)
   {
    if (num==126)
    {
     fputc(num|128,zspfile);
     fputc(cur,zspfile);
     sz+=2;
     num=0;
    }
    else
     num++;
   }
   else
   {
    if ((num!=0) || ((cur&128) && (cur!=255)))
    {
     fputc(num|128,zspfile);
     sz++;
    }
    fputc(cur,zspfile);
    sz++;
    num=0;
    cur=nxt;
   }
  }
  if ((num!=0) || ((cur&128) && (cur!=255)))
  {
   fputc(num|128,zspfile);
   sz++;
  }
  fputc(cur,zspfile);
  sz++;
  szpic+=sz+1;
  fseek(zspfile,-sz-1,SEEK_CUR);
  fputc(sz,zspfile);
  fseek(zspfile,sz,SEEK_CUR);
 }
 return szpic;
}

