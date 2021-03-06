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
 *                 Copyright 2004-2008 Spencer Olson
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

/** \file
 * Routines to help write field-lookup table to file.
 */

#ifndef CREATEFIELDFILE_H
#define CREATEFIELDFILE_H



#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "ompexcept.h"
#include "Vector.h"
#include "indices.h"


namespace olson_tools {
    using namespace indices;

template <class FieldTable>
int spitfieldout(std::ostream & output,
                 const FieldTable & ftable,
                 const Vector<double,3> & xi,
                 const Vector<double,3> & xf,
                 const Vector<double,3> & dx );

/** Create the field file from the given parameters.
 * @param ftable
 *     The source of field calculation.
 * @param X_MINc
 *     The core minima.
 * @param X_MAXc
 *     The core maxima.
 * @param dxc
 *     The core stepsize.
 * @param X_MINs
 *     The shell minima.
 * @param X_MAXs
 *     The shell maxima.
 * @param dxs
 *     The shell stepsize.
 * @param filename
 *     The place to store this all.
 * @param comments
 *     A set of lines that begin with '#' each [Default ""].
 */
template <class FieldTable>
void createFieldFile(const FieldTable & ftable,
                const Vector<double,3> & X_MINc,
                const Vector<double,3> & X_MAXc,
                const Vector<double,3> & dxc,
                const Vector<double,3> & X_MINs,
                const Vector<double,3> & X_MAXs,
                const Vector<double,3> & dxs,
                const std::string & filename,
                const std::string & comments = "") {
    Vector<double,3> r0(0.0), dlc, dls;
    Vector<int,3> Nc, Ns;
    dlc   = (X_MAXc - X_MINc);
    dls   = (X_MAXs - X_MINs);
    r0    =  X_MINc + 0.5*dlc;

    Nc    = compDiv(dlc, dxc) + 1.0;
    Ns    = compDiv(dls, dxs) + 1.0;

    std::ofstream fieldout(filename.c_str());
    fieldout.precision(8);
    fieldout << std::scientific;
    fieldout << "# center \n"
                "# " << r0 << "\n"
                "# CORE : \n"
                "# " << Nc << dxc << X_MINc << X_MAXc << "\n"
                "# SHELL : \n"
                "# " << Ns << dxs << X_MINs << X_MAXs << "\n"
                "# \n"
             << comments << "# \n";


    try {
        /** do core data first */
        int N = 0;
        if ( (N =spitfieldout(fieldout, ftable, X_MINc, X_MAXc, dxc)) != Nc.prod()) {
            THROW(std::runtime_error,"wrote out " + to_string(N) + ", should have been " + to_string(Nc.prod()));
        }
        fieldout << '\n';
        /** do shell data second */
        if ( (N=spitfieldout(fieldout, ftable, X_MINs, X_MAXs, dxs)) != Ns.prod()) {
            THROW(std::runtime_error,"didn't write out " + to_string(N) + ", should have been " + to_string(Ns.prod()));
        }
    } catch (std::exception & e) {
        std::cout << "failed:  " << e.what() << std::endl;
    }

    fieldout.flush();
    fieldout.close();
}


template <class FieldTable>
int spitfieldout(std::ostream & output,
                 const FieldTable & ftable,
                 const Vector<double,3> & xi,
                 const Vector<double,3> & xf,
                 const Vector<double,3> & dx ) {
    int Nx = 0, Ny = 0, Nz = 0, N = 0;
    for (Vector<double,3> x = xi; x[Z] <= xf[Z]; x[Z] += dx[Z]) {
        Nx = 0;
        for (x[X] = xi[X]; x[X] <= xf[X]; x[X]+= dx[X]) {
            Ny = 0;
            for (x[Y] = xi[Y]; x[Y] <= xf[Y]; x[Y] += dx[Y]) {
                output << ftable.getRecord(x) << '\n';
                N++;
                Ny++;
            }

            Nx++;
        }/*for */
        Nz++;

        output << '\n';
    }
    return N;
}

}/* namespace olson_tools */

#endif // CREATEFIELDFILE_H
