// -*- c++ -*-
// $Id$
/*
 * Copyright 2005-2006 Spencer Olson
 *
 * $Log$
 *
 */

/** \file
 * Force lookup specialization of FieldLookup class.
 * @see createFieldFile.h for routines to help creating the field-lookup table
 * file.
 */

/** \example field/lookup/testfield.C
 * \input field/lookup/common.h
 *
 * Demonstrates the field lookup code by using a set of field/force data from
 * a previously calculated field file.  The data pertains to magnetic forces
 * on an atom in the trapping hyperfine ground-state.
 */

/** \example field/lookup/createfieldfile.C
 * \input field/lookup/common.h
 *
 * Demonstrates the field field writing utility that produces a file that can
 * be read in by the field-lookup code.  The data pertains to magnetic forces
 * on an atom in the trapping hyperfine ground-state.
 */


#ifndef __FORCE_LOOKUP_H
#define __FORCE_LOOKUP_H

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "ompexcept.h"
#include "field-lookup.h"


/* ************** BEGIN FORCE LOOKUP SPECIALIZATION *********** */

template <unsigned int L = 3U>
class ForceRecord {
  public:
    ForceRecord() : a(0.0), V(0.0) {}
    Vector<double,L> a;
    double V;

    /* This is an attempt to generalize the vector_lookup function but not
     * implement it as a macro (which would probably be faster though). */

    /** For using the FieldLookup::vector_lookup routine. */
    inline Vector<double,L> & vector(const unsigned int & i) { return a; }

    /** For using the FieldLookup::vector_lookup routine. */
    inline const Vector<double,L> & vector(const unsigned int & i) const { return a; }

    /** For using the FieldLookup::scalar_lookup routine. */
    inline double & scalar(const unsigned int & i) { return V; }

    /** For using the FieldLookup::scalar_lookup routine. */
    inline const double & scalar(const unsigned int & i) const { return V; }
};

template <unsigned int L>
inline std::istream & operator>>(std::istream & input, ForceRecord<L> & fr) {
    input >> fr.a >> fr.V;
    return input;
}

template <unsigned int L>
inline std::ostream & operator<<(std::ostream & output, const ForceRecord<L> & fr) {
    output << fr.a << '\t' << fr.V;
    return output;
}

/** The base Lookup class of the associated FieldLookup container class.
 * If you have an axially symmetric force, you can more optimally use
 * AxiSymFieldLookup< ForceRecord<L> >.  This will both speed up the lookup
 * process as well as minimize the memory footprint.
 *
 * @see FieldLookup for cartesian lookup table.
 * @see AxiSymFieldLookup for axially symmetric lookup table.
 */
template <unsigned int L = 3U, class T = FieldLookup< ForceRecord<L> > >
class ForceLookup : public T {
  public:
    typedef T super;
    inline void accel(Vector<double,3> & a,
                      const Vector<double,3> & r,
                      const Vector<double,3> & v = V3(0,0,0),
                      const double & t = 0.0,
                      const double & dt = 0.0) const {
        super::vector_lookup(a, r, 0);
    }

    inline double potential(const Vector<double,3> & r,
                            const Vector<double,3> & v = V3(0,0,0),
                            const double & t = 0.0) const {
        return super::scalar_lookup(r, 0);
    }
}; /* ForceLookup class */

template <class T, unsigned int L = 3U>
class ForceTableWrapper : public T {
  public:
    typedef T super;
    ForceRecord<L> getRecord(const Vector<double,3> & r) const {
        ForceRecord<L> retval;
        super::accel(retval.a, r);
        retval.V = super::potential(r);
        return retval;
    }
};

/* **************   END FORCE LOOKUP SPECIALIZATION *********** */


#endif // __FORCE_LOOKUP_H
