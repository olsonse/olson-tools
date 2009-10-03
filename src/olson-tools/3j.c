/* Name: s3j
**       Evaluates 3j symbol
**
** Author: Riccardo Gusmeroli (web.address@libero.it)
** Altered by:  Spencer Olson (olsonse@umich.edu)
**
** Notes: 
**     - defining S3J_TEST enables the compilation of a very small test suite.
**     - the maximum allowed factorial is S3J_MAX_FACT (currently 25!). 
**     - Added caching of factorials. (Spencer Olson)
**     - eliminated S3J_MAX_FACT limit on factorials. (Spencer Olson)
**
**
** This program is free software; you can redistribute it and/or  
** modify it under the terms of the GNU General Public License 
** as published by the Free Software Foundation; either version 2  
** of the License, or (at your option) any later version.
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of 
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License 
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA. */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define S3J_0		1e-10

//#define S3J_TEST

#define S3J_EQUAL(a,b)		(fabs((a)-(b))<S3J_0)
#define S3J_MAX(a,b,c,ris)	(((a)>(b)?(ris=(a)):(ris=(b)))>(c)?ris:(ris=(c)))
#define S3J_MIN(a,b,c,ris)	(((a)<(b)?(ris=(a)):(ris=(b)))<(c)?ris:(ris=(c)))


