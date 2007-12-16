// -*- c++ -*-
// $Id$
/*
 * Copyright 2005 Spencer Olson
 *
 * $Log$
 *
 */


/** \file
 * A keyed histogramming class to make it easier to perform mundane binning
 * tasks based on a keyed value.
 *
 * Copyright 2004-2005 Spencer Olson.
 *
 * @see GenericBin.
 */


#ifndef KEYEDBIN_H
#define KEYEDBIN_H

#include <iostream>
#include <string.h>
#include <cfloat>
#include "Vector.h"

/** A keyed histogramming class.
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
    }

    /** Initialize the binning. */
    inline void init(const double & mn, const double & mx) {
        max = mx;
        min = mn;
        scale = (mn==0 && mx==0 ? DBL_MAX : double(nbins)/(max - min) * 0.999999);
        clearBins();
    }

    /** Summed up bin-data. */
    Vector<T2,L> bins[nbins];

    /** Histogram of binning. */
    int hist[nbins];

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
    inline std::ostream & print(std::ostream & output, const std::string & prefix) const {
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
};

#endif //KEYEDBIN_H
