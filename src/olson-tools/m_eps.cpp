// -*- c++ -*-
// $Id$
/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2004-2007 Spencer E. Olson --- All Rights Reserved
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *  
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *                                                                                 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.                                                                           .
 * 
 * Questions? Contact Spencer Olson (olsonse@umich.edu) 
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
