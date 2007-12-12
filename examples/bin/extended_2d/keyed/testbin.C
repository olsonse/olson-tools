#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <olson-tools/random/random.h>
#include <olson-tools/Distribution.h>
#include <olson-tools/KeyedBin.h>
#include <olson-tools/GenericBinExtender.h>

using namespace physical::constants;

/** A flat distribution for use.
*/
typedef struct {
    /** Return 0.5.
     */
    inline double distrib (const double & x) const {
        return x == 0 ? 2.0 : (1.0 + sin(x) / x);
    }
} SincDistribution;

typedef GenericBinExtender<double,KeyedBin<double,double,3,100>,100> BinType;

int main() {
    BinType bin(-10.0,10.0, -10.0,10.0);

    int iter = 0;
    std::cout << "Enter the nubmer of samples:  "
              << std::flush;
    std::cin >> iter;
    if (iter == 0) return EXIT_FAILURE;
    std::cout << iter << " samples requested." << std::endl;

    SincDistribution sinc;

    Distribution distro(sinc, -10.0, 10.0, 1000);
    for (int i = 0; i < iter; i++) {
        Vector<double,3> v(VInit,distro(),distro(),distro());
        bin.bin(distro(), distro(), v);
    }

    std::ofstream outf("bin.dat");
    bin.print(outf,"");
    outf.close();
    return 0;
}

