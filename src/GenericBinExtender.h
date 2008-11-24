// -*- c++ -*-
// $Id$
/*
 * Copyright 2005 Spencer Olson
 *
 * $Log$
 *
 */


/** \file
 * A generic way to create higher dimensional histogramming classes.
 *
 * Copyright 2004-2005 Spencer Olson.
 *
 * @see GenericBin and KeyedBin.
 */

/** \example bin/extended_2d/generic/testbin.C
 * Demonstrates the binning extender so that two-dimensional data can be
 * binned.  This demonstration extends the simple GenericBin to two dimensions.  
 *
 * @see GenericBinExtender,GenericBin
 */

/** \example bin/extended_2d/keyed/testbin.C
 * Demonstrates the binning extender so that two-dimensional data can be
 * binned.  This demonstration extends the KeyedBin to two dimensions.  
 *
 * @see GenericBinExtender,KeyedBin
 */


#ifndef GENERICBINEXTENDER_H
#define GENERICBINEXTENDER_H

#include <iostream>
#include <string.h>
#include "Vector.h"
#include "strutil.h"
#include <cfloat>

#ifdef USE_MPI
#  include "mpi_init.h"
#endif

namespace olson_tools {

/** A histogramming dimension extender.
 *
 * If MPI is used, this class supplies its own SUM operator for MPI::Reduce
 * operations.  This class assumes that the subclass T2 has its own
 * specialization of BlockAdder::*.  
 *
 * @param TKey
 *     The type of the key to base histogram (double, int, ...).
 *
 * @param T2
 *     The type to return when calling getBin().
 *
 * @param nbins
 *     The number of bins to for the key (thus also the number of T2 elements
 *     in the internal binarray (an individual element is returned by
 *     getBin()).
 *
 * @see GenericBin for a generic histogramming only class.
 * @see KeyedBin for a keyed histogramming class.
 */
template <class TKey, class T2, unsigned int nbins>
class GenericBinExtender {

    /** The maximum of the data range within which to histogram. */
    double max;

    /** The minimum of the data range within which to histogram. */
    double min;

    double scale;
  public:

    /** The actual bin containers holding the data. */
    T2 bins[nbins];

    /** Constructor.
     * @param mn
     *     Expected minimum of the data.
     *
     * @param mx
     *     Expected maximum of the data.
     *
     * @param mn2
     *     Expected minimum of data used for child-bins.  This helps with n-D
     *     application.
     *
     * @param mx2
     *     Expected minimum of data used for child-bins.  This helps with n-D
     *     application.
     */
    inline GenericBinExtender(const double & mn  = 0.0, const double & mx  = 0.0,
                              const double & mn2 = 0.0, const double & mx2 = 0.0) {
        init(mn,mx);
        if (mx2 != mn2) {
            initChildBins(mn2,mx2);
        }

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

    /** Call init of all child bins with the same arguments. */
    inline void initChildBins(const double & mn, const double & mx) {
        for(unsigned int i = 0; i < nbins; bins[i++].init(mn,mx));
    }

    /** Operator for multiplying the whole distrib by a factor. */
    template <class Tf>
    inline GenericBinExtender & operator*=(const Tf & factor) {
        for(unsigned int i = 0; i < nbins; bins[i++] *= factor );
        return *this;
    }

    /** Add a value to the histogram.
     * This function will only compile if the child bin type has a bin()
     * interface similar to KeyedBin.
     */
    template <class T3,unsigned int L>
    inline void bin(const TKey & key, const TKey & key2, const Vector<T3,L> & v) {
        getBin(key).bin(key2,v);
    }

    /** Add a predetermined value to the histogram.
     * This function will only compile if the child bin type has a bin()
     * interface similar to GenericBin.
     */
    inline void bin(const TKey & key, const TKey & key2) {
        getBin(key).bin(key2);
    }

    /** Gets the appropriate child bin.
     * This generic bin extender will allow an arbitrary dimension of bins.
     */
    inline T2 & getBin(const TKey & key) {
        register int i = int( ( (key<max?(key>min?key:min):max) - min) * scale);
        return bins[i];
    }

    /** Returns the size of the histogram. */
    inline unsigned int nBins() const { return nbins; }

    /** Sets the entire histogram to zero.
     * This call recursively calls clearBins on the child containers, i.e. it
     * doesn't actually do any work on its own.
     */
    inline void clearBins() {
        for (unsigned int i = 0; i < nbins; bins[i++].clearBins());
    }

    /** Stream the histogram out.
     * This function (recursively) calls the print function of the child bins
     * and then prints and extra newline character (compatible with gnuplot).
     *
     * @param output
     *     The output stream.
     *
     * @param prefix
     *     A string to prepend to each row of the output.
     */
    inline std::ostream & print(std::ostream & output, const std::string & prefix = "") const {
        for (unsigned int i = 0; i < nbins; i++) {
            std::string pref = prefix + to_string( (TKey) (min + ((((double)i) + 0.5)/scale)) ) + '\t';
            bins[i].print(output, pref) << '\n';
        }
        return output;
    }

    /** The minimum range of this histogrammer. */
    const double & getMin() const { return min; }

    /** The maximum range of this histogrammer. */
    const double & getMax() const { return max; }

    /** Add in the histogram values.  Note that only the histogram
     * and associated data is SUMMED. This function relies on the T2
     * class having an appropriate T2::operator+=(const T2 &) defined.
     */
    GenericBinExtender & operator+=(const GenericBinExtender & that) {
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
            msb.addBlocks<GenericBinExtender>();
            MPI.TYPE = msb.Create_struct();
        }

        static void finish() {
            MPI.SUM.Free();
            MPI.TYPE.Free();
        }

        /** Provide the equivalent of MPI::SUM.  
         * @see GenericBinExtender::operator+=(const GenericBinExtender &).
         */
        static void MPISUM(const GenericBinExtender* in, GenericBinExtender* inout, int len, const MPI::Datatype & typ) {
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
template <class TKey, class T2, unsigned int nbins>
typename GenericBinExtender<TKey,T2,nbins>::MPI_INFO GenericBinExtender<TKey,T2,nbins>::MPI = {
    MPI::OP_NULL,       /* SUM */
    MPI::DATATYPE_NULL  /* TYPE */
};


template <class TKey, class T2, unsigned int nbins>
int GenericBinExtender<TKey,T2,nbins>::added_mpi_init = GenericBinExtender<TKey,T2,nbins>::MPI.add_init();



/** blockAdder for GenericBinExtender. 
 * reference layout:
    struct GenericBinExtender {
        double max;
        double min;
        double scale;
        T2 bins[nbins];
    }
*/
template <class TKey, class T2, unsigned int nbins, class Block>
void blockAdder( std::vector<Block> & blocks,
                 const GenericBinExtender<TKey,T2,nbins> * null_ptr ) {
    int bi = MPIStructBuilder::total_size(blocks);
    blocks.push_back(Block(3,sizeof(double))); /* pad */
    for (unsigned int i = 0; i < nbins; i++) {
        blockAdder( blocks, (T2*)NULL );
        /* we assume that any class T2 will take care of its own alignement
         * problems. */
    }

    /* now we'll add any remaining padding necessary--helpful for arrays.
     * This should actually not really be necessary, but we'll do it anyway. */
    int pad = sizeof(GenericBinExtender<TKey,T2,nbins>)
            - (MPIStructBuilder::total_size(blocks) - bi);
    if (pad) blocks.push_back(Block(1,pad));    /* pad */
}
#endif // USE_MPI


} /*namespace olson_tools*/


#endif //GENERICBINEXTENDER_H
