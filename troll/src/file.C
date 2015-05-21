/*-------------------------------------------------------------------------*\
  <file.c> -- Binary file opperations
    (Least significant byte first version)

  Date      Programmer  Description
  03/20/98  Dennis      Created.
\*-------------------------------------------------------------------------*/

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  Includes
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "file.h"

FileList::FileList(const char *pattern)
{
 if (glob(pattern, 0, NULL, &results))
 {
  results.gl_pathc = 0;
 }
}

FileList::~FileList()
{
 if (results.gl_pathc)
 {
  globfree(&results);
 }
}

IUShort FileList::length()
{
 return results.gl_pathc;
}

char *FileList::operator[](int num)
{
 return results.gl_pathv[num];
}

BinaryReadFile::BinaryReadFile()
{
 file = (FILE *)0;
}

BinaryReadFile::BinaryReadFile(const char *filename)
{
 file = fopen(filename, "rb");
}

BinaryReadFile::~BinaryReadFile()
{
 if (file) fclose(file);
}

void BinaryReadFile::close()
{
 if (file) fclose(file);
}

void BinaryReadFile::open(const char *filename)
{
 if (file) close();
 file = fopen(filename, "rb");
}

void BinaryReadFile::readByte(IByte &a)
{
 fread(&a, 1, 1, file);
}

void BinaryReadFile::readByteArray(const int size, IByte *a)
{
 fread(a, 1, size, file);
}

void BinaryReadFile::readShort(IShort &a)
{
 fread(&a, 2, 1, file);
}

void BinaryReadFile::readShortArray(const int size, IShort *a)
{
 fread(a, 2, size, file);
}

void BinaryReadFile::readLong(ILong &a)
{
 fread(&a, 4, 1, file);
}

void BinaryReadFile::readLongArray(const int size, ILong *a)
{
 fread(a, 4, size, file);
}

void BinaryReadFile::readUByte(IUByte &a)
{
 fread(&a, 1, 1, file);
}

void BinaryReadFile::readUByteArray(const int size, IUByte *a)
{
 fread(a, 1, size, file);
}

void BinaryReadFile::readUShort(IUShort &a)
{
 fread(&a, 2, 1, file);
}

void BinaryReadFile::readUShortArray(const int size, IUShort *a)
{
 fread(a, 2, size, file);
}

void BinaryReadFile::readULong(IULong &a)
{
 fread(&a, 4, 1, file);
}

void BinaryReadFile::readULongArray(const int size, IULong *a)
{
 fread(a, 4, size, file);
}

void BinaryReadFile::seek(IULong where)
{
 fseek(file, where, SEEK_SET);
}

BinaryWriteFile::BinaryWriteFile()
{
 file = (FILE *)0;
}

BinaryWriteFile::BinaryWriteFile(const char *filename)
{
 file = fopen(filename, "wb");
}

BinaryWriteFile::~BinaryWriteFile()
{
 if (file) fclose(file);
}

void BinaryWriteFile::close()
{
 if (file) fclose(file);
}

void BinaryWriteFile::open(const char *filename)
{
 if (file) close();
 file = fopen(filename, "wb");
}

void BinaryWriteFile::writeByte(const IByte &a)
{
 fwrite(&a, 1, 1, file);
}

void BinaryWriteFile::writeByteArray(const int size, const IByte *a)
{
 fwrite(a, 1, size, file);
}

void BinaryWriteFile::writeShort(const IShort &a)
{
 fwrite(&a, 2, 1, file);
}

void BinaryWriteFile::writeShortArray(const int size, const IShort *a)
{
 fwrite(a, 2, size, file);
}

void BinaryWriteFile::writeLong(const ILong &a)
{
 fwrite(&a, 4, 1, file);
}

void BinaryWriteFile::writeLongArray(const int size, const ILong *a)
{
 fwrite(a, 4, size, file);
}

void BinaryWriteFile::writeUByte(const IUByte &a)
{
 fwrite(&a, 1, 1, file);
}

void BinaryWriteFile::writeUByteArray(const int size, const IUByte *a)
{
 fwrite(a, 1, size, file);
}

void BinaryWriteFile::writeUShort(const IUShort &a)
{
 fwrite(&a, 2, 1, file);
}

void BinaryWriteFile::writeUShortArray(const int size, const IUShort *a)
{
 fwrite(a, 2, size, file);
}

void BinaryWriteFile::writeULong(const IULong &a)
{
 fwrite(&a, 4, 1, file);
}

void BinaryWriteFile::writeULongArray(const int size, const IULong *a)
{
 fwrite(a, 4, size, file);
}

void BinaryWriteFile::seek(const IULong where)
{
 fseek(file, where, SEEK_SET);
}

