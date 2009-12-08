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
 * A keyed histogramming class to make it easier to perform mundane binning
 * tasks based on a keyed value.
 *
 * Copyright 2004-2008 Spencer Olson.
 *
 * @see GenericBin.
 */

/** \example bin/keyed/testbin.cpp
 * Demonstrates binning a set of data according to a specific single key value.
 * Instead of just incrementing a counter for each bin() operation as done in
 * GenericBin, the data is accumulated (summed).
 *
 * @see KeyedBin,GenericBin
 */


#ifndef KEYEDBIN_H
#define KEYEDBIN_H

#include <iostream>
#include <string.h>
#include <cfloat>
#include "Vector.h"

#ifdef USE_MPI
#  include "mpi_init.h"
#endif

namespace olson_tools {

/** A keyed histogramming class.
 *
 * If MPI is used, this class supplies its own SUM operator for MPI::Reduce
 * operations.  If a non standard bin type is used (i.e. not double, float,
 * int), then the user will have to specialize BlockAdder::* for GenericBin
 * using the standard specializations below.  
 *
 * @param TKey
 *     The type of the key to base histogram (double, int, ...).
 *
 * @param T2
 *     The base type of the Vector<T2,L> data to store (double, int, ...).
 *
 * @param L
 *     The length of the Vector<T2,L> data to store.
 *
 * @param nbins
 *     The number of bins to use for the histogram.
 *
 * @see GenericBin for a generic histogramming only class.
 */
template <class TKey, class T2, unsigned int L, unsigned int nbins>
class KeyedBin {

    /** The maximum of the data range within which to histogram. */
    double max;

    /** The minimum of the data range within which to histogram. */
    double min;

    /** The scale factor for bin length. */
    double scale;

    /** Output an extra newline every print_extra_newline_mod lines.
     Defaults to 0 == never. */
    int print_extra_newline_mod;
  public:

    /** Summed up bin-data. */
    Vector<T2,L> bins[nbins];

    /** Histogram of binning. */
    int hist[nbins];

    /** Constructor.
     * @param mn
     *     Expected minimum of the data.
     *
     * @param mx
     *     Expected maximum of the data.
     */
    inline KeyedBin(const double & mn = 0.0, const double & mx = 0.0, const int & _penl_mod = 0) {
        init(mn,mx);
        print_extra_newline_mod = _penl_mod;

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

    /** Add a value to the histogram. */
    inline void bin(const TKey & key, const Vector<T2,L> & v) {
        register int i = int( ( (key<max?(key>min?key:min):max) - min) * scale);
        bins[i] += v;
        hist[i]++;
    }

    /** Returns the size of the histogram. */
    inline unsigned int nBins() const { return nbins; }

    /** Sets the entire histogram to zero. */
    inline void clearBins() {
        memset(bins,0,sizeof(Vector<T2,L>)*nbins);
        memset(hist,0,sizeof(int)*nbins);
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
                   << hist[i] << '\t'
                   << bins[i] << '\n';
            if (print_extra_newline_mod > 0 &&
                ((1 + i) % print_extra_newline_mod) == 0) {
                output << '\n';
            }
        }/* for */
        return output;
    }

    /** The minimum range of this histogrammer. */
    const double & getMin() const { return min; }

    /** The maximum range of this histogrammer. */
    const double & getMax() const { return max; }

    /** Add in the histogram values.  Note that only the histogram
     * and associated data is SUMMED. This function relies on the
     * Vector::operator+=(const Vector&) function.
     */
    KeyedBin & operator+=(const KeyedBin & that) {
        for (unsigned int j = 0; j < nbins; j++) {
            bins[j] += that.bins[j];
            hist[j] += that.hist[j];
        }
        return *this;
    }

#ifdef USE_MPI
    typedef struct {
        MPI::Op SUM;
        MPI::Datatype TYPE;

        static void init() {
            MPI.SUM.Init(reinterpret_cast<MPI::User_function*>(&MPISUM), true);

            olson_tools::MPIStructBuilder msb;
            msb.addBlocks<KeyedBin>();
            MPI.TYPE = msb.Create_struct();
        }

        static void finish() {
            MPI.SUM.Free();
            MPI.TYPE.Free();
        }

        /** Provide the equivalent of MPI::SUM.  
         * @see KeyedBin::operator+=(const KeyedBin &).
         */
        static void MPISUM(const KeyedBin* in, KeyedBin* inout, int len, const MPI::Datatype & typ) {
            for (int i = 0; i < len; i++) {
                inout[i] += in[i];
            }
        }

        static int add_init() {
            olson_tools::MPIInit::add_init(init);
            olson_tools::MPIInit::add_finish(finish);
            return 1;
        }

    } MPI_INFO;

    static MPI_INFO MPI;

