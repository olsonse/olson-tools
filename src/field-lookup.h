// -*- c++ -*-
// $Id$
/*
 * Copyright 2005-2006 Spencer Olson
 *
 * $Log$
 *
 */

/** \file
 * Field-lookup utility.
 * @see createFieldFile.h for routines to help creating the field-lookup table
 * file.
 */



#ifndef __FIELD_LOOKUP_H
#define __FIELD_LOOKUP_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "bfield.h"

/**
 * Field-lookup class.
 * This class loads a table (from flat file created by createFieldFile.h
 * routines) to provide acceleration and potential information.  
 * The returned values are interpolated using the triangle interpolant
 * described in Jackson's "Electricity and Magnetism" book.  
 *
 * @see createFieldFile.h for routines to help creating the field-lookup table
 * file.
 */
class BFieldLookup : public virtual BField::BaseSrc {
  public:
    typedef BField::BaseSrc super0;

  private:
    std::string fname;

  public:
    BFieldLookup() : super0(), fname("") {}

    BFieldLookup(const std::string & filename) : super0() {
        readindata(filename);
    }

    /** this function will allow the user to change the field-file then
     * request a re-read mid-stream.  This is meant to be useful as a trigger
     * point inside a debugger if necessary. */
    void rereadindata() {
        readindata();
    }

    /** Provide acceleration data from a file source.
     * The following employs a 3D lever rule, or triangle rule.
     * @see Jackson's E&M book.
     */
    inline void accel(Vector<double,3> & a, const Vector<double,3> & r) const {
        register unsigned int table;
        register unsigned int xi, yi, zi;
        register double xf, yf, zf, xF, yF, zF;
        getindx(table, xi, xf, yi, yf, zi, zf, r);
        xF = 1.0 - xf;
        yF = 1.0 - yf;
        zF = 1.0 - zf;

        a.zero();
        a.addFraction(xF*yF*zF, data[table](xi  ,yi  ,zi  ).a);
        a.addFraction(xf*yF*zF, data[table](xi+1,yi  ,zi  ).a);
        a.addFraction(xF*yf*zF, data[table](xi  ,yi+1,zi  ).a);
        a.addFraction(xf*yf*zF, data[table](xi+1,yi+1,zi  ).a);
        a.addFraction(xF*yF*zf, data[table](xi  ,yi  ,zi+1).a);
        a.addFraction(xf*yF*zf, data[table](xi+1,yi  ,zi+1).a);
        a.addFraction(xF*yf*zf, data[table](xi  ,yi+1,zi+1).a);
        a.addFraction(xf*yf*zf, data[table](xi+1,yi+1,zi+1).a);
    }

    inline double potential(const Vector<double,3> & r) const {
        return potentialNoG(r) - super0::mass * (super0::gravity * r);
    }

    /** Provide potential data (without gravity) from a file source.
     * The following employs a 3D lever rule, or triangle rule.
     * @see Jackson's E&M book.
     */
    inline double potentialNoG(const Vector<double,3> & r) const {
        register unsigned int table;
        register unsigned int xi, yi, zi;
        register double xf, yf, zf, xF, yF, zF;
        getindx(table, xi, xf, yi, yf, zi, zf, r);
        xF = 1.0 - xf;
        yF = 1.0 - yf;
        zF = 1.0 - zf;

        return xF*yF*zF * data[table](xi  ,yi  ,zi  ).V
             + xf*yF*zF * data[table](xi+1,yi  ,zi  ).V
             + xF*yf*zF * data[table](xi  ,yi+1,zi  ).V
             + xf*yf*zF * data[table](xi+1,yi+1,zi  ).V
             + xF*yF*zf * data[table](xi  ,yi  ,zi+1).V
             + xf*yF*zf * data[table](xi+1,yi  ,zi+1).V
             + xF*yf*zf * data[table](xi  ,yi+1,zi+1).V
             + xf*yf*zf * data[table](xi+1,yi+1,zi+1).V;
    }

  private:
    class Record {
      public:
        Vector<double,3> a;
        double V;
    };

    class DTable {
      public:
        Record * data;
        inline DTable () : data(NULL), xlen(0), ylen(0),
                           zlen(0), xlen_times_ylen(0) {}

        inline void initialize (const unsigned int & Nx,
                                const unsigned int & Ny,
                                const unsigned int & Nz) {
            xlen = Nx;
            ylen = Ny;
            zlen = Nz;
            xlen_times_ylen = Nx*Ny;

            data = new Record[xlen*ylen*zlen];
        }

        inline void cleanup () {
            if (data) {
                delete[] data;
                data = NULL;
            }

            xlen = ylen = zlen = xlen_times_ylen = 0;
        }

        inline ~DTable () {
            cleanup();
        }

        inline std::istream & readindata(std::istream & in) {
            unsigned int elt = 0;
            char line[512] = {0};
            for (unsigned int i = 0; i < zlen; i++) {
                for (unsigned int j = 0; j < xlen; j++) {
                    for (unsigned int k = 0; k < ylen; k++, elt++) {
                        /* format:
                            x y z Bx By Bz ax ay az V dBdx[xyz] 
                        */
                        //Vector<double,3> pos, B, dBdx;
                        line[0] = 0x0;
                        while (in.good() && strlen(line) == 0) {
                            in.getline(line,sizeof(line));
                        }
                        std::istringstream ins(line);
                        ins /* >> pos >> B */ >> data[elt].a >> data[elt].V /* >> dBdx */;
                    }/* for */
                }/* for */
            }/* for */

            return in;
        }

