#include "bfield.h"
#include <math.h>

#define DOTP(a,b)       ( (((a)[X])*((b)[X])) + (((a)[Y])*((b)[Y])) + (((a)[Z])*((b)[Z])) )

#define CROSSP(result,a,b) \
        (result)[X] = (a)[Y] * (b)[Z]  -  (a)[Z] * (b)[Y]; \
        (result)[Y] = (a)[Z] * (b)[X]  -  (a)[X] * (b)[Z]; \
        (result)[Z] = (a)[X] * (b)[Y]  -  (a)[Y] * (b)[X];

#define SQR(x)  ((x)*(x))

namespace BField {

/* this must currently be a time independent force/macroatomic mass
! this function computes the !!!ACTUAL!!!! force on the atom due to it
! being in a magnetic potential
*/
void derivs(const double p[VZ+1], const double * time, double rkf[VZ+1], void * f) {
    rkf[X]  = p[VX];
    rkf[Y]  = p[VY];
    rkf[Z]  = p[VZ];
    accel(rkf+VX, p, (const Args*)f);
}

double potential(double r[3], Args * f) {
    static const double mu = (-0.5) * (-1) * physical::constant::mu_B;
    double B[Z+1];

    thgetb(B, r, f);

#if 1
    B[X] *= -mu;
    B[X] +=  f->gravity[X];
    B[Y] *= -mu;
    B[Y] +=  f->gravity[Y];
    B[Z] *= -mu;
    B[Z] +=  f->gravity[Z];
    return sqrt( DOTP(B,B) );
#else
    B[X] += f->bg[X];
    B[Y] += f->bg[Y];
    B[Z] += f->bg[Z];
    return mu * sqrt( DOTP(B,B) );
#endif
}

/*
! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! Calculate the Bfield for a bunch of thin current elements
! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void thgetb(double Bfield[4], const double r[3], const Args * f) {
      
    double x1,x2,rho,dB,rl,h1;

    double y[3],z[3],rn[3];
    double rcut = f->rcut;
      
    if (rcut < 1e-10) rcut = 1e-10;

    /* set field vector to zero */
    for (int i = Z+1; i >= X; Bfield[i--]=0 );
        
    /*
    ! integrate over the fields produced by the discrete
    ! current elements. The implemented formula can be obtained by
    ! analytically integrating the field produced by a linear 
    ! current using Biot-Savart's law. 
    */
          
    for (int k = f->currents.size()-1; k >= 0; k--) {
        const ThinCurrentElement & cur = f->currents[k];

        for (int j = X; j <= Z; j++) {
            rn[j]= cur.pb[j] - cur.pa[j];
        }
         
        rl = sqrt (DOTP(rn,rn));

        rn[X] /= rl;
        rn[Y] /= rl;
        rn[Z] /= rl;

        y[X] = r[X]-cur.pa[X];
        y[Y] = r[Y]-cur.pa[Y];
        y[Z] = r[Z]-cur.pa[Z];

        /* x1 = y * rn */
        x1 = DOTP(y,rn);

        /* x2 = (y * rn) - rl */
        x2=x1-rl;

        /* rho = sqrt ( (y * y) - (y * rn)**2 ) */
        rho = sqrt( DOTP(y,y) - SQR(x1) );

        /* This IF avoids accidental zeros in denominators. Outside
         * the coil packs, there is no effect.            
         */
        if (rho < rcut) rho = rcut;
 
        dB = 1e-7 * cur.I *(   x1/sqrt( SQR(x1) + SQR(rho) )
                             - x2/sqrt( SQR(x2) + SQR(rho) )
                           ) / rho;
        
        /* z = rn X y */
        CROSSP(z /* == */, rn /* X */, y);
 
        /* h1 = sqrt(z * z) */
        h1=sqrt( DOTP(z,z) );
 
        /* Increment the field vector Bfield(3) by the field produced
         * by the presently considered current element.         
         */
        if (h1 > 0) {
            for (int j = X; j <= Z; j++) {
                Bfield[j] += dB * (z[j]/h1);
            }
        }

    }/* for */

    /* add in a background */
    Bfield[X] += f->bg[X];
    Bfield[Y] += f->bg[Y];
    Bfield[Z] += f->bg[Z];

    /* Get the magnitude of B. */
    Bfield[Z+1] = sqrt( DOTP(Bfield,Bfield) );
}

}; /* namespace */
