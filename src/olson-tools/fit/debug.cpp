// -*- c++ -*-
// $Id$
/*
 * Copyright 2004 Spencer Eugene Olson --- All Rights Reserved
 */

#include "io.h"
#include "debug.h"
#include <stdlib.h>

namespace olson_tools{ namespace fit {

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

}}/*namespace olson_tools::fit */
