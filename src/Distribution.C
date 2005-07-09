// -*- c++ -*-
// $Id: Distribution.C,v 1.1 2005/01/14 14:09:30 olsonse Exp $
/*
 * Copyright 2004 Spencer Olson
 *
 * $Log: Distribution.C,v $
 * Revision 1.1  2005/01/14 14:09:30  olsonse
 * Fixed documentation on memory.h, msh.h, options.h.
 * Moved new files Distribution.[hC] listutil.h readData.h from dsmc code.
 *
 * Revision 1.1.1.1  2005/01/08 04:57:25  olsonse
 * Initial CVS import (finally).
 *
 *
 */

/** \file
 * Generic distribution inverter.
 * Copyright 2004 Spencer Olson.
 *
 */

#include "Distribution.h"

Distribution::~Distribution() {
    delete[] q;
}

