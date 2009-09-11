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
 * Field-lookup utility.
 * @see createFieldFile.h for routines to help creating the field-lookup table
 * file.
 */



#ifndef __FIELD_LOOKUP_H
#define __FIELD_LOOKUP_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "Vector.h"
#include "indices.h"
#include "ompexcept.h"
#include "power.h"


namespace olson_tools {
    using namespace indices;

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
template <class Record>
class FieldLookupBase {
  private:
    std::string fname;
    bool initialized;

  public:
    /** Default constructor.
     * Does not initialize the lookup table.
     */
    FieldLookupBase() : fname(""), initialized(false) {}

    FieldLookupBase(const std::string & filename) : fname(""), initialized(false) {
        readindata(filename);
    }

    /** Create a field lookup table where each element in the field is default
     * initialized (depends on the record constructor). */
    void initialize(const Vector<double,3> & _r0,
                    const Vector<double,3> & _core_dx,
                    const Vector<double,3> & _core_min,
                    const Vector<double,3> & _core_max,
                    const Vector<double,3> & _shell_dx,
                    const Vector<double,3> & _shell_min,
                    const Vector<double,3> & _shell_max) {
        r0 = _r0;

        core_dx = _core_dx;
        core_min = _core_min;
        core_max = _core_max;

        {
            Vector<double,3> dlc;
            dlc   = (core_max - core_min);
            core_N    = compDiv(dlc, core_dx) + 1.0;
        }

        core_dx_inv  = 1.0; core_dx_inv .compDiv(core_dx);
        core_L_2 = 0.5*compMult((core_N-1).to_type<double>(), core_dx);
        data[CORE].initialize(core_N[X], core_N[Y], core_N[Z]);


#ifndef DISABLE_SHELL_LOOKUP

        shell_dx = _shell_dx;
        shell_min = _shell_min;
        shell_max = _shell_max;

        {
            Vector<double,3> dls;
            dls   = (shell_max - shell_min);
            shell_N   = compDiv(dls, shell_dx) + 1.0;
        }

        shell_dx_inv = 1.0; shell_dx_inv.compDiv(shell_dx);
        data[SHELL].initialize(shell_N[X], shell_N[Y], shell_N[Z]);
#endif
    }

    const bool & isInitialized() const { return initialized; }

    /** this function will allow the user to change the field-file then
     * request a re-read mid-stream.  This is meant to be useful as a trigger
     * point inside a debugger if necessary. */
    void rereadindata() {
        readindata();
    }

  protected:

    class DTable {
      private:
        Record * data;

      public:
        inline DTable () : data(NULL), xlen(0), ylen(0),
                           zlen(0), xlen_times_ylen(0) {}

