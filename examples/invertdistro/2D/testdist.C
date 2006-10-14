
#include <iostream>
#include <fstream>

#include <olson-tools/BTrapDistro.h>
#include <olson-tools/physical.h>
#include <olson-tools/InvCylindricalDist.h>
#include <olson-tools/strutil.h>

using namespace physical::units;
using namespace physical::constants;

const double I = 300;

    /** Guide wires without the knee. */
    BField::ThinCurrentElement wires[] = {
        /* source for wire 2. */
        BField::ThinCurrentElement(-0.0032567, 0.0,  2.0, -0.0012567, 0.0,-4.0,I),

        /* source for wire 1. */
        BField::ThinCurrentElement( 0.0032567, 0.0,  2.0,  0.0012567, 0.0,-4.0,I),

        /* end marker. */
        BField::ThinCurrentElement(0,0,0,0,0,0,0)
    };


template <class ThinWireBSrc>
inline void addwires(ThinWireBSrc & bsrc) {
    for (int i = 0; fabs(wires[i].I) > 0; bsrc.currents.push_back(wires[i++]));
}

typedef BField::BCalcs<
    BField::AddSrc<
        BField::ThinWireSrc,
        BField::BgSrc
    >
> BFieldSrc;


typedef InvCylindricalDist<
            BField::BTrapDistribution2D<BFieldSrc>,
            FlatDistribution,
            1u,
            BField::BTrap2DInit<BFieldSrc>
> P_xyz;

int main() {
    /* get the magnetic fields ready. */
    BFieldSrc bsrc;
    addwires(bsrc);
    bsrc.delta = 1e-6;
    bsrc.bg[X] = 0;
    bsrc.bg[Y] = 0;
    bsrc.bg[Z] = 1.0*Gauss;
    bsrc.mass = 87.0*amu;
    bsrc.gravity = 0.0;

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
    std::cout << "enter max vals:  ";
    std::cin >> im;

    std::ofstream ofile ("disttest.dat");

    for(int i = 0; i < im; i++) {
        /* you must not include pos_distrib.x() and pos_distrib.y() on the same ofile<< line.
         * This is because you need to deterministically execute x() before
         * y().
         */
        ofile << (pos_distrib.x() + origin[X]) << '\t'
              << (pos_distrib.y() + origin[Y]) << '\t'
              << (pos_distrib.z() + origin[Y]) << '\n';
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

