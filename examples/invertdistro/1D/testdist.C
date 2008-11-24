
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <olson-tools/physical/physical.h>
#include <olson-tools/random/random.h>
#include <olson-tools/Distribution.h>
#include <olson-tools/Vector.h>
#include <olson-tools/GenericBin.h>

using namespace physical::constants;
using olson_tools::Distribution;
using olson_tools::MaxwellianDistrib2D;
using olson_tools::MaxwellianDistrib3D;
using olson_tools::GaussianDistrib;
using olson_tools::Vector;
using olson_tools::GenericBin;

/* this is how G.A. Bird gets his diffusely reflecting velocities. */
void nextvel(double p[3]) {
    static double betaw = sqrt(2.0 * K_B * 500.0 * uK / (87.0 * amu) );

    double ran = std::max(1.e-8,MTRNGrand());
    double B   = sqrt(-log(ran));

    double A   = 2.0 * pi * MTRNGrand();
    double uu  = B * sin(A) * betaw;
    double ww  = B * cos(A) * betaw;

           ran = std::max(1.e-8, MTRNGrand());
    double vv  = sqrt(-log(ran)) * betaw;

    p[0] = -vv;
    p[1] = uu;
    p[2] = ww;
}


/** SOME DISTRIBUTION FUNCTIONS. **/

/** A flat distribution for use.
*/
typedef struct {
    /** Return 0.5.
     */
    inline double distrib (const double & x) const {
        return 0.5;
    }
} FlatDistribution;


static double beta = 0.5 * (87.0 * amu) / (K_B * 500.0 * uK);
static double sigma = sqrt( 0.5 / beta);

static Distribution t2d = Distribution(MaxwellianDistrib2D(beta), 0.0, 5*sigma, 10000);

static Distribution t3d = Distribution(MaxwellianDistrib3D(beta), 0.0, 5*sigma, 10000);

static Distribution gau = Distribution(GaussianDistrib(beta), -4*sigma, 4*sigma, 10000);

static Distribution flt = Distribution(FlatDistribution(),-4*sigma, 4*sigma, 10000);

void nextvalues(Vector<double,5> & p) {
    p[0] = gauss_deviate(sigma);
    p[1] = gau();
    p[2] = t2d();
    p[3] = t3d();
    p[4] = flt();
}

typedef GenericBin<double,1000> bin;

bin bins[5] = {
    bin(-4*sigma, 4*sigma),     /* gauss_deviate */
    bin(-4*sigma, 4*sigma),     /* GaussianDistrib */
    bin(0.0, 5*sigma),          /* MaxwellianDistrib2D */
    bin(0.0, 5*sigma),          /* MaxwellianDistrib3D */
    bin(-4*sigma, 4*sigma)      /* FlatDistribution */
};

int main() {

    int iter = 0;
    std::cout << "Enter the number of samples:  " << std::flush;
    std::cin >> iter;
    if (iter == 0) return EXIT_FAILURE;
    std::cout << iter << " samples requested." << std::endl;

    for (int i = 0; i < iter; i++) {
        Vector<double,5> p;
        nextvalues(p);

        for (int j = 0; j < 5; j++) bins[j].bin(p[j]);

    }

    std::ofstream outf("dist.dat");
    for (int j = 0; j < 5; j++) {
        bins[j].print(outf,"") << "\n\n";
    }

    return 0;
}

