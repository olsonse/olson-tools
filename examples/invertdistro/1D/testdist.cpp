
#include <olson-tools/Vector.h>
#include <olson-tools/GenericBin.h>
#include <olson-tools/random/random.h>
#include <olson-tools/distribution/Uniform.h>
#include <olson-tools/distribution/Inverter.h>
#include <olson-tools/distribution/Gaussian.h>
#include <olson-tools/distribution/Maxwellian2D.h>
#include <olson-tools/distribution/Maxwellian3D.h>

#include <physical/physical.h>

#include <iostream>
#include <fstream>

#include <cmath>
#include <cstdlib>


namespace si = physical::constants::si;
using namespace si;
using namespace physical::units;
namespace dist = olson_tools::distribution;
using olson_tools::Vector;
using olson_tools::GenericBin;
using olson_tools::random::MTRNGrand;
using olson_tools::random::gauss_deviate;

/* this is how G.A. Bird gets his diffusely reflecting velocities. */
void nextvel(double p[3]) {
  static double betaw = std::sqrt(2.0 * K_B * 500.0 * uK / (87.0 * amu) );

  double ran = std::max(1.e-8,MTRNGrand());
  double B   = std::sqrt(-log(ran));

  double A   = 2.0 * si::pi * MTRNGrand();
  double uu  = B * sin(A) * betaw;
  double ww  = B * cos(A) * betaw;

         ran = std::max(1.e-8, MTRNGrand());
  double vv  = std::sqrt(-log(ran)) * betaw;

  p[0] = -vv;
  p[1] = uu;
  p[2] = ww;
}


/** SOME DISTRIBUTION FUNCTIONS. **/

static double beta = 0.5 * (87.0 * amu) / (K_B * 500.0 * uK);
static double sigma = std::sqrt( 0.5 / beta);

static dist::Inverter t2d = dist::Inverter(dist::Maxwellian2D(beta), 0.0, 5*sigma, 10000);

static dist::Inverter t3d = dist::Inverter(dist::Maxwellian3D(beta), 0.0, 5*sigma, 10000);

static dist::Inverter gau = dist::Inverter(dist::Gaussian(beta), -4*sigma, 4*sigma, 10000);

static dist::Inverter flt = dist::Inverter(dist::Uniform(),-4*sigma, 4*sigma, 10000);

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
  bin(-4*sigma, 4*sigma),     /* Gaussian */
  bin(0.0, 5*sigma),          /* Maxwellian2D */
  bin(0.0, 5*sigma),          /* Maxwellian3D */
  bin(-4*sigma, 4*sigma)      /* Uniform */
};

int main() {
  int iter = 0;
  std::cout << "Enter the number of samples:  " << std::flush;
  std::cin >> iter;
  if (iter == 0) return EXIT_FAILURE;
  std::cout << iter << " samples requested." << std::endl;

  for ( int i = 0; i < iter; ++i ) {
    Vector<double,5> p;
    nextvalues(p);

    for ( int j = 0; j < 5; ++j )
      bins[j].bin(p[j]);

  }

  std::ofstream outf("dist.dat");
  for ( int j = 0; j < 5; ++j )
    bins[j].print(outf,"") << "\n\n";
  outf.close();

  return 0;
}

