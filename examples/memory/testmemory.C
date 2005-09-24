#include <sys/time.h>
#include <iostream>
#include <olson-tools/memory.h>
#include <sys/times.h>
#include <unistd.h>

static double seconds_per_clock_tick = 1.0 / sysconf(_SC_CLK_TCK);

class bob {
    REDEF_NEW_DEL_H(bob);
    int v1[100];
};

#define cache_lim 1000000
#define test_size cache_lim

REDEF_NEW_DEL_C(bob,cache_lim);

inline void borrow (struct timeval & ti, struct timeval & tf){
    if (tf.tv_usec < ti.tv_usec) {
	tf.tv_sec--;
	tf.tv_usec += 1000000;
    }
}

int main() {
    struct timeval ti = {0,0},  tf = {0,0};
    bob * ba[test_size];

    struct tms tti, ttf;

    times(&tti);
    gettimeofday(&ti,0x0);
    for ( int i = 0; i < 10; i++ ) {
	for (int j = 0; j < test_size; ba[j++] = ::new bob);
	for (int j = 0; j < test_size; ::delete ba[j++]);
    }
    gettimeofday(&tf,0x0);
    times(&ttf);
    borrow(ti,tf);

    double cycle_cpu_time = 
         ( ( (ttf.tms_utime + ttf.tms_stime) - (tti.tms_utime + tti.tms_stime) )
           * seconds_per_clock_tick
         );

    std::cout << "unoverloaded time = "
    	      << (tf.tv_sec - ti.tv_sec)
	      << "s, "
	      << (tf.tv_usec - ti.tv_usec)
	      << "us"
              << "  or  " << cycle_cpu_time << 's' << std::endl;

    times(&tti);
    gettimeofday(&ti,0x0);
    for ( int i = 0; i < 10; i++ ) {
	for (int j = 0; j < test_size; ba[j++] = new bob);
	for (int j = 0; j < test_size; delete ba[j++]);
    }
    gettimeofday(&tf,0x0);
    times(&ttf);
    borrow(ti,tf);

    cycle_cpu_time = 
         ( ( (ttf.tms_utime + ttf.tms_stime) - (tti.tms_utime + tti.tms_stime) )
           * seconds_per_clock_tick
         );

    std::cout << "overloaded time = "
    	      << (tf.tv_sec - ti.tv_sec)
	      << "s, "
	      << (tf.tv_usec - ti.tv_usec)
	      << "us"
              << "  or  " << cycle_cpu_time << 's' << std::endl;
    return 0;
}

