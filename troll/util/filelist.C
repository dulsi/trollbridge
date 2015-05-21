#include "../src/file.h"
#include "../src/file.C"

int main()
{
 FileList files("*.trs");
 for (int i = 0; i < files.length(); i++)
 {
  printf("%d : %s\n", i, files[i]);
 }
 return 0;
}

