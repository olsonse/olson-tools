// -*- c -*-
// $Id$
/** \file
 * FPU initialization routines to arm FPU exceptions.  
 *
 * Supported processors: i386, amd64, PowerPC (MacOS X), SUN Solaris SPARC.
 *
 * Copyright 2004 Spencer Eugene Olson --- All Rights Reserved
 *
 */

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
           "Address of 'stop_now' variable:  %p\n"
           "Set stop_now to false to continue\n", &stop_now
          );

    while (!stop_now) {
        sleep(1);
    }
}

/** a simple function to clear the exception register values with the
 * debugger. */
static int __attribute__ ((used)) clearfpe() {
#if defined(__sun__)
    fprintf(stderr, "I'm not yet sure how to clear the exception register\n"
                    "on sun machines\n"
           );
#else
    feclearexcept (FE_INVALID | FE_DIVBYZERO |FE_UNDERFLOW |FE_OVERFLOW);
#endif
    return -1;
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
