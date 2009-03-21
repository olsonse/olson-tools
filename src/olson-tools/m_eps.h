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
    float  getMachineEPS_single ();
    double getMachineEPS_double ();
    long double getMachineEPS_quad ();

    /** The machine precision for long doubles
     * (at least when long doubles are 16 bytes and the computation is done on
     * my current machine).  Looking below, it looks like the precision is not
     * limited by the number of bytes but rather by something else, such as the
     * size of the register in the floating point unit.  I think the precision
     * difference between M_EPS and M_EPSl can easily be explained by the fact
     * that the registers in the FPU are 80 bit.  In other words, the registers
     * are not large enough to accommodate the full 16 bytes (or 128 bits).  */
    const double M_EPSl = 1.08420217248550443400745280086994171142578125e-19;

    /** The machine precision for doubles (at least when doubles are 8 bytes) */
    const double M_EPS  = 2.220446049250313080847263336181640625e-16;

    /** The machine precision for floats (at least when doubles are 4 bytes) */
    const float  M_EPSf = 1.1920928955078125e-07;

#ifdef __cplusplus
}
} /*namespace olson_tools*/
#endif 

#endif // M_EPS_H
