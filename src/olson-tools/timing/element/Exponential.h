
#ifndef TIMING_H
#define TIMING_H

#include <olson-tools/timing/element/Base.h>

#include <olson-tools/power.h>

#include <limits>


namespace olson_tools {
  namespace timing {
    namespace element {

      /** Exponential rise/fall timing element. 
       * This timing element will change its value over the length of the element
       * (dt) from val_i to val_f.  The exponent and reverse parameters determine
       * the type of change (i.e. the type of exponential rise/fall).
       */
      struct Exponential : timing::element::Base {
        /* TYPEDEFS */
        typedef timing::element::Base super;

        /* MEMBER STORAGE */
        /** Exponent of change. */
        double exponent;
        /** Boolean to specify whether \f$ (1-z)^{k} \f$ (reverse==true) or 
         * \f$ z^{k} \f$ (reverse==false) is computed for the exponential rise/fall. 
         */
        bool reverse;
        /** Initial value of this interval. */
        double val_i;
        /** Final value of this interval. */
        double val_f;



        /* MEMBER FUNCTIONS */
        /** Constructor. */
        Exponential( const double & _dt   = 0.0,
                     const double & exp   = 1.0,
                     const double & vi    = 0.0,
                     const double & vf    = 1.0 ) :
          super(_dt),
          exponent(fabs(exp)),
          reverse(exp < 0.0),
          val_i(vi), val_f(vf) { }

        /** Required virtual destructor.  This one is a NO-OP. */
        virtual ~Exponential() {}

        /** Calculates the value of this timing element. */
        virtual double getValue(const double & t_rel) {
          /* to avoid imaginary numbers: */
          const double eps10 = 10. * std::numeric_limits<double>::epsilon();

          const double tau = (t_rel / dt);
          using olson_tools::fast_pow;

          if (t_rel >= dt)
            return val_f;
          else if (t_rel <= 0.0)
            return val_i;
          else if (reverse)
            return val_f + (  (val_i - val_f) * fast_pow((1.0 - tau) + eps10, exponent));
          else /* forward */
            return val_i + (  (val_f - val_i) * fast_pow(tau, exponent));
        }
      };

    }/* namespace olson_tools::timing::element */
  }/* namespace olson_tools::timing */
}/* namespace olson_tools */

#endif // TIMING_H
