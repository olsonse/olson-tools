
#ifndef TIMING_H
#define TIMING_H

#include <vector>
#include <stdexcept>
#include "m_eps.h"

/**
 * \example timing/testtiming.C
 * An example of how use the timing code.  This code mimics the functionality
 * of the timing in my Arbitrary Waveform Generator program.  
 @see Timing,TimingElement,ExpTimingElement
 */

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
    virtual double getValue(const double & t_rel) = 0;
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

    virtual double getValue(const double & t_rel) {
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
    Timing() : current_val(0.0),
               timings(), time_stack(),
               current_time_absolute(0.0) {}

    /** saves an absolute time on the time stack. */
    Timing & push_time(const double & t_abs) {
        time_stack.push_back(t_abs);
        return *this;
    }

    /** save current absolute time on the time stack.
     * @return Reference to self.  
     */
    Timing & push_time() {
        return push_time(current_time_absolute);
    }

    /** pop and restore an old timing value from the time stack. 
     * @return Reference to self.
     */
    Timing & pop_time() {
        double t_abs = time_stack.back();
        this->set_timing(t_abs);
        time_stack.pop_back();
        return *this;
    }

    /** Set the current value of the timed change.
     * @param t_absolute The absolute time will define which element in the
     * time interval array is used. 
     */
    void set_timing(const double & t_absolute) {
        /* set the current time for possible later use. */
        current_time_absolute = t_absolute;
        double t_i = 0.0, t_f = 0.0;
        int i = 0;
        for (; ((unsigned int)i) < timings.size(); i++) {
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

    /** vector of saved times.  Useful for printing as a function of t without
     * messing up with default time settings.  */
    std::vector<double> time_stack;

    /** Current absolute time:  last set time. */
    double current_time_absolute;
};


/** Generic timing print class.  
 * This class prints a set of timings out to file.
 *
 * @see Timing
 * @see TimingElement
 */
class TimingPrinter {
  public:
    TimingPrinter() {}

    void printTimings(const std::string & filename,
                      const double & ti,
                      const double & dt,
                      const double & tf) {
        std::ofstream fout(filename.c_str());

        /* save old timing values. */
        for (unsigned int i = 0; i < timers.size(); i++) {
            timers[i]->push_time();
        }

        double t_max = tf * (1.0 + 10.0 * M_EPS);
        for (double t = ti; t <= t_max ; t+=dt) {
            fout << t << '\t';

            for (unsigned int i = 0; i < timers.size(); i++) {
                timers[i]->set_timing(t);
                fout << timers[i]->current_val << '\t';
            }

            fout << '\n';
        }

        fout << std::flush;
        fout.close();

        /* restore old timing values. */
        for (unsigned int i = 0; i < timers.size(); i++) {
            timers[i]->pop_time();
        }
    }

    std::vector<Timing *> timers;
};

#endif // TIMING_H
