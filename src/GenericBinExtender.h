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


#ifndef GENERICBINEXTENDER_H
#define GENERICBINEXTENDER_H

#include <iostream>
#include <string.h>
#include "Vector.h"
#include "strutil.h"
#include <cfloat>

/** A histogramming dimension extender.
 * @param T
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
template <class T, class T2, unsigned int nbins>
class GenericBinExtender {

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
     *
     * @param mn2
     *     Expected minimum of data used for child-bins.  This helps with n-D
     *     application.
     *
     * @param mx2
     *     Expected minimum of data used for child-bins.  This helps with n-D
     *     application.
     */
    inline GenericBinExtender(const T & mn  = (T)0, const T & mx  = (T)0,
                              const T & mn2 = (T)0, const T & mx2 = (T)0) {
        init(mn,mx);
        if (mx2 != mn2) {
            initChildBins(mn2,mx2);
        }
    }

    /** Initialize the binning. */
    inline void init(const T & mn, const T & mx) {
        max = mx;
        min = mn;
        scale = (mn==0 && mx==0 ? DBL_MAX : double(nbins)/(max - min) * 0.999999);
        clearBins();
    }

    /** Call init of all child bins with the same arguments. */
    inline void initChildBins(const T & mn, const T & mx) {
        for(unsigned int i = 0; i < nbins; bins[i++].init(mn,mx));
    }

    /** Operator for multiplying the whole distrib by a factor. */
    template <class Tf>
    inline GenericBinExtender & operator*=(const Tf & factor) {
        for(unsigned int i = 0; i < nbins; bins[i++] *= factor );
        return *this;
    }

    /** The actual bin containers holding the data. */
    T2 bins[nbins];

    /** Add a value to the histogram.
     * This function will only compile if the child bin type has a bin()
     * interface similar to KeyedBin.
     */
    template <class T3,unsigned int L>
    inline void bin(const T & key, const T & key2, const Vector<T3,L> & v) {
        getBin(key).bin(key2,v);
    }

    /** Gets the appropriate child bin.
     * This generic bin extender will allow an arbitrary dimension of bins.
     */
    inline T2 & getBin(const T & key) {
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
            std::string pref = prefix + to_string( (T) (min + (((double)i)/scale)) ) + '\t';
            bins[i].print(output, pref) << '\n';
        }
        return output;
    }
};

#endif //GENERICBINEXTENDER_H
