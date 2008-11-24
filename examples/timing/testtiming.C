#include <vector>
#include <float.h>
#include <fstream>

#include <olson-tools/physical/physical.h>
#include <olson-tools/Vector.h>
#include <olson-tools/Fields.h>
#include <olson-tools/Forces.h>
#include <olson-tools/Timing.h>
#include <olson-tools/ScaleField.h>
#include <olson-tools/ScaleForce.h>

using namespace physical::units;
using olson_tools::BaseField;
using olson_tools::ScaleField;
using olson_tools::ScaleForce;
using olson_tools::BgField;
using olson_tools::Gravity;
using olson_tools::Vector;
using olson_tools::TimingElement;
using olson_tools::ExpTimingElement;
using olson_tools::TimingPrinter;

typedef ScaleField< BgField<double> > ScaledScalarField;
typedef ScaleField< BgField< Vector<double,3> > > ScaledVectorField;
typedef ScaleForce< Gravity > ScaledGravity;


int main() {
    double t_max = 15.*ms;
    double dt    = 0.1*ms;

    ScaledGravity gravity;
    ScaledScalarField sfield;
    ScaledVectorField vfield;

    std::vector<TimingElement*> gtimings;
    gtimings.push_back(new ExpTimingElement(3.*ms, 1.0, 0.0, 0.0));
    gtimings.push_back(new ExpTimingElement(3.*ms, 1.0, 0.0, 1.0));
    gtimings.push_back(new ExpTimingElement(3.*ms, 1.0, 0.0, 1.0));
    gtimings.push_back(new ExpTimingElement(3.*ms, 3.0, 1.0, 0.0));
    gtimings.push_back(new ExpTimingElement(3.*ms, 1.0, 0.0, 1.0));
    gravity.timing.timings = gtimings;

    sfield.bg = 1.0;
    std::vector<TimingElement*> stimings;
    stimings.push_back(new ExpTimingElement(2.*ms, 1.0, 0.0, 0.0));
    stimings.push_back(new ExpTimingElement(4.*ms, 1.0, 0.0, 5.0));
    stimings.push_back(new ExpTimingElement(1.*ms, 1.0, 0.0, 8.0));
    stimings.push_back(new ExpTimingElement(5.*ms, 3.0, 3.0, 0.0));
    stimings.push_back(new ExpTimingElement(3.*ms, 1.0, 0.0, 1.0));
    sfield.timing.timings = stimings;

    vfield.bg = V3(1.0,-1.0,-.5);
    std::vector<TimingElement*> vtimings;
    vtimings.push_back(new ExpTimingElement(4.*ms, 1.0, 0.0, 0.0));
    vtimings.push_back(new ExpTimingElement(2.*ms, 1.0, 0.0, 3.0));
    vtimings.push_back(new ExpTimingElement(5.*ms, 1.0, 0.0, 7.0));
    vtimings.push_back(new ExpTimingElement(2.*ms, 3.0, 7.0, 0.0));
    vtimings.push_back(new ExpTimingElement(2.*ms, 1.0, 0.0, 1.0));
    vfield.timing.timings = vtimings;


    std::ofstream gout("gravity-force.dat");
    std::ofstream sout("scalar-field.dat");
    std::ofstream vout("vector-field.dat");
    Vector<double,3> r(0.0);

    for (double t = 0.0; t <= t_max ; t+=dt) {
        gravity.timing.set_timing(t);
        sfield.timing.set_timing(t);
        vfield.timing.set_timing(t);
        Vector<double,3> a;
        gravity.accel(a,r);

        gout << t << '\t'
            << a << '\n'
            << std::flush;

        sout << t << '\t'
            << sfield(r) << '\n'
            << std::flush;

        vfield(a,r);
        vout << t << '\t'
            << a << '\n'
            << std::flush;
    }

    TimingPrinter tp;
    tp.timers.push_back(&vfield.timing);
    tp.timers.push_back(&sfield.timing);
    tp.timers.push_back(&gravity.timing);
    tp.printTimings("timing.dat", 0.0, dt, t_max);
}

