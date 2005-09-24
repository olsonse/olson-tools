// -*- c++ -*-
// $Id$
/*
 * Copyright 2005 Spencer Olson
 *
 * $Log$
 *
 */


#ifndef KEYEDBIN_H
#define KEYEDBIN_H

#include <iostream>
#include <string.h>
#include "Vector.h"

/** A keyed histogramming class.
 * @param T
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
template <class T, class T2, unsigned int L, unsigned int nbins>
class KeyedBin {

    /** The maximum of the data range within which to histogram. */
    T max;

    /** The minimum of the data range within which to histogram. */
    T min;

    double scale;
  public:

    /** Constructor.
     * @param mn
     *     Expected minimum of the data.
     *
     * @param mx
     *     Expected maximum of the data.
     */
    inline KeyedBin(const T & mn, const T & mx) :
        max(mx), min(mn), scale(double(nbins)/(max - min) * 0.999999) {
        clearBins();
    }

    /** Summed up bin-data. */
    Vector<T2,L> bins[nbins];

    /** Histogram of binning. */
    int hist[nbins];

    /** Add a value to the histogram. */
    inline void bin(const T & key, const Vector<T2,L> & v) {
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
                   << i << '\t'
                   << hist[i] << '\t'
                   << bins[i] << '\n';
        }/* for */
        return output;
    }
};

#endif //KEYEDBIN_H
