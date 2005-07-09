#include <olson-tools/random.h>
#include <iostream>
#include <fstream>
#include <sys/times.h>
#include <unistd.h>

using std::cin;
using std::cout;
using std::flush;
using std::ofstream;
extern int errno;

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

    /* now we do a little timing test between gauss_deviate and
     * MTRand::randNorm().
     */

    struct tms ti, tf;
    double dev = 0;
    const double seconds_per_clock_tick = 1.0 / sysconf(_SC_CLK_TCK);

#define L 10000000

    times(&ti);
    for (int64_t i = 0; i < L; i++) {
        dev = 0.0 + gauss_deviate(sigma);
    }
    times(&tf);


    double cycle_cpu_time = 
        ( ( (tf.tms_utime + tf.tms_stime) - (ti.tms_utime + ti.tms_stime) )
          * seconds_per_clock_tick
        );

    std::cout << "gauss_deviate gives " << cycle_cpu_time << std::endl;

    times(&ti);
    for (int64_t i = 0; i < L; i++) {
        dev = r.randNorm(0,sigma);
    }
    times(&tf);

    cycle_cpu_time = 
        ( ( (tf.tms_utime + tf.tms_stime) - (ti.tms_utime + ti.tms_stime) )
          * seconds_per_clock_tick
        );

    std::cout << "MTRand::randNorm() gives " << cycle_cpu_time << std::endl;

}
