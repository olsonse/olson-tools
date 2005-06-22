
#include <iostream>
#include <fstream>

#include <olson-tools/BTrapDistro.h>
#include <olson-tools/physical.h>
#include <olson-tools/InvCylindricalDist.h>

using namespace physical::units;
using namespace physical::constants;

const double I = 300;

    /** Guide wires without the knee. */
    BField::ThinCurrentElement wires[] = {
        /* source for wire 2. */
        BField::ThinCurrentElement(-0.0031515, 0.0,  2.0, -0.0014693, 0.0,-4.0,I),

        /* source for wire 1. */
        BField::ThinCurrentElement( 0.0031515, 0.0,  2.0,  0.0014693, 0.0,-4.0,I),

        /* end marker. */
        BField::ThinCurrentElement(0,0,0,0,0,0,0)
    };


inline void addwires(BField::Args & args) {
    for (int i = 0; fabs(wires[i].I) > 0; args.currents.push_back(wires[i++]));
}

int main() {
    /* get the magnetic fields ready. */
    BField::Args args;
    addwires(args);
    args.rcut = 1e-10;
    args.delta = 10e-6;
    args.bg[X] = 0;
    args.bg[Y] = 0;
    args.bg[Z] = 1.0*Gauss;
    args.mass = 87.0*amu;
    args.gravity[X] = 0;
    args.gravity[Y] = -gravity;
    args.gravity[Z] = 0;

    InvCylindricalDist<BField::BTrapDistribution2D,FlatDistribution,BField::BTrap2DInit>  id;

    //id.range[RHO].min = 0;
    id.range[RHO].max = 800*microns;
    //id.range[PHI].min = 0;
    //id.range[PHI].max = 2.0*pi;
    id.nbins[RHO] = 500;
    id.nbins[PHI] = 200;

    id.range[Z].min = 0;
    id.range[Z].max = 0;
    id.nbins[Z] = 10;
    id.init.T_perp = 500*uK;
    id.init.bargs = &args;
    id.indx_long = Z;
    Vector<double,3> origin(V3(0, 0, 0));
    id.initialize(origin);

    int im = 10;
    std::cout << "enter max vals:  ";
    std::cin >> im;

    std::ofstream ofile ("disttest.dat");

    for(int i = 0; i < im; i++) {
        /* you must not include id.x() and id.y() on the same ofile<< line.
         * This is because you need to deterministically execute x() before
         * y().
         */
        ofile << (id.x() + origin[X]) << '\t';
        ofile << (id.y() + origin[Y]) << '\n';
    }
    ofile << std::endl;

    return 0;
}
