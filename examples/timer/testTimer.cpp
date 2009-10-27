#include <olson-tools/Timer.h>

#include <iostream>

#include <cmath>

int main() {
  double r = 0.0;
  olson_tools::Timer t;
  t.wall_time_label = "s";
  t.cpu_time_label = "s (cpu)";
  t.start();
  for (double i = 0; i< 1.0; i+= 3e-8) {
    double b = std::log(i);
    r = b;
  }
  t.stop();
  std::cout << "        clock\tcpu\n"
               "Timer:  " << t << std::endl;
  // Printing the value so the optimizer to remove the log() calculation.
  std::cout << "Dummy variable 'r' was left at value '"<< r << "'\n"
            << std::flush;
  return 0;
}
