// -*- c++ -*-
// $Id$
/** \file
 * Scale an intensity source.
 *
 * Copyright 2004-2007 Spencer Olson
 *
 */



#ifndef SCALEFIELD_H
#define SCALEFIELD_H

#include "Fields.h"
#include "Timing.h"

template <class Field>
class ScaleField : public virtual BaseField,
                   public Field {
  public:
    typedef BaseField      super0;
    typedef Field F0;

    static ExpTimingElement DefaultTiming;

    ScaleField () : super0(), F0(), timing() {
        /* default to having no timing effect. */
        timing.timings.push_back(&DefaultTiming);
        timing.current_val = DefaultTiming.getValue(DBL_MAX);
    }

    inline const ScaleField & operator=(const ScaleField & that) {
        super0::operator=(that);
        F0::operator=(that);
        timing = that.timing;
        return *this;
    }

    /** Return the timed value of the Scalar field together.
     * This will not instantiate with Vector Fields.
     * @param r
     *     The position to evaluate the field.
     * */
    inline double operator()(const Vector<double,3> & r) const {
        return timing.current_val * F0::operator()(r);
    }

    /** Return the timed value of the Vector field together.
     * This may not instantiate with Scalar Fields.
     * @param r
     *     The position to evaluate the field.
     * */
    template <class T>
    inline void operator()(T & F, const Vector<double,3> & r) const {
        F0::operator()(F,r);
        F *= timing.current_val;
    }

    Timing timing;
};

template <class F>
ExpTimingElement ScaleField<F>::DefaultTiming(-DBL_MAX,1.0,1.0,1.0);

#endif //SCALEFIELD_H
