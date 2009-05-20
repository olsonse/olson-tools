// -*- c++ -*-
// $Id$
/*
 * Copyright 2004-2008 Spencer E. Olson --- All Rights Reserved
 */

#include <olson-tools/Timer.h>

#include <unistd.h>

namespace olson_tools {
  const double Timer::seconds_per_clock_tick = 1.0 / sysconf(_SC_CLK_TCK);
} /*namespace olson_tools*/
