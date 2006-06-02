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
 * @param T
 *     The type of data to histogram (double, int, ...).
 *
 * @param nbins
 *     The number of bins to use for the histogram.
 */
template <class T, unsigned int nbins>
class GenericBin {

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
    inline GenericBin(const T & mn = (T)0, const T & mx = (T)0) {
        init(mn,mx);
    }

    /** Initialize the binning. */
    inline void init(const T & mn, const T & mx) {
        max = mx;
        min = mn;
        scale = (mn==0 && mx==0 ? DBL_MAX : double(nbins)/(max - min) * 0.999999);
        clearBins();
    }

    /** The actual histogram. */
    int bins[nbins];

    /** Add a value to the histogram. */
    inline void bin(const T & v) {
        bins[int( ( (v<max?(v>min?v:min):max) - min) * scale)]++;
    }

    /** Returns the size of the histogram. */
    inline unsigned int nBins() const { return nbins; }

    /** Sets the entire histogram to zero. */
    inline void clearBins() {
        memset(bins,0,sizeof(int)*nbins);
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
                   << bins[i] << '\n';
        }/* for */
        return output;
    }
};

#endif //GENERICBIN_H
