// -*- c -*-
// $Id: 3j.h,v 1.1 2005/01/08 04:27:25 olsonse Exp $
/*
 * Copyright 2004 Spencer Eugene Olson --- All Rights Reserved
 * See c-file for further license stuff.
 *
 * $Log: 3j.h,v $
 * Revision 1.1  2005/01/08 04:27:25  olsonse
 * Initial revision
 *
 */

#ifndef ANGM_COUPLING
#define ANGM_COUPLING

/** \file
 * Angular momentum coupling coefficients (only cg and 3j so far ).
 * See c-file for further license stuff.
 * */


#  ifdef __cplusplus
extern "C" {
#  endif

    /** Compute 3j symbols.
     * @return \f$ \left( \stackrel{j_{1}\ j_{2}\ j} {m_{1}\ m_{2}\ m} \right) \f$
     */
    double s3j(double j1, double j2, double j3, double m1, double m2, double m3);


    /** Computer Clebsch-Gordon coefficients.
     * @return \f$ \left< \left\{j_{1},m_{1}\right\}\ \left\{j_{2},m_{2}\right\} | j, m \right> \f$
     * */
    double cg (double j1, double m1, double j2, double m2, double j, double m);

#  ifdef __cplusplus
}
#  endif

#endif // ANGM_COUPLING
