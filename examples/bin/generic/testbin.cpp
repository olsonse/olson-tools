#include <olson-tools/random/random.h>
#include <olson-tools/distribution/Inverter.h>
#include <olson-tools/distribution/Uniform.h>
#include <olson-tools/GenericBin.h>

#include <physical/physical.h>

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>

using namespace physical::constants;

int main() {
  olson_tools::GenericBin<double,201> bin(-0.5,0.5);

  int iter = 0;
  std::cout << "Enter the number of samples:  "
            << std::flush;
  std::cin >> iter;
  if (iter == 0) return EXIT_FAILURE;
  std::cout << iter << " samples requested." << std::endl;

  namespace dist = olson_tools::distribution;

  dist::Inverter distro(dist::Uniform(), -0.5, 0.5, 1000);
  for (int i = 0; i < iter; i++) {
    bin.bin(distro());
  }

  std::ofstream outf("bin.dat");
  bin.print(outf,"");
  outf.close();
  return 0;
}