    static int added_mpi_init;
#endif // USE_MPI
};



#ifdef USE_MPI
template <class TKey, class T2, unsigned int L, unsigned int nbins>
typename KeyedBin<TKey,T2,L,nbins>::MPI_INFO KeyedBin<TKey,T2,L,nbins>::MPI = {
    MPI::OP_NULL,       /* SUM */
    MPI::DATATYPE_NULL  /* TYPE */
};


template <class TKey, class T2, unsigned int L, unsigned int nbins>
int KeyedBin<TKey,T2,L,nbins>::added_mpi_init = KeyedBin<TKey,T2,L,nbins>::MPI.add_init();



/** blockAdder for KeyedBin<TKey,T2 = double,L,nbins>.
 * reference layout:
    struct KeyedBin {
        double max;
        double min;
        double scale;
        int print_extra_newline_mod;
        Vector<T2,L> bins[nbins];
        int hist[nbins];
    }
*/
template <class TKey, unsigned int L, unsigned int nbins, class Block>
void blockAdder( std::vector<Block> & blocks,
                 const KeyedBin<TKey,double,L,nbins> * null_ptr ) {
    /* my hack to take care of alignment issues.  I tried looking at the
     * MPI::*::Get_extent and company functions, but couldn't tell how
     * they might be able to test for when integers aren't always padded
     * as can be if two integers follow each other. */
    KeyedBin<TKey,double,L,nbins> * bob = 0x0;
    int offs = ((char*)&bob->bins) - ((char*)bob);
    blocks.push_back(Block(1,offs));    /* pad */
    blocks.push_back(Block(nbins*L,sizeof(double),MPI::DOUBLE)); /* v */
    blocks.push_back(Block(nbins,sizeof(int),MPI::INT)); /* hist */

    /* now we'll add any remaining padding necessary--helpful for arrays */
    int pad = sizeof(KeyedBin<TKey,double,L,nbins>)
            - (offs + nbins*L*sizeof(double) + nbins*sizeof(int));
    if (pad) blocks.push_back(Block(1,pad));    /* pad */
}

/** blockAdder for KeyedBin<TKey,T2 = float,L,nbins>.
 * reference layout:
    struct KeyedBin {
        double max;
        double min;
        double scale;
        int print_extra_newline_mod;
        Vector<T2,L> bins[nbins];
        int hist[nbins];
    }
*/
template <class TKey, unsigned int L, unsigned int nbins, class Block>
void blockAdder( std::vector<Block> & blocks,
                 const KeyedBin<TKey,float,L,nbins> * null_ptr ) {
    /* my hack to take care of alignment issues.  I tried looking at the
     * MPI::*::Get_extent and company functions, but couldn't tell how
     * they might be able to test for when integers aren't always padded
     * as can be if two integers follow each other. */
    KeyedBin<TKey,float,L,nbins> * bob = 0x0;
    int offs = ((char*)&bob->bins) - ((char*)bob);
    blocks.push_back(Block(1,offs));    /* pad */
    blocks.push_back(Block(nbins*L,sizeof(float),MPI::FLOAT)); /* v */
    blocks.push_back(Block(nbins,sizeof(int),MPI::INT)); /* hist */

    /* now we'll add any remaining padding necessary--helpful for arrays */
    int pad = sizeof(KeyedBin<TKey,float,L,nbins>)
            - (offs + nbins*L*sizeof(float) + nbins*sizeof(int));
    if (pad) blocks.push_back(Block(1,pad));    /* pad */
}

/** blockAdder for KeyedBin<TKey,T2 = int,L,nbins>.
 * reference layout:
    struct KeyedBin {
        double max;
        double min;
        double scale;
        int print_extra_newline_mod;
        Vector<T2,L> bins[nbins];
        int hist[nbins];
    }
*/
template <class TKey, unsigned int L, unsigned int nbins, class Block>
void blockAdder( std::vector<Block> & blocks,
                 const KeyedBin<TKey,int,L,nbins> * null_ptr ) {
    /* my hack to take care of alignment issues.  I tried looking at the
     * MPI::*::Get_extent and company functions, but couldn't tell how
     * they might be able to test for when integers aren't always padded
     * as can be if two integers follow each other. */
    KeyedBin<TKey,int,L,nbins> * bob = 0x0;
    int offs = ((char*)&bob->bins) - ((char*)bob);
    blocks.push_back(Block(1,offs));    /* pad */
    blocks.push_back(Block(nbins*L,sizeof(int),MPI::INT)); /* v */
    blocks.push_back(Block(nbins,sizeof(int),MPI::INT)); /* hist */

    /* now we'll add any remaining padding necessary--helpful for arrays */
    int pad = sizeof(KeyedBin<TKey,int,L,nbins>)
            - (offs + nbins*L*sizeof(int) + nbins*sizeof(int));
    if (pad) blocks.push_back(Block(1,pad));    /* pad */
}
#endif // USE_MPI


} /*namespace olson_tools*/


#endif //KEYEDBIN_H