double s3j(double j1, double j2, double j3, 
		   double m1, double m2, double m3) {

/*  ( j1 j2 j3 )
    (          ) = delta(m1+m2+m3,0) * (-1)^(j1-j2-m3) * 
    ( m1 m2 m3 )

      +-
      |  (j1+j2-j3)! (j1-j2+j3)! (-j1+j2+j3)! 
    * | -------------------------------------- ...
      |
      +-
                                                                 -+ 1/2
           (j1-m1)! (j1+m1)! (j2-m2)! (j2+m2)! (j3-m3)! (j3+m3)!  |
      ... ------------------------------------------------------- |     * 
                              (j1+j2+j3+1)!                       |
                                                                 -+

         +---
          \                       (-1)^k
      *    |   ---------------------------------------------------------------------
          /      k! (j1+j2-j3-k)! (j1-m1-k)! (j2+m2-k)! (j3-j2+m1+k)! (j3-j1-m2+k)!
         +---
           k

  Where factorials must have non-negative integral values:

	j1+j2-j3   >= 0		j1-j2+j3   >= 0		-j1+j2+j3 >= 0		j1+j2+j3+1 >= 0
	k          >= 0		j1+j2-j3-k >= 0		j1-m1-k   >= 0		j2+m2-k    >= 0
	j3-j2+m1+k >= 0		j3-j1-m2+k >= 0

  The 3j symbol is therefore non-null if

				j1+j2 >= j3		(1)
				j1+j3 >= j2		(2)
				j2+j3 >= j1		(3)

  and k values in the sum must be such that

				k <= j1+j2-j3		(4)			k >= 0				(7)
				k <= j1-m1			(5)			k >= -j3+j2-m1		(8)
				k <= j2+m2			(6)			k >= -j3+j1+m2		(9)

  If no values of k satisfy the (4) to (9), the result is null because the sum is null,
  otherwise one can find   kmin < kmax   such that

							kmin <= k <= kmax

		(4) to (6) => kmin=MAX(j1+j2-j3,  j1-m1,       j2+m2     )
		(7) to (9) => kmax=MIN(0,         -j3+j2-m1,   -j3+j1+m2 )

  The condition kmin < kmax includes (1) to (3) because

		(4) and (7)    =>    (1)
		(5) and (8)    =>    (2)
		(6) and (9)    =>    (3)

  Once the values of kmin and kmax are found, the only "selection rule" is kmin<kmax.
*/
        static double * f = NULL; 
        static size_t f_size = 0;
        size_t f_size_old = f_size;

	int k, kmin, kmax;
	int jpm1, jmm1, jpm2, jmm2, jpm3, jmm3;
	int j1pj2mj3, j3mj2pm1, j3mj1mm2;
	double ris, mult;
	
	jpm1=(int)(j1+m1);
	if (!S3J_EQUAL(jpm1,j1+m1)) return 0.0;

	jpm2=(int)(j2+m2);
	if (!S3J_EQUAL(jpm2,j2+m2)) return 0.0;

	jpm3=(int)(j3+m3);
	if (!S3J_EQUAL(jpm3,j3+m3)) return 0.0;

	jmm1=(int)(j1-m1);
	if (!S3J_EQUAL(jmm1,j1-m1)) return 0.0;

	jmm2=(int)(j2-m2);
	if (!S3J_EQUAL(jmm2,j2-m2)) return 0.0;

	jmm3=(int)(j3-m3);
	if (!S3J_EQUAL(jmm3,j3-m3)) return 0.0;

	/* delta(m1+m2+m3,0) */
	if ((jpm1-jmm1+jpm2-jmm2+jpm3-jmm3)!=0) return 0.0;

	/* j1+j2-j3 = (j1+j2-j3) + (m1+m2+m3) = jpm1+jpm2-jmm3 */
	j1pj2mj3=jpm1+jpm2-jmm3;

	/* j3-j2+m1 = (j3-j2+m1) - (m1+m2+m3) = jmm3-jpm2 */
	j3mj2pm1=jmm3-jpm2;

	/* j3-j1-m2 = (j3-j1-m2) + (m1+m2+m3) = jpm3-jmm1 */
	j3mj1mm2=jpm3-jmm1;
	

	S3J_MAX(-j3mj2pm1, -j3mj1mm2,   0,         kmin);
	S3J_MIN(j1pj2mj3,  jmm1,       jpm2,       kmax);
	if (kmin>kmax) return 0.0;	

        k = (int)(j1+j2+j3+m1+m2+m3 + 1.0) + 1;
        f_size = f_size >= k ? f_size : k;
        if (f_size > f_size_old) {
            /* begin futher allocation for factorial lookup table */
            f = realloc(f, f_size * sizeof(double));
            printf("realloc()'d f[] to length = %i from %i\n", (int)f_size, (int)f_size_old);

            if (f_size_old == 0) {
                f[0] = 1.0;
                f_size_old = 1;
            }
            for (k = f_size_old; k < f_size; k++) {
                f[k] = f[k-1]*k;
            }

        }//fi

	ris=0.0;
	if (kmin%2==0) mult=1.0;
	else mult=-1.0;
	for (k=kmin; k<=kmax; ++k) {

		ris+=mult/(f[k]*f[j1pj2mj3-k]*f[jmm1-k]*f[jpm2-k]*f[j3mj2pm1+k]*f[j3mj1mm2+k]);

		mult=-mult;
	}

	/* (-1)^(j1-j2-m3)=(-1)^(j1-j2-m3+m1+m2+m3)=(-1)^(jpm1-jmm2) */
	if ((jpm1-jmm2)%2!=0) ris=-ris;

	ris*=sqrt(f[j1pj2mj3]*f[jpm1-jmm2+jpm3]*f[-jmm1+jpm2+jpm3]*
		f[jpm1]*f[jpm2]*f[jpm3]*f[jmm1]*f[jmm2]*f[jmm3]/
		f[jpm1+jpm2+jpm3+1]);

	return ris;
}

double cg (double j1, double m1, double j2, double m2, double j, double m) {

    int esp = (int)(j1-j2+m);
    double cgris;

    if (!S3J_EQUAL(esp,j1-j2+m)) return 0;

    if (esp%2==0) cgris=1.0;
    else cgris=-1.0;

    cgris*=sqrt(2*j+1)*s3j(j1,j2,j,m1,m2,-m);

    return cgris;
}
































#ifdef S3J_TEST

#define S3J_EQUALTOL(a,b,tol)		(fabs(a-b)<tol)

int test3j(double j1, double j2, double j3, 
		   double m1, double m2, double m3, 
		   double ris, double tol) {

	double s3jris;

	s3jris=s3j(j1,j2,j3,m1,m2,m3);
	printf(" (%4.1lf %4.1lf %4.1lf)\n",j1,j2,j3);
	printf(" (              )      = %.4lf %s %.4lf   (tol. %lg)\n",s3jris,
		S3J_EQUALTOL(s3jris,ris,tol)?"==":"!=",ris,tol);
	printf(" (%4.1lf %4.1lf %4.1lf)\n",m1,m2,m3);

	return S3J_EQUALTOL(s3jris,ris,tol);
}

