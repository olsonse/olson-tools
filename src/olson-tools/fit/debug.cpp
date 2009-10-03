// -*- c++ -*-
// $Id$
/*
 * Copyright 2004 Spencer Eugene Olson --- All Rights Reserved
 */

#include <olson-tools/fit/io.h>
#include <olson-tools/fit/debug.h>

#include <stdlib.h>

namespace olson_tools{
  namespace fit {

    ///
    int default_debug_level = 0;
    ///
    int debug_level = default_debug_level;

    ///
    int pausel(){
      char c;
      gaout<<"Press <Enter> to continue.";
      gaout.flush();
      c = gain.get();
      return int( c );
    } // pausel

  }/*namespace olson_tools::fit */
}/*namespace olson_tools */
