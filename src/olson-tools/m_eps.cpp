// -*- c++ -*-
// $Id$
/*
 * Copyright 2004-2007 Spencer E. Olson --- All Rights Reserved
 */

#include "m_eps.h"

namespace olson_tools {

/*   This function is to ensure that x gets written to system memory at
 *   sometime and is not always in a register. */
template < typename T >
T setMachineEPSHelper (const T & x) {
    static T y = 0.0;
    y = x;
    return y;
}

template < typename T >
T calcMachineEPS () {
    // For something of this nature, it is ok to store on a common block.
    T eps;
    static T tmp;
    eps = 1.;

    do {
        eps = 0.5 * eps;
        tmp = eps + 1.;
    } while (setMachineEPSHelper(tmp) > 1.0);

    return eps*2.0;
}

long double getMachineEPS_quad () {
  return calcMachineEPS<long double>();
};

double getMachineEPS_double () {
  return calcMachineEPS<double>();
};

float getMachineEPS_single () {
  return calcMachineEPS<float>();
};

} /*namespace olson_tools*/
