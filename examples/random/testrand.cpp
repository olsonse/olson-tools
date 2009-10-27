#include <olson-tools/random/random.h>
#include <olson-tools/Distribution.h>
#include <olson-tools/Timer.h>

#include <iostream>
#include <fstream>

#include <stdint.h>

using std::cin;
using std::cout;
using std::flush;
using std::ofstream;
extern int errno;
using olson_tools::random::gauss_deviate;

int main() {
    MTRand r;
    double sigma = 1.0;
    int N = 100;

    cout << "enter sigma value:  " << flush;
    cin >> sigma;
    cout << "enter total iterations:  " << flush;
    cin >> N;

    ofstream outgd("test_gauss_deviate.dat");

    for ( int i = 0; i < N; i++) {
      outgd << gauss_deviate(sigma) << '\n';
    }

    outgd.flush();
    outgd.close();

    ofstream outrn("test_mtrn.dat");

    for ( int i = 0; i < N; i++) {
      outrn << r.randNorm(0,sigma) << '\n';
    }

    outrn.flush();
    outrn.close();


    olson_tools::Distribution d(
      olson_tools::GaussianDistrib(1./(2.*sigma*sigma)),
      -5*sigma, 5*sigma, 10000
    );

    ofstream outinv("test_invgaussian.dat");

    for ( int i = 0; i < N; i++) {
      outinv << d() << '\n';
    }

    outinv.flush();
    outinv.close();


    /* now we do a little timing test between gauss_deviate and
     * MTRand::randNorm().
     */

    double dev = 0;

#define L 10000000

    olson_tools::Timer timer;
    timer.cpu_time_label = "s (cpu)";
    timer.wall_time_label = "s";

    timer.start();
    for (int64_t i = 0; i < L; i++) {
        dev = 0.0 + gauss_deviate(sigma);
    }
    timer.stop();


    std::cout << "gauss_deviate finished in " << timer << std::endl;

    timer.start();
    for (int64_t i = 0; i < L; i++) {
        dev = r.randNorm(0,sigma);
    }
    timer.stop();

    std::cout << "MTRand::randNorm() finished in " << timer << std::endl;

    timer.start();
    for (int64_t i = 0; i < L; i++) {
        dev = d();
    }
    timer.stop();

    std::cout << "Distribution inversion (Gaussian) finished in " << timer << std::endl;


}