        inline void initialize (const unsigned int & Nx,
                                const unsigned int & Ny,
                                const unsigned int & Nz) {
            cleanup();

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
                        line[0] = 0x0;
                        while (in.good() && strlen(line) == 0) {
                            in.getline(line,sizeof(line));
                        }
                        std::istringstream ins(line);
                        ins >> data[elt];
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

    /** The table types. */
    enum DSECT {
        /** The core table.  This table should be more narrow in volume than
         * the SHELL table.  It is useful to make the cell separation for this
         * table be much smaller than that of the SHELL table.
         */
        CORE = 0,
        SHELL = 1
    };

    /** only supposed to be called once, upon class initialization. */
    void readindata(const std::string & filename = "") {
        if (filename.length() != 0) {
            fname = filename;
        }

        if (fname.length() == 0) {
            THROW(std::runtime_error,"field-lookup:readindata:  missing filename.");
        }

        /* format will be (note that {Ni \w} means Nx \w Ny \w Nz \w.):
            # center : \n
            #   x0 y0 z0 \n
            # CORE : \n
            #   {Ni \w} {di \w} {mini \w} {maxi \w} \n
            # SHELL : \n
            #   {Ni \w} {di \w} {mini \w} {maxi \w} \n
            # \n
            <any number of consecutive comment lines
             (each line starts with '#' and ends with '\n')>
            # \n
            CORE-DATA
            \n
            SHELL-DATA
        */
        std::ifstream infile(fname.c_str());

        if (!infile.good()) {
            THROW(std::runtime_error,"field-lookup::readindata:  invalid filename.");
        }


        {
            char pound;
            char line[1024];

            Vector<double,3> _r0;

            Vector<double,3> _core_dx;
            Vector<int,3>    _core_N;
            Vector<double,3> _core_min;
            Vector<double,3> _core_max;

            Vector<double,3> _shell_dx;
            Vector<int,3>    _shell_N;
            Vector<double,3> _shell_min;
            Vector<double,3> _shell_max;
            
            infile >> pound; infile.getline(line,sizeof(line)); /* # center  : */
            infile >> pound >> _r0;
            infile >> pound; infile.getline(line,sizeof(line)); /* # CORE : */
            infile >> pound >> _core_N >> _core_dx >> _core_min >> _core_max;

#ifndef DISABLE_SHELL_LOOKUP
            infile >> pound; infile.getline(line,sizeof(line)); /* # SHELL : */
            infile >> pound >> _shell_N >> _shell_dx >> _shell_min >> _shell_max;
#endif

            initialize(_r0,_core_dx, _core_min, _core_max, _shell_dx, _shell_min, _shell_max);

            if (//r0      != _r0          ||
                core_N  != _core_N
#ifndef DISABLE_SHELL_LOOKUP
                || shell_N != _shell_N
#endif
                ) {
                THROW(std::runtime_error,"field-lookup::readindata:  field filename header incorrect");
            }
        }


        /* now read in all comment lines and skip them */
        while (infile.good()) {
            char testchar = infile.peek();
            if (isspace(testchar)) {
                (void)infile.get();
            } else if (testchar == '#') {
                /* read past comments. */
                std::stringbuf cmtbuf;
                infile.get(cmtbuf, '\n');
            } else {
                break;
            }
        }

        /* now read in the core data-block. */
        data[CORE].readindata(infile);
#ifndef DISABLE_SHELL_LOOKUP
        data[SHELL].readindata(infile);
#endif

        initialized = true;
    }
};

/** The cartesian field lookup class. */
template <class Record>
class FieldLookup : public FieldLookupBase<Record> {
  public:
    typedef FieldLookupBase<Record> super;

    /** Default constructor.
     * Does not initialize the lookup table.
     */
    FieldLookup() : super() {}

    FieldLookup(const std::string & filename) : super(filename) { }


    /** Provide acceleration data from a file source.
     * The following employs a 3D lever rule, or triangle rule.
     * @see Jackson's E&M book.
     */
    inline void vector_lookup(Vector<double,3> & retval,
                              const Vector<double,3> & r,
                              const unsigned int & i) const {
        register unsigned int table;
        register unsigned int xi, yi, zi;
        register double xf, yf, zf, xF, yF, zF;
        getindx(table, xi, xf, yi, yf, zi, zf, r);
        xF = 1.0 - xf;
        yF = 1.0 - yf;
        zF = 1.0 - zf;

        retval.zero();
        retval.addFraction(xF*yF*zF, super::data[table](xi  ,yi  ,zi  ).vector(i));
        retval.addFraction(xf*yF*zF, super::data[table](xi+1,yi  ,zi  ).vector(i));
        retval.addFraction(xF*yf*zF, super::data[table](xi  ,yi+1,zi  ).vector(i));
        retval.addFraction(xf*yf*zF, super::data[table](xi+1,yi+1,zi  ).vector(i));
        retval.addFraction(xF*yF*zf, super::data[table](xi  ,yi  ,zi+1).vector(i));
        retval.addFraction(xf*yF*zf, super::data[table](xi+1,yi  ,zi+1).vector(i));
        retval.addFraction(xF*yf*zf, super::data[table](xi  ,yi+1,zi+1).vector(i));
        retval.addFraction(xf*yf*zf, super::data[table](xi+1,yi+1,zi+1).vector(i));
    }

