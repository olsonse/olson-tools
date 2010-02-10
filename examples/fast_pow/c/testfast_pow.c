
#ifndef USE_SPENCERS_FAST_POW
int main() { return 0; };
#else


#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <math.h>
#include <olson-tools/power.h>


double difftms(struct tms tf, struct tms ti) {
    double seconds_per_clock_tick = 1.0 / sysconf(_SC_CLK_TCK);
    return  
        ( (tf.tms_utime + tf.tms_stime) - (ti.tms_utime + ti.tms_stime) )
        * seconds_per_clock_tick ;
}

int main() {
    double x = 0.0l, y = -100.0l;

#if 0
    printf("fast_pow(%g,%g) = %g\n", x,y,fast_pow(x,y));
    printf("pow(%g,%g) = %g\n", x,y,pow(x,y));
#else
    struct tms ti, tf;
    double i =0, k=0, mink = 100, maxk = 110, min = -150, max = 150;

//#  if 1
    double total_err = 0.0, err = 0.0, t1,t2;
    for (k = mink; k < maxk; k+=0.01) {
	if (k == 0 && i <= 0) continue;
	for (i = min; i < max; i+=0.01) {
	    t1 = pow (k,i);
	    t2 = fast_pow (k, i);
	    total_err += err = fabs((t1 - t2)/t1);
	    //printf("%g\t%g\t%g\t%g\n", i, err, t1, t2);
	}
    }

    fprintf(stderr,"total error sum((pow(..) - fast_pow(..))/pow(..)) is %g\n", total_err);
//#  else
    double sum = 0.0;

    times(&ti);
    for (k = mink; k < maxk; k+=0.01) {
	if (k == 0 && i <= 0) continue;
	for (i = min; i < max; i+=0.01) {
	    t2 = fast_pow (k, i);
            sum += t2;
	}
    }
    times(&tf);

    double duration = difftms(tf,ti);

    printf ("fast_pow : \n");
    printf ("sum is %.3f\n", sum);
    printf ("Elapsed time = %.3f s\n", duration);


    sum = 0.0;

    times(&ti);
    for (k = mink; k < maxk; k+=0.01) {
	if (k == 0 && i <= 0) continue;
	for (i = min; i < max; i+=0.01) {
	    t1 = pow (k, i);
            sum += t1;
	}
    }
    times(&tf);

    duration = difftms(tf,ti);

    printf ("pow : \n");
    printf ("sum is %.3f\n", sum);
    printf ("Elapsed time = %.3f s\n", duration);
//#  endif
#endif
    fflush(stdout);

    return 0;
}

#endif // USE_SPENCERS_FAST_POW
