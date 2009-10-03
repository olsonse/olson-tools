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
 * A generic binning class to make it easier to perform mundane binning tasks.
 *
 * Copyright 2004-2008 Spencer Olson.
 */

/** \example bin/generic/testbin.C
 * Demonstrates the simple binning utility class. 
 * @see GenericBin
 */


#ifndef GENERICBIN_H
#define GENERICBIN_H

#include <iostream>
#include <string.h>
#include <cfloat>

#ifdef USE_MPI
#  include "mpi_init.h"
#endif

namespace olson_tools {

/** A generic histogramming class.
 *
 * If MPI is used, this class supplies its own SUM operator for MPI::Reduce
 * operations.  If a non standard bin type is used (i.e. not double, float,
 * int), then the user will have to specialize BlockAdder::* for GenericBin
 * using the standard specializations below.  
 *
 * @param TKey
 *     The type of the histogram key (double, int, ...).
 *
 * @param nbins
 *     The number of bins to use for the histogram.
 *
 * @param TBin
 *     The type of data to histogram (double, int, ...).
 */
template <class TKey, unsigned int nbins, class TBin = int>
class GenericBin {

    /** The maximum of the data range within which to histogram. */
    double max;

    /** The minimum of the data range within which to histogram. */
    double min;

    double scale;
  public:

    /** The actual histogram. */
    TBin bins[nbins];

    /** Constructor.
     * @param mn
     *     Expected minimum of the data.
     *
     * @param mx
     *     Expected maximum of the data.
     */
    inline GenericBin(const double & mn = 0.0, const double & mx = 0.0) {
        init(mn,mx);

#ifdef USE_MPI
        /* just to ensure that this is not optimized away. */
        int * a = &added_mpi_init;
        (void) ((*a)*1);
#endif
    }

    /** Initialize the binning. */
    inline void init(const double & mn, const double & mx) {
        max = mx;
        min = mn;
        scale = (mn==0 && mx==0 ? DBL_MAX : double(nbins)/(max - min) * 0.999999);
        clearBins();
    }

    /** Gets the appropriate child bin.
     * This generic bin extender will allow an arbitrary dimension of bins.
     */
    inline TBin & getBin(const TKey & key) {
        register int i = int( ( (key<max?(key>min?key:min):max) - min) * scale);
        return bins[i];
    }

    /** Increment (by 1) the bin for the specified key value to the histogram. */
    inline void bin(const TKey & key) {
        getBin(key)++;
    }

    /** Returns the size of the histogram. */
    inline unsigned int nBins() const { return nbins; }

    /** Sets the entire histogram to zero. */
    inline void clearBins() {
        memset(bins,0,sizeof(TBin)*nbins);
    }

    /** Operator for multiplying the whole distrib by a factor. */
    template <class Tf>
    inline GenericBin & operator*=(const Tf & factor) {
        for(unsigned int i = 0; i < nbins; i++ ) 
            bins[i] = (TBin) ( factor * bins[i]);
        return *this;
    }

    /** Stream the histogram out.
     * @param output
     *     The output stream.
     *
     * @param prefix
     *     A string to prepend to each row of the output.
     */
    inline std::ostream & print(std::ostream & output, const std::string & prefix = "") const {
        for (unsigned int i = 0; i < nbins; i++) {
            output << prefix
                   << ( (TKey) (min + ((((double)i) + 0.5)/scale)) ) << '\t'
                   << bins[i] << '\n';
        }/* for */
        return output;
    }

    /** The minimum range of this histogrammer. */
    const double & getMin() const { return min; }

    /** The maximum range of this histogrammer. */
    const double & getMax() const { return max; }

    /** Add in the histogram values.  Note that only the histogram
     * and associated data is SUMMED. This function relies on the TBin
     * class having an appropriate TBin::operator+=(const TBin &) defined.
     */
    GenericBin & operator+=(const GenericBin & that) {
        for (unsigned int j = 0; j < nbins; j++) {
            bins[j] += that.bins[j];
        }
        return *this;
    }

#ifdef USE_MPI
    typedef struct {
        MPI::Op SUM;
        MPI::Datatype TYPE;

        static void init() {
            MPI.SUM.Init(reinterpret_cast<MPI::User_function*>(&MPISUM), true);

            MPIStructBuilder msb;
            msb.addBlocks<GenericBin>();
            MPI.TYPE = msb.Create_struct();
        }

        static void finish() {
            MPI.SUM.Free();
            MPI.TYPE.Free();
        }

        /** Provide the equivalent of MPI::SUM.  
         * @see GenericBin::operator+=(const GenericBin &).
         */
        static void MPISUM(const GenericBin* in, GenericBin* inout, int len, const MPI::Datatype & typ) {
            for (int i = 0; i < len; i++) {
                inout[i] += in[i];
            }
        }

        static int add_init() {
            MPIInit::add_init(init);
            MPIInit::add_finish(finish);
            return 1;
        }

    } MPI_INFO;

    static MPI_INFO MPI;

