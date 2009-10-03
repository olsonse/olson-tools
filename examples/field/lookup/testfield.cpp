#define DX_TIMED 0.3*um
#define DX_TEST 6*um

#include <sys/times.h>
#include <unistd.h>

#include <olson-tools/field-lookup.h>
#include "common.h"

using olson_tools::Vector;
using olson_tools::V3;
using namespace olson_tools::indices;

template <class BSrc1, class BSrc2>
double testfield(std::ostream & output,
                 const BSrc1 & bsrc1,
                 const BSrc2 & bsrc2,
                 const Vector<double,3> & xi,
                 const Vector<double,3> & xf,
                 const Vector<double,3> & dx );

template <class BSrc>
double timefield(const BSrc & bsrc,
                 const Vector<double,3> & xi,
                 const Vector<double,3> & xf,
                 const Vector<double,3> & dx );

const Vector<double,3> X_MIN   = V3(-30.0*um,          -30.0*um,         -30.*um );
const Vector<double,3> X_MAX   = V3( 30.0*um + 1e-12,   30.0*um + 1e-12,  30.*um + 1e-12 );
const Vector<double,3> dx_timed= V3(DX_TIMED, DX_TIMED, DX_TIMED);
const Vector<double,3> dx_test = V3(DX_TEST, DX_TEST, DX_TEST);

static const double seconds_per_clock_tick = 1.0 / sysconf(_SC_CLK_TCK);

int main() {
    /* Dynamic Field Calc */
    BFieldForce bsrc;
    addwires(bsrc);
    bsrc.mass = mass;
    bsrc.Gravity::bg[Z] = -physical::unit::gravity;
    bsrc.delta = delta_B;


    /* Static Field Calc (interpolated from lookup table) */
    olson_tools::ForceLookup<> flookup;
    flookup.readindata(FIELD_FILENAME);


    std::ofstream errout(ERR_FILE);
    errout.precision(8);
    errout << std::scientific;
    /* print header in file */
    errout << "# x[3] error a_calc[3] a_lookup[3] Verr V_calc V_lookup\n";
    testfield(errout, bsrc, flookup, X_MIN, X_MAX, dx_test);
    errout.close();
    std::cout << "Finished field test.\nDoing timed test" << std::endl;

    std::cout
            << "BSrc Time    : "
            << timefield(bsrc,X_MIN, X_MAX, dx_timed) << " s" << std::endl;

    std::cout
            << "flookup Time : "
            << timefield(flookup,X_MIN, X_MAX, dx_timed) << " s" << std::endl;

    return 0;
}

template <class BSrc1, class BSrc2>
double testfield(std::ostream & output,
                 const BSrc1 & bsrc1,
                 const BSrc2 & btst,
                 const Vector<double,3> & xi,
                 const Vector<double,3> & xf,
                 const Vector<double,3> & dx ) {
    double err = 0.0;
    for (Vector<double,3> x = xi; x[Z] <= xf[Z]; x[Z] += dx[Z]) {
        for (x[X] = xi[X]; x[X] <= xf[X]; x[X]+= dx[X]) {
            for (x[Y] = xi[Y]; x[Y] <= xf[Y]; x[Y] += dx[Y]) {
                Vector<double,3> a1, a2, error;
                bsrc1.accel(a1, x);
                btst.accel(a2, x);

                double V1,V2,verr;
                V1 = bsrc1.potential(x);
                V2 = btst.potential(x);
                verr = 1.0 - V2/(V1 + 1e-10);

                error = a2; error.compDiv(a1 + 1e-10); error -= 1.0;
                err += sum(error);

                output << x << error << a1 << a2
                       << verr << '\t' << V1 << '\t' << V2 << '\n';
            }
        }/*for */

        output << '\n';
    }
    return err;
}


template <class BSrc>
double timefield(const BSrc & bsrc,
                 const Vector<double,3> & xi,
                 const Vector<double,3> & xf,
                 const Vector<double,3> & dx ) {

    struct tms ti, tf;
    times(&ti);

    for (Vector<double,3> x = xi; x[Z] <= xf[Z]; x[Z] += dx[Z]) {
        for (x[X] = xi[X]; x[X] <= xf[X]; x[X]+= dx[X]) {
            for (x[Y] = xi[Y]; x[Y] <= xf[Y]; x[Y] += dx[Y]) {
                Vector<double,3> a;
                bsrc.accel(a, x);
            }
        }/*for */
    }

    times(&tf);

    double cpu_time = 
                     ( ( (tf.tms_utime + tf.tms_stime) - (ti.tms_utime + ti.tms_stime) )
                       * seconds_per_clock_tick
                     );
    return cpu_time;
}
