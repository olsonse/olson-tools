#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <olson-tools/random/random.h>
#include <olson-tools/Distribution.h>
#include <olson-tools/GenericBin.h>
#include <olson-tools/GenericBinExtender.h>

using namespace physical::constants;

/** A flat distribution for use.
*/
typedef struct {
    /** Return 0.5.
     */
    inline double distrib (const double & x) const {
        return x == 0.0 ? 2. : (1. + sin(x) / x);
    }
} SincDistribution;

typedef olson_tools::GenericBinExtender<double,olson_tools::GenericBin<double,100>,100> BinType;

int main() {
    BinType bin(-10.0,10.0, -10.0,10.0);

    int iter = 0;
    std::cout << "Enter the nubmer of samples:  "
              << std::flush;
    std::cin >> iter;
    if (iter == 0) return EXIT_FAILURE;
    std::cout << iter << " samples requested." << std::endl;

    SincDistribution sinc;

    olson_tools::Distribution distro(sinc, -10.0, 10.0, 1000);
    for (int i = 0; i < iter; i++) {
        bin.bin(distro(), distro());
    }

    std::ofstream outf("bin.dat");
    bin.print(outf,"");
    outf.close();
    return 0;
}