    static int added_mpi_init;
#endif // USE_MPI
};


#ifdef USE_MPI
template <class TKey, unsigned int nbins, class TBin>
typename GenericBin<TKey,nbins,TBin>::MPI_INFO GenericBin<TKey,nbins,TBin>::MPI = {
    MPI::OP_NULL,       /* SUM */
    MPI::DATATYPE_NULL  /* TYPE */
};


template <class TKey, unsigned int nbins, class TBin>
int GenericBin<TKey,nbins,TBin>::added_mpi_init = GenericBin<TKey,nbins,TBin>::MPI.add_init();



/** blockAdder for GenericBin<Tkey,nbins,TBin = double>. 
 * reference layout:
    struct GenericBin {
        double max;
        double min;
        double scale;
        TBin bins[nbins];
    }
*/
template <class TKey, unsigned int nbins, class Block>
void blockAdder( std::vector<Block> & blocks,
                 const GenericBin<TKey,nbins,double> * null_ptr ) {
    blocks.push_back(Block(3,sizeof(double))); /* pad */
    blocks.push_back(Block(nbins,sizeof(double),MPI::DOUBLE));
    /* shouldn't have to worry about alignment issues for doubles (yet) */
}

/** blockAdder for GenericBin<Tkey,nbins,TBin = Vector<double,L> >. 
 * reference layout:
    struct GenericBin {
        double max;
        double min;
        double scale;
        TBin bins[nbins];
    }
*/
template <class TKey, unsigned int nbins, class Block, unsigned int L>
void blockAdder( std::vector<Block> & blocks,
                 const GenericBin<TKey,nbins,Vector<double,L> > * null_ptr ) {
    blocks.push_back(Block(3,sizeof(double))); /* pad */
    blocks.push_back(Block(L*nbins,sizeof(double),MPI::DOUBLE));
    /* shouldn't have to worry about alignment issues for doubles (yet) */
}

/** blockAdder for GenericBin<Tkey,nbins,TBin = float>. 
 * reference layout:
    struct GenericBin {
        double max;
        double min;
        double scale;
        TBin bins[nbins];
    }
*/
template <class TKey, unsigned int nbins, class Block>
void blockAdder( std::vector<Block> & blocks,
                 const GenericBin<TKey,nbins,float> * null_ptr ) {
    blocks.push_back(Block(3,sizeof(double))); /* pad */
    blocks.push_back(Block(nbins,sizeof(float),MPI::FLOAT));

    /* now we'll add any remaining padding necessary--helpful for arrays */
    int pad = sizeof(GenericBin<TKey,nbins,float>)
            - (3*sizeof(double) + nbins*sizeof(float));
    if (pad) blocks.push_back(Block(1,pad));    /* pad */
}

/** blockAdder for GenericBin<Tkey,nbins,TBin = Vector<float,L> >. 
 * reference layout:
    struct GenericBin {
        double max;
        double min;
        double scale;
        TBin bins[nbins];
    }
*/
template <class TKey, unsigned int nbins, class Block, unsigned int L>
void blockAdder( std::vector<Block> & blocks,
                 const GenericBin<TKey,nbins,Vector<float,L> > * null_ptr ) {
    blocks.push_back(Block(3,sizeof(double))); /* pad */
    blocks.push_back(Block(L*nbins,sizeof(float),MPI::FLOAT));

    /* now we'll add any remaining padding necessary--helpful for arrays */
    int pad = sizeof(GenericBin<TKey,nbins,Vector<float,L> >)
            - (3*sizeof(double) + L*nbins*sizeof(Vector<float,L>));
    if (pad) blocks.push_back(Block(1,pad));    /* pad */
}

/** blockAdder for GenericBin<Tkey,nbins,TBin = int>. 
 * reference layout:
    struct GenericBin {
        double max;
        double min;
        double scale;
        TBin bins[nbins];
    }
*/
template <class TKey, unsigned int nbins, class Block>
void blockAdder( std::vector<Block> & blocks,
                 const GenericBin<TKey,nbins,int> * null_ptr ) {
    blocks.push_back(Block(3,sizeof(double))); /* pad */
    blocks.push_back(Block(nbins,sizeof(int),MPI::INT));

    /* now we'll add any remaining padding necessary--helpful for arrays */
    int pad = sizeof(GenericBin<TKey,nbins,int>)
            - (3*sizeof(double) + nbins*sizeof(int));
    if (pad) blocks.push_back(Block(1,pad));    /* pad */
}

/** blockAdder for GenericBin<Tkey,nbins,TBin = Vector<int,L> >. 
 * reference layout:
    struct GenericBin {
        double max;
        double min;
        double scale;
        TBin bins[nbins];
    }
*/
template <class TKey, unsigned int nbins, class Block, unsigned int L>
void blockAdder( std::vector<Block> & blocks,
                 const GenericBin<TKey,nbins,Vector<int,L> > * null_ptr ) {
    blocks.push_back(Block(3,sizeof(double))); /* pad */
    blocks.push_back(Block(L*nbins,sizeof(int),MPI::INT));

    /* now we'll add any remaining padding necessary--helpful for arrays */
    int pad = sizeof(GenericBin<TKey,nbins,Vector<int,L> >)
            - (3*sizeof(double) + L*nbins*sizeof(Vector<int,L>));
    if (pad) blocks.push_back(Block(1,pad));    /* pad */
}
#endif // USE_MPI


} /*namespace olson_tools*/

#endif //GENERICBIN_H
