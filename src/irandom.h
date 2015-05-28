#ifndef __IRANDOM_H
#define __IRANDOM_H
/*-------------------------------------------------------------------------*\
  <irandom.h> -- IRandom header file

  Date      Programmer  Description
  11/07/00  Dennis      Created.
\*-------------------------------------------------------------------------*/

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

typedef boost::minstd_rand base_generator_type;

class IRandom
{
 public:
  static void init();
  static int roll(int range);

 private:
  static base_generator_type generator;
};

inline int IRandom(int range) { return IRandom::roll(range); }

#endif

