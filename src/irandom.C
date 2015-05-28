/*-------------------------------------------------------------------------*\
  <irandom.C> -- IRandom implementation file

  Date      Programmer  Description
  11/07/00  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include "irandom.h"
#include <ctime>

base_generator_type IRandom::generator(42u);

void IRandom::init()
{
 generator.seed(static_cast<unsigned int>(std::time(0)));
}

int IRandom::roll(int range)
{
 boost::uniform_int<> ran_dist(0, range - 1);
 boost::variate_generator<base_generator_type&, boost::uniform_int<> > ran(generator, ran_dist);
 return ran();
}

