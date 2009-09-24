#include <vector>
#include <float.h>
#include <fstream>

#include <physical/physical.h>
#include <olson-tools/Vector.h>
#include <olson-tools/Fields.h>
#include <olson-tools/Forces.h>
#include <olson-tools/ScaleField.h>
#include <olson-tools/ScaleForce.h>
#include <olson-tools/timing/Timing.h>
#include <olson-tools/timing/Printer.h>
#include <olson-tools/timing/element/Exponential.h>

using namespace physical::units;
using olson_tools::BaseField;
using olson_tools::ScaleField;
using olson_tools::ScaleForce;
using olson_tools::BgField;
using olson_tools::Gravity;
using olson_tools::Vector;
using olson_tools::V3;
namespace timing = olson_tools::timing;
using olson_tools::timing::element::Base;
using olson_tools::timing::element::Exponential;
using olson_tools::timing::Printer;

typedef ScaleField< BgField<double> > ScaledScalarField;
typedef ScaleField< BgField< Vector<double,3> > > ScaledVectorField;
typedef ScaleForce< Gravity > ScaledGravity;


int main() {
    double t_max = 15.*ms;
    double dt    = 0.1*ms;

    ScaledGravity gravity;
    ScaledScalarField sfield;
    ScaledVectorField vfield;

    std::vector<timing::element::Base*> gtimings;
    gtimings.push_back(new timing::element::Exponential(3.*ms, 1.0, 0.0, 0.0));
    gtimings.push_back(new timing::element::Exponential(3.*ms, 1.0, 0.0, 1.0));
    gtimings.push_back(new timing::element::Exponential(3.*ms, 1.0, 0.0, 1.0));
    gtimings.push_back(new timing::element::Exponential(3.*ms, 3.0, 1.0, 0.0));
    gtimings.push_back(new timing::element::Exponential(3.*ms, 1.0, 0.0, 1.0));
    gravity.timing.timings = gtimings;

    sfield.bg = 1.0;
    std::vector<timing::element::Base*> stimings;
    stimings.push_back(new timing::element::Exponential(2.*ms, 1.0, 0.0, 0.0));
    stimings.push_back(new timing::element::Exponential(4.*ms, 1.0, 0.0, 5.0));
    stimings.push_back(new timing::element::Exponential(1.*ms, 1.0, 0.0, 8.0));
    stimings.push_back(new timing::element::Exponential(5.*ms, 3.0, 3.0, 0.0));
    stimings.push_back(new timing::element::Exponential(3.*ms, 1.0, 0.0, 1.0));
    sfield.timing.timings = stimings;

    vfield.bg = V3(1.0,-1.0,-.5);
    std::vector<timing::element::Base*> vtimings;
    vtimings.push_back(new timing::element::Exponential(4.*ms, 1.0, 0.0, 0.0));
    vtimings.push_back(new timing::element::Exponential(2.*ms, 1.0, 0.0, 3.0));
    vtimings.push_back(new timing::element::Exponential(5.*ms, 1.0, 0.0, 7.0));
    vtimings.push_back(new timing::element::Exponential(2.*ms, 3.0, 7.0, 0.0));
    vtimings.push_back(new timing::element::Exponential(2.*ms, 1.0, 0.0, 1.0));
    vfield.timing.timings = vtimings;


    std::ofstream gout("gravity-force.dat");
    std::ofstream sout("scalar-field.dat");
    std::ofstream vout("vector-field.dat");
    Vector<double,3> r(0.0);

    for (double t = 0.0; t <= t_max ; t+=dt) {
        gravity.timing.set_time(t);
        sfield.timing.set_time(t);
        vfield.timing.set_time(t);
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

    timing::Printer tp;
    tp.timers.push_back(&vfield.timing);
    tp.timers.push_back(&sfield.timing);
    tp.timers.push_back(&gravity.timing);
    tp.print("timing.dat", 0.0, dt, t_max);
}