    /** Provide potential data from a file source.
     * The following employs a 3D lever rule, or triangle rule.
     * @see Jackson's E&M book.
     */
    inline double scalar_lookup(const Vector<double,3> & r, const unsigned int & i) const {
        register unsigned int table;
        register unsigned int xi, yi, zi;
        register double xf, yf, zf, xF, yF, zF;
        getindx(table, xi, xf, yi, yf, zi, zf, r);
        xF = 1.0 - xf;
        yF = 1.0 - yf;
        zF = 1.0 - zf;

        return xF*yF*zF * super::data[table](xi  ,yi  ,zi  ).scalar(i)
             + xf*yF*zF * super::data[table](xi+1,yi  ,zi  ).scalar(i)
             + xF*yf*zF * super::data[table](xi  ,yi+1,zi  ).scalar(i)
             + xf*yf*zF * super::data[table](xi+1,yi+1,zi  ).scalar(i)
             + xF*yF*zf * super::data[table](xi  ,yi  ,zi+1).scalar(i)
             + xf*yF*zf * super::data[table](xi+1,yi  ,zi+1).scalar(i)
             + xF*yf*zf * super::data[table](xi  ,yi+1,zi+1).scalar(i)
             + xf*yf*zf * super::data[table](xi+1,yi+1,zi+1).scalar(i);
    }

