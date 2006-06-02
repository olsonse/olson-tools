// -*- c++ -*-
// $Id$
/*
 * Copyright 2005-2006 Spencer Olson
 *
 * $Log$
 *
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
#include "Vector.h"
#include "indices.h"


template <class FieldSrc>
int spitfieldout(std::ostream & output,
                 const FieldSrc & fsrc,
                 const Vector<double,3> & xi,
                 const Vector<double,3> & xf,
                 const Vector<double,3> & dx );

/** Create the field file from the given parameters.
 * @param fsrc
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
 */
template <class FieldSrc>
void createFieldFile(const FieldSrc & fsrc,
                const Vector<double,3> & X_MINc,
                const Vector<double,3> & X_MAXc,
                const Vector<double,3> & dxc,
                const Vector<double,3> & X_MINs,
                const Vector<double,3> & X_MAXs,
                const Vector<double,3> & dxs,
                const std::string & filename) {
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
                "# \n";


    try {
        int N = 0;
        if ( (N =spitfieldout(fieldout, fsrc, X_MINc, X_MAXc, dxc)) != Nc.prod()) {
            throw std::runtime_error("wrote out " + to_string(N) + ", should have been " + to_string(Nc.prod()));
        }
        fieldout << '\n';
        if ( (N=spitfieldout(fieldout, fsrc, X_MINs, X_MAXs, dxs)) != Ns.prod()) {
            throw std::runtime_error("didn't wrote out " + to_string(N) + ", should have been " + to_string(Ns.prod()));
        }
    } catch (std::exception & e) {
        std::cout << "failed:  " << e.what() << std::endl;
    }


    fieldout.flush();
    fieldout.close();
}


template <class FieldSrc>
int spitfieldout(std::ostream & output,
                 const FieldSrc & fsrc,
                 const Vector<double,3> & xi,
                 const Vector<double,3> & xf,
                 const Vector<double,3> & dx ) {
    int Nx = 0, Ny = 0, Nz = 0, N = 0;
    for (Vector<double,3> x = xi; x[Z] <= xf[Z]; x[Z] += dx[Z]) {
        Nx = 0;
        for (x[X] = xi[X]; x[X] <= xf[X]; x[X]+= dx[X]) {
            Ny = 0;
            for (x[Y] = xi[Y]; x[Y] <= xf[Y]; x[Y] += dx[Y]) {
#if defined (DEBUG_FIELD)
                Vector<double,3> B;
                fsrc.field(B, x);

                Vector<double,3> dBdx;
                fsrc.gradient(dBdx, x);
#endif

                Vector<double,3> a;
                fsrc.accel(a, x);

                output
#if defined (DEBUG_FIELD)
                         << x << '\t'
                       /*<< B << '\t' */
#endif
                         << a << '\t'
                         << fsrc.potentialNoG(x)
#if defined (DEBUG_FIELD)
                       /*<< '\t' << dBdx */
#endif
                         << '\n';
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

#endif // CREATEFIELDFILE_H
