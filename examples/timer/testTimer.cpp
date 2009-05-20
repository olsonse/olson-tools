#include <iostream>
#include <math.h>
#include <olson-tools/Timer.h>

int main() {
    double r = 0.0;
    olson_tools::Timer t;
    t.wall_time_label = "s";
    t.cpu_time_label = "s (cpu)";
    t.start();
    for (double i = 0; i< 1.0; i+= 3e-8) {
        double b = log(i);
        r = b;
    }
    t.stop();
    std::cout << "        clock\tcpu\n"
                 "Timer:  " << t << std::endl;
    return int(0*r);
}
