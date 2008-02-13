
#ifndef TIMING_H
#define TIMING_H

#include <vector>
#include <stdexcept>

/** Abstract timing element class. 
 * In general, each timing element only operates in relative time.  An array
 * of these elements will then describe how a particular value (of something)
 * changes over the length of absolute time.  
 *
 * @see Timing
 */
class TimingElement {
  public:
    /** Length of this time interval. */
    double dt;

    TimingElement( const double & _dt   = 0.0 ) : dt(_dt) { }
    virtual ~TimingElement() {}

    /** return the value of this interval at t_rel relative time.
     * @param t_rel relative time.
     */
    virtual double getValue(const double & t_rel) const = 0;
};

/** Exponential rise/fall timing element. 
 * This timing element will change its value over the length of the element
 * (dt) from val_i to val_f.  The exponent and reverse parameters determine
 * the type of change (i.e. the type of exponential rise/fall).
 */
class ExpTimingElement : public TimingElement {
  public:
    typedef TimingElement super;
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

    ExpTimingElement( const double & _dt   = 0.0,
                   const double & exp   = 1.0,
                   const double & vi    = 0.0,
                   const double & vf    = 1.0 ) :
        super(_dt), exponent(fabs(exp)), reverse(exp < 0.0), val_i(vi), val_f(vf) { }
    virtual ~ExpTimingElement() {}

    virtual double getValue(const double & t_rel) const {
        const double eps10 = 1e-14; /* to avoid imaginary numbers */
        const double tau = (t_rel / dt);

        if (t_rel >= dt) {
            return val_f;
        } else if (t_rel <= 0.0) {
            return val_i;
        } else if (reverse) {
            return val_f + (  (val_i - val_f) * fast_pow((1.0 - tau) + eps10, exponent));
        } else { /* forward */
            return val_i + (  (val_f - val_i) * fast_pow(tau + eps10, exponent));
        }
    }
};

/** Generic timing class.  
 * This class can be used to change a particular value in a controlled fashion
 * over a given set of time intervals (defined by an array of TimeElement
 * instances).
 *
 * @see TimingElement
 */
class Timing {
  public:
    Timing() : current_val(0.0) {}

    /** Set the current value of the timed change.
     * @param t_absolute The absolute time will define which element in the
     * time interval array is used. 
     */
    void set_timing(const double & t_absolute) {
        double t_i = 0.0, t_f = 0.0;
        int i = 0;
        for (; i < timings.size(); i++) {
            TimingElement & te = *timings[i];

            t_i = t_f;         /* absolute start of this interval. */
            t_f = t_i + te.dt; /* absolute   end of this interval. */

            if (t_absolute <= t_f) {
                i++;
                break;
            }
        }
        i--;
        if (i < 0) throw std::runtime_error("There are no timing elements!");

        /* set the current value according to the relative time for the ith
         * time interval. */
        current_val = timings[i]->getValue(t_absolute - t_i);
    }

    double current_val;
    std::vector<TimingElement *> timings;
};

#endif // TIMING_H
