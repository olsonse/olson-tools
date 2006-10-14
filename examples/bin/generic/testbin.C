#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <olson-tools/random/random.h>
#include <olson-tools/Distribution.h>
#include <olson-tools/GenericBin.h>

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
    GenericBin<double,201> bin(-0.5,0.5);

    int iter = 0;
    std::cout << "Enter the nubmer of samples:  "
              << std::flush;
    std::cin >> iter;
    if (iter == 0) return EXIT_FAILURE;
    std::cout << iter << " samples requested." << std::endl;

    Distribution distro(FlatDistribution(), -0.5, 0.5, 1000);
    for (int i = 0; i < iter; i++) {
        bin.bin(distro());
    }

    std::ofstream outf("bin.dat");
    bin.print(outf,"");
    outf.close();
    return 0;
}

