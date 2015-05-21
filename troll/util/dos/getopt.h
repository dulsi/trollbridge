#ifndef __GETOPT_H
#define __GETOPT_H
/*-------------------------------------------------------------------------*\
  <getopt.h> -- Dummy getopt_long for Dos

  Date      Programmer  Description
  04/05/00  Dennis      Created.
\*-------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
  New Types
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
struct option {
 const char *name;
 int has_arg;
 int *flag;
 int val;
};

/* Current ignore long arguments under Dos*/
#define getopt_long(argc,argv,optstring,longopts,longindex)                 \
  getopt(argc,argv,optstring)

#ifdef __cplusplus
}
#endif

#endif

