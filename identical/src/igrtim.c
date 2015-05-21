/*-------------------------------------------------------------------------*\
  <igrtim.c> -- Identical Software's Tim Routines

  Date      Programmer  Description
  10/23/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include <istdlib.h>
#include <igrbasics.h>
#include <igrimage.h>
#include <igrtim.h>
#include <stdio.h>

ITim IImage2Tim(IImage img, IUShort bpp, IUShort px, IUShort py, IUShort cx,
  IUShort cy, IBool transparent, IColor r, IColor g, IColor b)
{
 ITim tim;
 int i, k;

 /* 8 bit per pixel images must have a width divisible by 2. */
 /* 4 bit per pixel images must have a width divisible by 4. */
 /* When converting to 4 bit per pixel it does no checks on used colors.
   Only the first 16 are saved. */
 if ((bpp == ITIM_16BPP) || (bpp == ITIM_24BPP))
 {
  /* Conversion to 16 or 24 bit images not supported at this time. */
  return NULL;
 }
 tim = (ITim)IMalloc(sizeof(struct ITimStruct));
 if (tim == NULL)
 {
  return NULL;
 }
 tim->flag = bpp;
 tim->cx = cx;
 tim->cy = cy;
 tim->px = px;
 tim->py = py;
 if (img->pal)
 {
  tim->flag |= 0x00000008;
  tim->ch = 1;
  tim->cw = ((bpp == ITIM_8BPP) ? 256 : 16);
  tim->clut = (IUShort IFAR *)IMalloc(tim->cw * tim->ch * sizeof(IUShort));
  for (i = 0; i < tim->cw; i++)
  {
   if ((transparent) && ((*img->pal)[i][0] == r) &&
     ((*img->pal)[i][1] == g) && ((*img->pal)[i][2] == b))
   {
    tim->clut[i] = 0x0000;
   }
   else
   {
    tim->clut[i] = (((IUShort)((*img->pal)[i][2] & 0x3E)) << 9) +
      (((IUShort)((*img->pal)[i][1] & 0x3E)) << 4) +
      (((IUShort)((*img->pal)[i][0] & 0x3E)) >> 1);
    if (tim->clut[i] == 0)
    {
     tim->clut[i] = 0x8000;
    }
   }
  }
 }
 else
 {
  tim->cw = tim->ch = 0;
  tim->clut = NULL;
 }
 tim->ph = img->y;
 tim->pw = img->x;
 if (bpp == ITIM_8BPP)
 {
  tim->pw /= 2;
 }
 else if (bpp == ITIM_4BPP)
 {
  tim->pw /= 4;
 }
 tim->pic = (IUShort IFAR *)IMalloc(tim->pw * tim->ph * sizeof(IUShort));
 for (i = 0; i < tim->ph; i++)
 {
  for (k = 0; k < tim->pw; k++)
  {
   if (bpp == ITIM_8BPP)
   {
    tim->pic[i * tim->pw + k] = (img->pic[i * img->x + k * 2 + 1] << 8) +
      img->pic[i * img->x + k * 2];
   }
   else if (bpp == ITIM_4BPP)
   {
    tim->pic[i * tim->pw + k] =
      ((img->pic[i * img->x + k * 4 + 3] & 0x0F) << 12) +
      ((img->pic[i * img->x + k * 4 + 2] & 0x0F) << 8) +
      ((img->pic[i * img->x + k * 4 + 1] & 0x0F) << 4) +
      (img->pic[i * img->x + k * 4] & 0x0F);
   }
  }
 }
 return tim;
}

IImage ITim2Image(ITim tim, IUShort palnum, IColor r, IColor g, IColor b)
{
 IImage img;
 IUShort tmp;
 int i, k;

 if ((ITimIs16bit(tim)) || (ITimIs24bit(tim)))
 {
  /* Conversion of 16 or 24 bit images not supported at this time. */
  return NULL;
 }
 img = (IImage)IMalloc(sizeof(struct IImageStruct));
 if (img == NULL)
 {
  return NULL;
 }
 img->x = tim->pw;
 if (ITimIs8bit(tim))
 {
  img->x *= 2;
 }
 else if (ITimIs4bit(tim))
 {
  img->x *= 4;
 }
 img->y = tim->ph;
 img->pic = (IPixel IFAR *)IMalloc(img->x * img->y);
 if (img->pic == NULL)
 {
  IFree(img);
  return NULL;
 }
 for (i = 0; i < tim->ph; i++)
 {
  for (k = 0; k < tim->pw; k++)
  {
   if (ITimIs8bit(tim))
   {
    img->pic[i * img->x + k * 2] = tim->pic[i * tim->pw + k] & 0x00FF;
    img->pic[i * img->x + k * 2 + 1] = (tim->pic[i * tim->pw + k] >> 8) & 0x00FF;
   }
   else if (ITimIs4bit(tim))
   {
    img->pic[i * img->x + k * 4] = tim->pic[i * tim->pw + k] & 0x000F;
    img->pic[i * img->x + k * 4 + 1] = (tim->pic[i * tim->pw + k] >> 4) & 0x000F;
    img->pic[i * img->x + k * 4 + 2] = (tim->pic[i * tim->pw + k] >> 8) & 0x000F;
    img->pic[i * img->x + k * 4 + 3] = (tim->pic[i * tim->pw + k] >> 12) & 0x000F;
   }
  }
 }
 img->pal = NULL;
 if ((palnum != ITIM_PAL_NONE) &&
   (tim->clut) && (palnum < tim->ch))
 {
  img->pal = IPaletteCreate();
  if (img->pal == NULL)
  {
   IFree(img->pic);
   IFree(img);
   return NULL;
  }
  for (i = 0; i < 256; i++)
  {
   if (i < tim->cw)
   {
    tmp = tim->clut[palnum * tim->cw + i];
    if (tmp == 0x0000)
    {
     IPaletteSet(img->pal, i, r, g, b);
    }
    else
    {
     IPaletteSet(img->pal, i, ((tmp & 0x001F) << 1), ((tmp & 0x03E0) >> 4),
       ((tmp & 0x7C00) >> 9));
    }
   }
   else
   {
    IPaletteSet(img->pal, i, 0, 0, 0);
   }
  }
 }
 return img;
}

