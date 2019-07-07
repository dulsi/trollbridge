/*-------------------------------------------------------------------------*\
  <irandom.C> -- IRandom implementation file

  Date      Programmer  Description
  11/07/00  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "irandom.h"
#include <ctime>

static std::random_device *r;
static std::default_random_engine *generator;

void IRandom::init()
{
 r = new std::random_device;
 generator = new std::default_random_engine((*r)());
}

int IRandom::roll(int range)
{
 std::uniform_int_distribution<int> ran_dist(0, range - 1);
 return ran_dist(*generator);
}

