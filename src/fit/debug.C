
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

///
void doexit(int status){
  char c;
  // Print a prompt prior to exiting so that you can view output.
  gaout<<"Press <Enter> to exit.";
  gaout.flush();
  gain.get( c );
  exit(status);
} // doexit

