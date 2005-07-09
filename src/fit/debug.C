
#include "io.h"
#include "debug.h"
#include <stdlib.h>

///
int default_debug_level = 0;
///
int debug_level = default_debug_level;

///
int pausel(){
  char c;
  gaout<<"Press <Enter> to continue.";
  gaout.flush();
  gain.get( c );
  return int( c );
} // pausel