        inline const Record & operator()(const unsigned int & xi,
                                       const unsigned int & yi,
                                       const unsigned int & zi) const {
            return data[zi*(xlen_times_ylen) + xi*ylen + yi];
        }

        unsigned int xlen, ylen, zlen, xlen_times_ylen;
    };

    enum DSECT {
        CORE,
        SHELL
    };

    inline void getindx ( unsigned int & table,
                          unsigned int & xi,
                          double       & xf,
                          unsigned int & yi,
                          double       & yf,
                          unsigned int & zi,
                          double       & zf,
                          const Vector<double,3> & r) const {
#if !defined(NOTRUNCX) || !defined(NOTRUNCY) || !defined(NOTRUNCZ)
        const Vector<int,3> * nmax;
#endif
        if (fabs(r[X] - r0[X]) > core_L_2[X] ||
            fabs(r[Y] - r0[Y]) > core_L_2[Y] ||
            fabs(r[Z] - r0[Z]) > core_L_2[Z]   ) {
            table = SHELL;
            xf = ( (r[X]-shell_min[X]) * shell_dx_inv[X] );
            yf = ( (r[Y]-shell_min[Y]) * shell_dx_inv[Y] );
            zf = ( (r[Z]-shell_min[Z]) * shell_dx_inv[Z] );
#if !defined(NOTRUNCX) || !defined(NOTRUNCY) || !defined(NOTRUNCZ)
            nmax = &shell_N;
#endif
        } else {
            table = CORE;
            xf = ( (r[X]-core_min[X]) * core_dx_inv[X] );
            yf = ( (r[Y]-core_min[Y]) * core_dx_inv[Y] );
            zf = ( (r[Z]-core_min[Z]) * core_dx_inv[Z] );
#if !defined(NOTRUNCX) || !defined(NOTRUNCY) || !defined(NOTRUNCZ)
            nmax = &core_N;
#endif
        }
#if !defined(NOTRUNCX)
        xf = std::max(0.0,std::min((double)((*nmax)[X])-1.001,xf));
#endif
#if !defined(NOTRUNCY)
        yf = std::max(0.0,std::min((double)((*nmax)[Y])-1.001,yf));
#endif
#if !defined(NOTRUNCZ)
        zf = std::max(0.0,std::min((double)((*nmax)[Z])-1.001,zf));
#endif
        xi = (int) xf; xf -= xi;
        yi = (int) yf; yf -= yi;
        zi = (int) zf; zf -= zi;
    }


    /* two x,y,z tables:  core data and outlying data. */
    DTable data[2];

    Vector<double,3> r0;

    Vector<double,3> core_L_2;
    Vector<double,3> core_dx;
    Vector<double,3> core_dx_inv;
    Vector<int,3> core_N;
    Vector<double,3> core_min;
    Vector<double,3> core_max;

    Vector<double,3> shell_dx;
    Vector<double,3> shell_dx_inv;
    Vector<int,3> shell_N;
    Vector<double,3> shell_min;
    Vector<double,3> shell_max;


  public:

    /** only supposed to be called once, upon class initialization. */
    void readindata(const std::string & filename = "") {
        if (filename.length() != 0) {
            fname = filename;
        }

        if (fname.length() == 0) {
            throw std::runtime_error("field-lookup:readindata:  missing filename.");
        }

        /* format will be:
            # center : \n
            #   x0 y0 z0 \n
            # CORE : \n
            #   Nx \w Ny \w Nz \w dx \w dy \w dz \n
            # SHELL : \n
            #   Nx \w Ny \w Nz \w dx \w dy \w dz \n
            # \n
            CORE-DATA
            \n
            SHELL-DATA
        */
        std::ifstream infile(fname.c_str());
        char pound;
        char line[100];
        infile >> pound; infile.getline(line,sizeof(line)); /* center  : */
        infile >> pound >> r0;                              /* r0 */
        infile >> pound; infile.getline(line,sizeof(line)); /* CORE : */
        infile >> pound >> core_N >> core_dx >> core_min >> core_max; /* core_N core_dx core_min core_max*/
        infile >> pound; infile.getline(line,sizeof(line)); /* SHELL : */
        infile >> pound >> shell_N >> shell_dx >> shell_min >> shell_max;/* shell_N shell_dx min max */
        infile >> pound; infile.getline(line,sizeof(line)); /* blank comment line */

        core_dx_inv  = 1.0; core_dx_inv .compDiv(core_dx);
        shell_dx_inv = 1.0; shell_dx_inv.compDiv(shell_dx);

        core_L_2 = 0.5*compMult((core_N-1).to_type<double>(), core_dx);
        //core_min  = r0 - core_L_2;

        //shell_min = r0 - 0.5*compMult((shell_N-1).to_type<double>(), shell_dx);

        data[CORE].initialize(core_N[X], core_N[Y], core_N[Z]);
        data[SHELL].initialize(shell_N[X], shell_N[Y], shell_N[Z]);

        /* now read in the core data-block. */
        data[CORE].readindata(infile);
        data[SHELL].readindata(infile);
    }
};

#endif // __FIELD_LOOKUP_H
