// -*- c++ -*-
// $Id$
/*
 * Copyright 2004-2007 Spencer E. Olson --- All Rights Reserved
 */

/** \file
 * Machine precision calculation.
 *
 * Copyright 2004-2007 Spencer E. Olson --- All Rights Reserved
 */


#ifndef M_EPS_H
#define M_EPS_H

#ifdef __cplusplus
namespace olson_tools {
extern "C" {
#endif 

    /** Calculate and return the machine precision.
     * This calculation is performed at run-time and returns the machine
     * precision (as mostly limited by the main-memory double representation).
     * For 64-bit doubles, this is around 2e-15.
     * */
    double getMachineEPS ();

    /** The machine precision as calculated at constructor time (beginning of
     * run-time). */
    extern const double M_EPS;

#ifdef __cplusplus
}
} /*namespace olson_tools*/
#endif 

#endif // M_EPS_H
