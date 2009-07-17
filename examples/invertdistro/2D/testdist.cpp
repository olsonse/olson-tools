
#include <iostream>
#include <fstream>

#include <olson-tools/BTrapDistro.h>
#include <physical/physical.h>
#include <olson-tools/InvCylindricalDist.h>
#include <olson-tools/strutil.h>

using namespace physical::units;
using namespace physical::constants::si;
using namespace olson_tools::indices;

using olson_tools::SQR;
using olson_tools::AddField;
using olson_tools::AddForce;
using olson_tools::BgField;
using olson_tools::Vector;
using olson_tools::Gravity;
using olson_tools::InvCylindricalDist;
using olson_tools::FlatDistribution;
using olson_tools::BField::BCalcs;
using olson_tools::BField::ThinWireSrc;
using olson_tools::BField::ThinCurrentElement;
using olson_tools::BField::BTrapDistribution2D;
using olson_tools::BField::BTrap2DInit;
using olson_tools::to_string;

const double I = 300;

    /** Guide wires without the knee. */
    ThinCurrentElement wires[] = {
        /* source for wire 2. */
        ThinCurrentElement(-0.0032567, 0.0,  2.0, -0.0012567, 0.0,-4.0,I),

        /* source for wire 1. */
        ThinCurrentElement( 0.0032567, 0.0,  2.0,  0.0012567, 0.0,-4.0,I),

        /* end marker. */
        ThinCurrentElement(0,0,0,0,0,0,0)
    };


template <class ThinWireBSrc>
inline void addwires(ThinWireBSrc & bsrc) {
    for (int i = 0; fabs(wires[i].I) > 0; bsrc.currents.push_back(wires[i++]));
}

typedef BCalcs<
    AddField<
        ThinWireSrc,
        BgField< Vector<double,3> >
    >
> BForce;

typedef AddForce< BForce, Gravity > BFieldSrc;


typedef InvCylindricalDist<
            BTrapDistribution2D<BForce>,
            FlatDistribution,
            1u,
            BTrap2DInit<BForce>
> P_xyz;

int main() {
    /* get the magnetic fields ready. */
    BFieldSrc bsrc;
    addwires(bsrc);
    bsrc.delta = 1e-6;
    bsrc.BForce::bg[X] = 0;
    bsrc.BForce::bg[Y] = 0;
    bsrc.BForce::bg[Z] = 1.0*Gauss;
    bsrc.mass = 87.0*amu;
    bsrc.Gravity::bg = 0.0;

    P_xyz  pos_distrib;
    const double temperature = 150. * uK;

    pos_distrib.range[RHO].max = 800*microns;
    pos_distrib.range[Z].min =  0.0;
    pos_distrib.range[Z].max = 0.0; /* = 1.*mm; */

    pos_distrib.nbins[RHO] = 500;
    pos_distrib.nbins[PHI] = 200;
    pos_distrib.nbins[Z] = 10;

    pos_distrib.init.T_perp = temperature;
    pos_distrib.indx_long = Z;

    pos_distrib.init.bsrc = &bsrc;

    Vector<double,3> origin(V3(0, 0, 0));
    pos_distrib.initialize(origin);

    int im = 10;
    std::cout << "Enter the nubmer of samples:  " << std::flush;
    std::cin >> im;

    std::ofstream ofile ("disttest.dat");

    for(int i = 0; i < im; i++) {
        /* you must not include pos_distrib.x() on the same ofile<< line as
         * for pos_distrib.y() or pos_distrib.z().  This is because you need
         * to deterministically execute x() before y() and z().
         */
        ofile << (pos_distrib.x() + origin[X]) << '\t';
        ofile << (pos_distrib.y() + origin[Y]) << '\t' 
              << (pos_distrib.z() + origin[Z]) << '\n';
    }

    ofile << std::endl;


    /* now we'll compute the binned distribution function for the x-direction
     * just so we can compare the binned values of our distribution function.
     */
    {
        std::vector< Vector<double,2> > potential;
        double multf = 1 / (K_B * temperature);
        double potential_integral = 0;
        double last_potI = 0;

        double x[3];

        const double X_INIT[3]  = {-0.8*mm,     -0.8*mm,      0.0};
        const double X_MAX[3]   = { 0.800001*mm, 0.800001*mm, X_INIT[Z]};

        const double X_INCR = 1e-6;

        for (memcpy(x, X_INIT, 3*sizeof(double)); x[X] <= X_MAX[X]; x[X]+= X_INCR) {
            double potI = 0;
            for (x[Y] = X_INIT[Y]; x[Y] <= X_MAX[Y]; x[Y] += X_INCR) {
                for (x[Z] = X_INIT[Z]; x[Z] <= X_MAX[Z]; x[Z] += X_INCR) {
                    potI += exp(-bsrc.potential(x) * multf);
                }
            }
            /* simple integration the area under the curve, scaled to mircons*I. */
            potential_integral += 0.5 * 1e6 * X_INCR * (last_potI + potI);
            last_potI = potI;

            Vector<double,2> v(VInit,x[X],potI);
            potential.push_back(v);
        }

        std::ofstream potout(("disttest" + to_string(temperature/uK) + "uK.dat").c_str());

        potout << "# " << to_string(temperature/uK) << " uK\n"
                  "# Guide Current:  " << I << " A\n"
                  "# X_INIT[3] = { " << X_INIT[0] << ','
                                     << X_INIT[1] << ','
                                     << X_INIT[2] << "}\n"
                  "# X_MAX[3] = { "  << X_MAX[0] << ','
                                     << X_MAX[1] << ','
                                     << X_MAX[2] << "}\n"
                  "# X_INCR = " << X_INCR << "\n";

        /* and now renormalize the distribution to unit area. */
        for( std::vector< Vector<double,2> >::iterator i = potential.begin();
             i < potential.end();
             i++
           ) {
            Vector<double,2> & val = *i;
            /* renormalize. */
            val[1] /= potential_integral;
            potout << val << '\n';
        }

        potout.close();
    }

    return 0;
}

