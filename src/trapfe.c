
#define _GNU_SOURCE 1

#if defined(__x86_64__) || defined(__amd64__) || defined(__i386__) || defined(__sun__)
#  include <fenv.h>
#elif defined(__ppc__)
#  define feenableexcept(bits)                                  \
    asm (                                                       \
    "mtfsfi   6,15              # enables all floating-point exceptions\n\t"  \
    );
#endif


#if !defined(__sun__)
#  include <signal.h>
#endif
#  include <stdio.h>
#  include <unistd.h>

void fpesig(
            int sig
#if defined(__sun__)
            , fex_info_t *info
#endif
){
    int stop_now = 0;
    fprintf(stderr,
           "A math exception was trapped\n"
           "Attach to the debugger and find out where/why.\n"
           "This function will pause here until otherwise interrupted.\n"
          );

    while (!stop_now) {
        sleep(1);
    }
}

static int __attribute__ ((constructor)) trapfpe () {
    /* Enable some exceptions.  At startup all
     * exceptions are masked.  */

#if defined(__sun__)
    (void) fex_set_handling(FEX_COMMON, FEX_CUSTOM, fpesig);
#else
    feenableexcept (FE_INVALID | FE_DIVBYZERO |FE_UNDERFLOW |FE_OVERFLOW);
    signal(SIGFPE,fpesig);
#endif
    return 1;
}
