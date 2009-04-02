// -*- c -*-
// $Id$
/** \file
 * FPU initialization routines to arm FPU exceptions.  
 *
 * Supported processors: i386, amd64, PowerPC (MacOS X), SUN Solaris SPARC.
 *
 * Supported compilers:  gcc, Intel, PGI
 *
 * Copyright 2004 Spencer Eugene Olson --- All Rights Reserved
 *
 */

/** \example trapfe/testtrapfe.C
 * This program shows a calculation that causes a floating point exception. 
 * By linking in the trapfe library and compiling with the -g option, these
 * floating point exceptions can be caught and the program paused.  This
 * allows you to load the running, yet paused, program into a debugger to
 * pin-point the location of the floating point exception.
 */

#define _GNU_SOURCE 1

extern "C" {

#if defined(__x86_64__) || defined(__amd64__) || defined(__i386__) || defined(__sun__) || defined(__ia64__)
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
#  include <sys/types.h>
#  include <unistd.h>

static int fpe_pause = 1;

void fpesig(
            int sig
#if defined(__sun__)
            , fex_info_t *info
#endif
){
    fpe_pause = 1;
    fprintf(stderr,
           "A math exception was trapped\n"
           "Attach to the debugger and find out where/why.\n"
           "This function will pause here until otherwise interrupted.\n"
           "Address of 'fpe_pause' variable:  (int*)%p\n"
           "To debug:\n"
           "1.  Attach to process (with pid = %d) in debugger\n"
           "2.  Set fpe_pause to 0 to continue\n"
           "    (e.g. in gdb:  print *((int*)%p) = 0)\n"
           "3.  Single step in debugger until fpe signal handler is exited\n"
           "    and your own code is entered.\n"
           , &fpe_pause, getpid(), &fpe_pause
    );

    while (fpe_pause) {
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
    fprintf(stderr, "TrapFPE installed\n");
    fflush(stderr);

#if defined(__sun__)
    (void) fex_set_handling(FEX_COMMON, FEX_CUSTOM, fpesig);
#else
    feenableexcept (FE_INVALID | FE_DIVBYZERO |FE_UNDERFLOW |FE_OVERFLOW);
    signal(SIGFPE,fpesig);
#endif
    return 1;
}

/* for some reason, some of the compilers (PGI, INTEL) are too dumb to know
 * what to do with some __attribute__ commands (PGI seems to ignore them all).
 * This makes sure that these functions are called.
 */
static int bob_the_builder      = trapfpe();
static int wendy_the_builder    = clearfpe();

};
