
#define _GNU_SOURCE 1

#if defined(__amd64__) || defined(__i386__)
#  include <fenv.h>
#elif defined(__ppc__)
#  define feenableexcept(bits)                                  \
    asm (                                                       \
    "mtfsfi   6,15              # enables all floating-point exceptions\n\t"  \
    );
#endif


#  include <signal.h>
#  include <stdio.h>
#  include <unistd.h>

void fpesig(int sig){
    fprintf(stderr,
           "A math exception was trapped\n"
           "Attach to the debugger and find out where/why.\n"
           "This function will pause here until otherwise interrupted.\n"
          );

    int exit = 0;
    while (!exit) {
        sleep(1);
    }
}

static int __attribute__ ((constructor)) trapfpe () {
    /* Enable some exceptions.  At startup all
     * exceptions are masked.  */

    feenableexcept (FE_INVALID | FE_DIVBYZERO |FE_UNDERFLOW |FE_OVERFLOW);
    signal(SIGFPE,fpesig);
    return 1;
}
