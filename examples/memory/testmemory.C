#include <sys/time.h>
#include <iostream>
#include <olson-tools/memory.h>

class bob {
    REDEF_NEW_DEL_H(bob);
    int v1[10000];
};

#define cache_lim 10
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


    gettimeofday(&ti,0x0);
    for ( int i = 0; i < 100000; i++ ) {
	for (int j = 0; j < test_size; ba[j++] = ::new bob);
	for (int j = 0; j < test_size; ::delete ba[j++]);
    }
    gettimeofday(&tf,0x0);
    borrow(ti,tf);

    std::cout << "unoverloaded time = "
    	      << (tf.tv_sec - ti.tv_sec)
	      << "s, "
	      << (tf.tv_usec - ti.tv_usec)
	      << "us" << std::endl;

    gettimeofday(&ti,0x0);
    for ( int i = 0; i < 100000; i++ ) {
	for (int j = 0; j < test_size; ba[j++] = new bob);
	for (int j = 0; j < test_size; delete ba[j++]);
    }
    gettimeofday(&tf,0x0);
    borrow(ti,tf);

    std::cout << "overloaded time = "
    	      << (tf.tv_sec - ti.tv_sec)
	      << "s, "
	      << (tf.tv_usec - ti.tv_usec)
	      << "us" << std::endl;
    return 0;
}

