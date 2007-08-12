// -*- c++ -*-
// $Id$
/*
 * Copyright 2004-2005 Spencer Olson
 *
 * $Log$
 *
 */

/** \file
 * A generic binning class to make it easier to perform mundane binning tasks.
 *
 * Copyright 2004-2005 Spencer Olson.
 */


#ifndef GENERICBIN_H
#define GENERICBIN_H

#include <iostream>
#include <string.h>
#include <cfloat>

/** A generic histogramming class.
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

    /** Constructor.
     * @param mn
     *     Expected minimum of the data.
     *
     * @param mx
     *     Expected maximum of the data.
     */
    inline GenericBin(const double & mn = 0.0, const double & mx = 0.0) {
        init(mn,mx);
    }

    /** Initialize the binning. */
    inline void init(const double & mn, const double & mx) {
        max = mx;
        min = mn;
        scale = (mn==0 && mx==0 ? DBL_MAX : double(nbins)/(max - min) * 0.999999);
        clearBins();
    }

    /** The actual histogram. */
    TBin bins[nbins];

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
                   << ( (TKey) (min + (((double)i)/scale)) ) << '\t'
                   << bins[i] << '\n';
        }/* for */
        return output;
    }
};

#endif //GENERICBIN_H
