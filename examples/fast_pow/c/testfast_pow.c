#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <olson-tools/power.h>

#define CUBE(x)  ((x)*(x)*(x))


int
main (void)
{
    double x = 0.0l, y = -100.0l;

#if 0
    printf("fast_pow(%g,%g) = %g\n", x,y,fast_pow(x,y));
    printf("pow(%g,%g) = %g\n", x,y,pow(x,y));
#else
    clock_t start, finish;
    double i =0, k=0, mink = 100, maxk = 110, min = -150, max = 150;

#  if 1
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

    fprintf(stderr,"total error is %g\n", total_err);
#  else
    double sum = 0.0;

    start = clock ();
    for (i = min; i < max; i+=0.01) {
	sum += fast_pow (x, i);
    }
    finish = clock ();

    double duration = (double) (finish - start) / CLOCKS_PER_SEC;

    printf ("sum is %.3f\n", sum);
    printf ("Elapsed time = %.3f s\n", duration);


    sum = 0.0;

    start = clock ();
    for (i = min; i < max; i+=0.01) {
	//sum += CUBE(i);
	sum += pow (x, i);
    }
    finish = clock ();

    duration = (double) (finish - start) / CLOCKS_PER_SEC;

    printf ("sum is %.3f\n", sum);
    printf ("Elapsed time = %.3f s\n", duration);
#  endif
#endif

    return 0;
}
