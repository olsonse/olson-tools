// -*- c++ -*-
// $Id$
/*
 * Copyright 2004-2007 Spencer E. Olson --- All Rights Reserved
 */

#include "m_eps.h"

/*   This function is to ensure that x gets written to system memory at
 *   sometime and is not always in a register. */
double setMachineEPSHelper (const double & x) {
    static double y = 0.0;
    y = x;
    return y;
}

double getMachineEPS () {
    // For something of this nature, it is ok to store on a common block.
    double eps;
    static double tmp;
    eps = 1.;

    do {
        eps = 0.5 * eps;
        tmp = eps + 1.;
    } while (setMachineEPSHelper(tmp) > 1.0);

    return eps*2.0;
}