ITim ITimLoad(const char *filename)
{
 FILE *timfile;
 IULong id;
 IULong tmp;
 int i, k;
 ITim tim;

 /* BIG EIDIAN: Won't work with the save routine either */
 timfile = fopen(filename, "rb");
 if (timfile == NULL)
 {
  return NULL;
 }
 tim = (ITim)IMalloc(sizeof(struct ITimStruct));
 fread(&id, 1, sizeof(id), timfile);
 fread(&tim->flag, 1, sizeof(tim->flag), timfile);
 if (tim->flag & 0x00000008)
 {
  fread(&tmp, 1, sizeof(tmp), timfile);
  fread(&tim->cx, 1, sizeof(tim->cx), timfile);
  fread(&tim->cy, 1, sizeof(tim->cy), timfile);
  fread(&tim->cw, 1, sizeof(tim->cw), timfile);
  fread(&tim->ch, 1, sizeof(tim->ch), timfile);
  tim->clut = (IUShort IFAR *)IMalloc(tim->cw * tim->ch * sizeof(IUShort));
  for (i = 0; i < tim->ch; i++)
  {
   for (k = 0; k < tim->cw; k++)
   {
    fread(&tim->clut[i * tim->cw + k], 1, sizeof(IUShort), timfile);
   }
  }
 }
 else
 {
  tim->cx = tim->cy = tim->cw = tim->ch = 0;
  tim->clut = NULL;
 }
 fread(&tmp, 1, sizeof(tmp), timfile);
 fread(&tim->px, 1, sizeof(tim->px), timfile);
 fread(&tim->py, 1, sizeof(tim->py), timfile);
 fread(&tim->pw, 1, sizeof(tim->pw), timfile);
 fread(&tim->ph, 1, sizeof(tim->ph), timfile);
 tim->pic = (IUShort IFAR *)IMalloc(tim->pw * tim->ph * sizeof(IUShort));
 for (i = 0; i < tim->ph; i++)
 {
  for (k = 0; k < tim->pw; k++)
  {
   fread(&tim->pic[i * tim->pw + k], 1, sizeof(IUShort), timfile);
  }
 }
 fclose(timfile);
 return tim;
}

void ITimSave(ITim tim, const char *filename)
{
 FILE *timfile;
 IULong id = 0x00000010;
 IULong tmp;
 int i, k;

 /* BIG EIDIAN: Won't work with the save routine either */
 timfile = fopen(filename, "wb");
 if (timfile == NULL)
 {
  return ;
 }
 fwrite(&id, 1, sizeof(id), timfile);
 fwrite(&tim->flag, 1, sizeof(tim->flag), timfile);
 if (tim->flag & 0x00000008)
 {
  tmp = tim->ch * tim->cw * 2 /*CLUT entries*/ + 4 /*bnum*/ + 4 /*dx/dy*/ + 4 /*h/w*/;
  fwrite(&tmp, 1, sizeof(tmp), timfile);
  fwrite(&tim->cx, 1, sizeof(tim->cx), timfile);
  fwrite(&tim->cy, 1, sizeof(tim->cy), timfile);
  fwrite(&tim->cw, 1, sizeof(tim->cw), timfile);
  fwrite(&tim->ch, 1, sizeof(tim->ch), timfile);
  for (i = 0; i < tim->ch; i++)
  {
   for (k = 0; k < tim->cw; k++)
   {
    fwrite(&tim->clut[i * tim->cw + k], 1, sizeof(IUShort), timfile);
   }
  }
 }
 tmp = tim->ph * tim->pw * 2 + 4 /*bnum*/ + 4 /*dx/dy*/ + 4 /*h/w*/;
 fwrite(&tmp, 1, sizeof(tmp), timfile);
 fwrite(&tim->px, 1, sizeof(tim->px), timfile);
 fwrite(&tim->py, 1, sizeof(tim->py), timfile);
 fwrite(&tim->pw, 1, sizeof(tim->pw), timfile);
 fwrite(&tim->ph, 1, sizeof(tim->ph), timfile);
 for (i = 0; i < tim->ph; i++)
 {
  for (k = 0; k < tim->pw; k++)
  {
   fwrite(&tim->pic[i * tim->pw + k], 1, sizeof(IUShort), timfile);
  }
 }
 fclose(timfile);
}

void ITimDestroy(ITim tim)
{
 if (tim->pic)
 {
  IFree(tim->pic);
 }
 if (tim->clut)
 {
  IFree(tim->clut);
 }
}

