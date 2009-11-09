#include <olson-tools/random/random.h>
#include <olson-tools/distribution/Inverter.h>
#include <olson-tools/distribution/Uniform.h>
#include <olson-tools/KeyedBin.h>

#include <physical/physical.h>

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>

using namespace physical::constants;

int main() {
  olson_tools::KeyedBin<double,double,3,201> bin(-0.5,0.5);

  int iter = 0;
  std::cout << "Enter the nubmer of samples:  "
            << std::flush;
  std::cin >> iter;
  if (iter == 0) return EXIT_FAILURE;
  std::cout << iter << " samples requested." << std::endl;

  namespace dist = olson_tools::distribution;

  dist::Inverter distro(dist::Uniform(), -0.5, 0.5, 1000);
  for (int i = 0; i < iter; i++) {
    using olson_tools::Vector;
    using olson_tools::V3;
    double x = distro();
    Vector<double,3> v =
      V3( std::log(std::abs(x)), std::exp(x), std::pow(2.0,x) );
    bin.bin(distro(),v);
  }

  std::ofstream outf("bin.dat");
  bin.print(outf,"");
  outf.close();
  return 0;
}