int testCleGor(double j1, double m1, double j2, double m2, double j, double m,
			   double ris, double tol) {

	double cgris = cg (j1,m1,j2,m2,j,m);

	printf("<{%lg %lg} {%lg %lg} | %lg %lg> = %.15lg %s %.15lg   (tol %lg)\n",
		j1,m1,j2,m2,j,m,cgris,S3J_EQUALTOL(cgris,ris,tol)?"==":"!=",ris,tol);

	return S3J_EQUALTOL(cgris,ris,tol);
}
#include <sys/time.h>

double msecdiff (struct timeval * ti, struct timeval *tf) {
    double s_to_ms = (tf->tv_sec - ti->tv_sec) * 1000.0;
    double us_to_ms = (((double)(tf->tv_usec - ti->tv_usec)) / 1000.0);

    return s_to_ms + us_to_ms;
}

int main(int argc, char *argv[]) {

	int val[]={3,4,5,3,4};
	int i, ris;
        struct timeval ti = {0,0};
        struct timeval tf = {0,0};

        gettimeofday(&ti,NULL);
        for (i = 0; i < 10000; i++) (void)s3j(10,10,10,0,0,0);
        gettimeofday(&tf,NULL);
        printf("        10000 times    took %6.3lg ms\n", msecdiff(&ti,&tf));

	printf("s3j by Riccardo Gusmeroli (web.address@libero.it)\n");
	printf("altered by Spencer Olson  (olsonse@umich.edu)\n");
	printf("           ******* TEST SUITE *******\n\n\n");
	for (i=0; i<3; ++i) {

		printf("S3J_MIN(%d,%d,%d)==%d\n",val[i],val[i+1],val[i+2],S3J_MIN(val[i],val[i+1],val[i+2],ris));
		if (ris!=3) {
			
			printf("Error %d!=3 !!! See macro definition for S3J_MIN\n",ris);
			return -1;
		}
	}
	printf("---------------------- Macro S3J_MIN: Passed.\n");

	for (i=0; i<3; ++i) {

		printf("S3J_MAX(%d,%d,%d)==%d\n",val[i],val[i+1],val[i+2],S3J_MAX(val[i],val[i+1],val[i+2],ris));
		
		if (ris!=5) {
			
			printf("Error %d!=3 !!! See macro definition for S3J_MAX\n",ris);
			return -1;
		}
	}
	printf("---------------------- Macro S3J_MAX: Passed.\n");

	if (!test3j(1.5,1.0,0.5,0.5,-1.0,0.5,.288675134595,1e-12)) {
		
		printf("---------------------- Error in s3j.\n");
		return -1;

	} else printf("---------------------- Function s3j evaluation: Passed.\n");

        gettimeofday(&ti,NULL);
        testCleGor(1.5,1.5,1.0,-1.0,2.5,0.5,sqrt(1.0/10.0),1e-14);
        gettimeofday(&tf,NULL);
        printf("         first one     took %.3lg ms\n", msecdiff(&ti,&tf));

        gettimeofday(&ti,NULL);
                /*          j1   m1  j2  m2   j   m                          */
	if (    !testCleGor(1.5,1.5,1.0,-1.0,0.5,0.5,sqrt(1.0/2.0),1e-14) ||
		!testCleGor(1.5,1.5,1.0,-1.0,1.5,0.5,sqrt(4.0/10.0),1e-14) ||
		!testCleGor(1.5,1.5,1.0,-1.0,2.5,0.5,sqrt(1.0/10.0),1e-14)
		) {

		printf("---------------------- Error in s3j.\n");
		return -1;

	} else {
            gettimeofday(&tf,NULL);
            printf("     second bunch      took %.3lg ms\n", msecdiff(&ti,&tf));
            printf("---------------------- Function s3j in Clebsh-Gordon: Passed.\n");
        }


	return 0;
}

#endif // def S3J_TEST

