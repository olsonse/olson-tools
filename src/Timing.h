
#ifndef TIMING_H
#define TIMING_H

#include <vector>
#include <stdexcept>

class TimingElement {
  public:
    double dt;
    double exponent;
    bool reverse;
    double val_i;
    double val_f;

    TimingElement( const double & _dt   = 0.0,
                   const double & exp   = 1.0,
                   const double & vi    = 0.0,
                   const double & vf    = 1.0 ) :
        dt(_dt), exponent(fabs(exp)), reverse(exp < 0.0), val_i(vi), val_f(vf) { }

    double getValue(const double & t_rel) const {
        const double eps10 = 1e-14; /* to avoid imaginary numbers */
        const double tau = (t_rel / dt);

        if (t_rel >= dt) {
            return val_f;
        } else if (t_rel <= 0.0) {
            return val_i;
        } else if (reverse) {
            return val_i + (  (val_f - val_i) * fast_pow((1.0 - tau) + eps10, exponent));
        } else { /* forward */
            return val_i + (  (val_f - val_i) * fast_pow(tau + eps10, exponent));
        }
    }
};

class Timing {
  public:
    Timing() : current_val(0.0) {}

    void set_timing(const double & t_absolute) {
        double t_i = 0.0, t_f = 0.0;
        int i = 0;
        for (; i < timings.size(); i++) {
            TimingElement & te = timings[i];
            t_f = t_i + te.dt;

            if (t_absolute <= t_f) {
                i++;
                break;
            }
        }
        i--;
        if (i < 0) throw std::runtime_error("There are no timing elements!");

        current_val = timings[i].getValue(t_absolute);
    }

    double current_val;
    std::vector<TimingElement> timings;
};

#endif // TIMING_H
