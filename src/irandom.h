#ifndef __IRANDOM_H
#define __IRANDOM_H
/*-------------------------------------------------------------------------*\
  <irandom.h> -- IRandom header file

  Date      Programmer  Description
  11/07/00  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include <random>

class IRandom
{
 public:
  static void init();
  static int roll(int range);
};

inline int IRandom(int range) { return IRandom::roll(range); }

#endif