    /** Obtain the nearest record of the lookup table.  */
    Record & getRecord( const Vector<double,3> & r,
                        const enum super::DSECT & table = super::CORE ) {
        register double xf, yf, zf;

#if !defined(NOTRUNCX) || !defined(NOTRUNCY) || !defined(NOTRUNCZ)
        const Vector<int,3> * nmax;
#endif

#ifndef DISABLE_SHELL_LOOKUP
        if ( table == super::SHELL ) {
            xf = ( (r[X]-super::shell_min[X]) * super::shell_dx_inv[X] );
            yf = ( (r[Y]-super::shell_min[Y]) * super::shell_dx_inv[Y] );
            zf = ( (r[Z]-super::shell_min[Z]) * super::shell_dx_inv[Z] );
#  if !defined(NOTRUNCX) || !defined(NOTRUNCY) || !defined(NOTRUNCZ)
            nmax = &(super::shell_N);
#  endif
        } else
#endif
        { /* table == super::CORE */
            xf = ( (r[X]-super::core_min[X]) * super::core_dx_inv[X] );
            yf = ( (r[Y]-super::core_min[Y]) * super::core_dx_inv[Y] );
            zf = ( (r[Z]-super::core_min[Z]) * super::core_dx_inv[Z] );
#if !defined(NOTRUNCX) || !defined(NOTRUNCY) || !defined(NOTRUNCZ)
            nmax = &(super::core_N);
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

        return super::data[table](int(round(xf)), int(round(yf)), int(round(zf)));
    }

  private:
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

#ifndef DISABLE_SHELL_LOOKUP
        if (fabs(r[X] - super::r0[X]) > super::core_L_2[X] ||
            fabs(r[Y] - super::r0[Y]) > super::core_L_2[Y] ||
            fabs(r[Z] - super::r0[Z]) > super::core_L_2[Z]   ) {
            table = super::SHELL;
            xf = ( (r[X]-super::shell_min[X]) * super::shell_dx_inv[X] );
            yf = ( (r[Y]-super::shell_min[Y]) * super::shell_dx_inv[Y] );
            zf = ( (r[Z]-super::shell_min[Z]) * super::shell_dx_inv[Z] );
#  if !defined(NOTRUNCX) || !defined(NOTRUNCY) || !defined(NOTRUNCZ)
            nmax = &( (const Vector<int,3> &) (super::shell_N) );
#  endif
        } else
#endif        
        {
            table = super::CORE;
            xf = ( (r[X]-super::core_min[X]) * super::core_dx_inv[X] );
            yf = ( (r[Y]-super::core_min[Y]) * super::core_dx_inv[Y] );
            zf = ( (r[Z]-super::core_min[Z]) * super::core_dx_inv[Z] );
#if !defined(NOTRUNCX) || !defined(NOTRUNCY) || !defined(NOTRUNCZ)
            nmax = &( (const Vector<int,3> &) (super::core_N) );
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

};


/** The axially symmetric field lookup class. */
template <class Record>
class AxiSymFieldLookup : public FieldLookupBase<Record> {
  public:
    typedef FieldLookupBase<Record> super;

    /** Default constructor.
     * Does not initialize the lookup table.
     */
    AxiSymFieldLookup() : super() {
        defaultR();
    }

    AxiSymFieldLookup(const std::string & filename) : super(filename) {
        defaultR();
    }


    /** Provide acceleration data from a file source.
     * The following employs a 3D lever rule, or triangle rule.
     * @see Jackson's E&M book.
     */
    inline void vector_lookup(Vector<double,3> & retval,
                              const Vector<double,3> & r,
                              const unsigned int & i) const {
        register unsigned int table;
        register unsigned int rhoi, zi;
        register double rhof, zf, rhoF, zF;
        getindx(table, rhoi, rhof, zi, zf, r);
        rhoF = 1.0 - rhof;
        zF = 1.0 - zf;

        retval.zero();
        retval.addFraction(rhoF*zF, super::data[table](rhoi  , 0, zi  ).vector(i));
        retval.addFraction(rhof*zF, super::data[table](rhoi+1, 0, zi  ).vector(i));
        retval.addFraction(rhoF*zf, super::data[table](rhoi  , 0, zi+1).vector(i));
        retval.addFraction(rhof*zf, super::data[table](rhoi+1, 0, zi+1).vector(i));
    }

    /** Provide potential data from a file source.
     * The following employs a 3D lever rule, or triangle rule.
     * @see Jackson's E&M book.
     */
    inline double scalar_lookup(const Vector<double,3> & r, const unsigned int & i) const {
        register unsigned int table;
        register unsigned int rhoi, zi;
        register double rhof, zf, rhoF, zF;
        getindx(table, rhoi, rhof, zi, zf, r);
        rhoF = 1.0 - rhof;
        zF = 1.0 - zf;

        return rhoF*zF * super::data[table](rhoi  , 0, zi  ).scalar(i)
             + rhof*zF * super::data[table](rhoi+1, 0, zi  ).scalar(i)
             + rhoF*zf * super::data[table](rhoi  , 0, zi+1).scalar(i)
             + rhof*zf * super::data[table](rhoi+1, 0, zi+1).scalar(i);
    }

    /** Rotation matrix INTO the field-lookup frame. */
    SquareMatrix<double,3> R;

private:
    inline void defaultR() {
        R = SquareMatrix<double,3U>::identity();
    }

public:
    void rotateField(const Vector<double,3> & k) {
        /* I know that the method I am doing for this is not the fastest, but who
         * cares.  It is the most transparent and I only have to do this once
         * anyway!
         * */
        /* cos(phi) = x/sqrt(x**2 + y**2)
         * sin(phi) = y/sqrt(x**2 + y**2)
         * cos(theta (polar)) = z/r
         * sin(theta) = sqrt(1-cos(theta)**2)
         * */
        double costheta = k[Z] /* /r */;
        double sintheta = sqrt(1.0 - SQR(costheta));
        double cosphi = 1.0, sinphi = 0.0;
        if (sintheta) {
            cosphi = k[X] / hypot( k[X], k[Y]);
            sinphi = k[Y] / hypot( k[X], k[Y]);
        }

        /* rotation matrix is given by:
         * Rz[+phi].Ry[+theta]
         * where Rz[x] and Ry[x] are passive rotations.
         * */
        const double ry_[3][3]  =
                             {{ costheta,    0,  -sintheta},
                              {   0,         1,       0   },
                              { sintheta,    0,   costheta}};

        const double rz_[3][3]  =
                             {{ cosphi,    sinphi,    0   },
                              {-sinphi,    cosphi,    0   },
                              {   0,         0,       1   }};

        SquareMatrix<double,3> & ry   = *((SquareMatrix<double,3>*)ry_);
        SquareMatrix<double,3> & rz   = *((SquareMatrix<double,3>*)rz_);

        R   = ry * rz;
    }


    /** Rotate r into the coordinate system of the lookup table and return
     * (rho,z). */
    inline void getRotatedRelativeCoords( const Vector<double,3> & r,
                                          double & rho,
                                          double & z ) const {

        Vector<double,3> r_rel = R * (r - super::r0);
        rho = sqrt(SQR(r_rel[X]) +  SQR(r_rel[Y]));
         z = r_rel[Z];
    }


    /** Obtain the nearest record of the lookup table.  */
    Record & getRecord( const Vector<double,3> & r,
                        const enum super::DSECT & table = super::CORE ) {
        register double rhof, zf;

#if !defined(NOTRUNCX) || !defined(NOTRUNCY) || !defined(NOTRUNCZ)
        const Vector<int,3> * nmax;
#endif
        double rho, z;
        getRotatedRelativeCoords(r, rho, z);

#ifndef DISABLE_SHELL_LOOKUP
        if ( table == super::SHELL ) {
            rhof = ( (rho -super::shell_min[RHO]) * super::shell_dx_inv[RHO] );
            zf   = ( (z   -super::shell_min[Z]  ) * super::shell_dx_inv[Z] );
#  if !defined(NOTRUNCX) || !defined(NOTRUNCY) || !defined(NOTRUNCZ)
            nmax = &(super::shell_N);
#  endif
        } else
#endif        
        { /* table == super::CORE */
            rhof = ( (rho -super::core_min[RHO]) * super::core_dx_inv[RHO] );
            zf = ( (z   -super::core_min[Z]  ) * super::core_dx_inv[Z] );
#if !defined(NOTRUNCX) || !defined(NOTRUNCY) || !defined(NOTRUNCZ)
            nmax = &(super::core_N);
#endif
        }
#if !defined(NOTRUNCX)
        rhof = std::max(0.0,std::min((double)((*nmax)[RHO])-1.001,rhof));
#endif
#if !defined(NOTRUNCZ)
        zf = std::max(0.0,std::min((double)((*nmax)[Z])-1.001,zf));
#endif

        return super::data[table](int(round(rhof)), 0, int(round(zf)));
    }

  private:
    inline void getindx ( unsigned int & table,
                          unsigned int & rhoi,
                          double       & rhof,
                          unsigned int & zi,
                          double       & zf,
                          const Vector<double,3> & r) const {
#if !defined(NOTRUNCX) || !defined(NOTRUNCY) || !defined(NOTRUNCZ)
        const Vector<int,3> * nmax;
#endif
        double rho, z;
        getRotatedRelativeCoords(r, rho, z);

#ifndef DISABLE_SHELL_LOOKUP
        if (rho > super::core_max[RHO] || rho < super::core_min[RHO] ||
            fabs(z) > super::core_L_2[Z]   ) {
            table = super::SHELL;
            rhof = ( (rho -super::shell_min[RHO]) * super::shell_dx_inv[RHO] );
            zf   = ( (z   -super::shell_min[Z]  ) * super::shell_dx_inv[Z]   );
#  if !defined(NOTRUNCX) || !defined(NOTRUNCY) || !defined(NOTRUNCZ)
            nmax = &(super::shell_N);
#  endif
        } else
#endif
        {
            table = super::CORE;
            rhof = ( (rho -super::core_min[RHO]) * super::core_dx_inv[RHO] );
            zf   = ( (z   -super::core_min[Z]  ) * super::core_dx_inv[Z] );
#if !defined(NOTRUNCX) || !defined(NOTRUNCY) || !defined(NOTRUNCZ)
            nmax = &(super::core_N);
#endif
        }
#if !defined(NOTRUNCX)
        rhof = std::max(0.0,std::min((double)((*nmax)[RHO])-1.001,rhof));
#endif
#if !defined(NOTRUNCZ)
        zf = std::max(0.0,std::min((double)((*nmax)[Z])-1.001,zf));
#endif
        rhoi = (int) rhof; rhof -= rhoi;
        zi   = (int) zf  ; zf   -= zi  ;
    }

};

}/* namespace olson_tools */


#endif // __FIELD_LOOKUP_H
