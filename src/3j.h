// -*- c -*-
// $Id: 3j.h,v 1.1.1.1 2005/01/08 04:27:25 olsonse Exp $
/*
 *         Copyright ? Riccardo Gusmeroli
 * Partial Copyright 2004 Spencer Eugene Olson --- All Rights Reserved
 *
 * Author: Riccardo Gusmeroli (web.address@libero.it)
 * Altered by:  Spencer Olson (olsonse@umich.edu)
 *
 * Notes: 
 *     - defining S3J_TEST enables the compilation of a very small test suite.
 *     - the maximum allowed factorial is S3J_MAX_FACT (currently 25!). 
 *     - Added caching of factorials. (Spencer Olson)
 *     - eliminated S3J_MAX_FACT limit on factorials. (Spencer Olson)
 *
 *
 * $Log: 3j.h,v $
 * Revision 1.1.1.1  2005/01/08 04:27:25  olsonse
 * Initial import
 *
 */

#ifndef ANGM_COUPLING
#define ANGM_COUPLING

/** \file
 * Angular momentum coupling coefficients (only cg and 3j so far ).
 *
 * This code was originally written by Riccardo Gusmeroli (web.address@libero.it)
 * and altered by Spencer Olson (olsonse@umich.edu).  The license on the code
 * in 3j.[hc]:
 *
 * This program is free software; you can redistribute it and/or  
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either version 2  
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * */


#  ifdef __cplusplus
extern "C" {
#  endif

    /** Compute 3j symbols.
     * @return \f$ \left( \stackrel{j_{1}\ j_{2}\ j} {m_{1}\ m_{2}\ m} \right) \f$
     */
    double s3j(double j1, double j2, double j3, double m1, double m2, double m3);


    /** Compute Clebsch-Gordon coefficients.
     * @return \f$ \left< \left\{j_{1},m_{1}\right\}\ \left\{j_{2},m_{2}\right\} | j, m \right> \f$
     * */
    double cg (double j1, double m1, double j2, double m2, double j, double m);

#  ifdef __cplusplus
}
#  endif

#endif // ANGM_COUPLING
