/*-------------------------------------------------------------------------*\
  <definition.C> -- Implemention for TrollDefinition methods

  Date      Programmer  Description
  12/31/00  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "troll.h"

TrollDefinition::TrollDefinition(const char *filename)
 : dllFile(NULL), paletteFile(NULL), spriteFile(NULL), startFile(NULL),
  titleFile(NULL)
{
 FILE *f;
 char line[256];
 int i;

 if (f = fopen(filename, "r"))
 {
  if (fgets(line, 256, f))
  {
   for (i = strlen(line) - 1; (i >= 0) && (isspace(line[i])); i--)
    ;
   line[i + 1] = 0;
   dllFile = strdup(line);
  }
  if (fgets(line, 256, f))
  {
   for (i = strlen(line) - 1; (i >= 0) && (isspace(line[i])); i--)
    ;
   line[i + 1] = 0;
   paletteFile = strdup(line);
  }
  if (fgets(line, 256, f))
  {
   for (i = strlen(line) - 1; (i >= 0) && (isspace(line[i])); i--)
    ;
   line[i + 1] = 0;
   spriteFile = strdup(line);
  }
  if (fgets(line, 256, f))
  {
   for (i = strlen(line) - 1; (i >= 0) && (isspace(line[i])); i--)
    ;
   line[i + 1] = 0;
   startFile = strdup(line);
  }
  if (fgets(line, 256, f))
  {
   for (i = strlen(line) - 1; (i >= 0) && (isspace(line[i])); i--)
    ;
   line[i + 1] = 0;
   titleFile = strdup(line);
  }
  fclose(f);
 }
}

TrollDefinition::~TrollDefinition()
{
 if (dllFile)
  free(dllFile);
 if (paletteFile)
  free(paletteFile);
 if (spriteFile)
  free(spriteFile);
 if (startFile)
  free(startFile);
 if (titleFile)
  free(titleFile);
}

const char *TrollDefinition::getDllFile()
{
 return dllFile;
}

const char *TrollDefinition::getPaletteFile()
{
 return paletteFile;
}

const char *TrollDefinition::getSpriteFile()
{
 return spriteFile;
}

const char *TrollDefinition::getStartFile()
{
 return startFile;
}

const char *TrollDefinition::getTitleFile()
{
 return titleFile;
}

