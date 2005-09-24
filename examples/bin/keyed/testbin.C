#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <olson-tools/random.h>
#include <olson-tools/Distribution.h>
#include <olson-tools/KeyedBin.h>

using namespace physical::constants;

/** A flat distribution for use.
*/
typedef struct {
    /** Return 0.5.
     */
    inline double distrib (const double & x) const {
        return 0.5;
    }
} FlatDistribution;

int main() {
    KeyedBin<double,double,3,201> bin(-0.5,0.5);

    Distribution distro(FlatDistribution(), -0.5, 0.5, 1000);
    for (int i = 0; i < 1000000; i++) {
        double x = distro();
        Vector<double,3> v(Vector<double,3>::list::dummy(), log(fabs(x)), exp(x), pow(2.0,x));
        bin.bin(distro(),v);
    }

    std::ofstream outf("bin.dat");
    bin.print(outf,"");
    outf.close();
    return 0;
}

