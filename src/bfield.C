// -*- c++ -*-
// $Id$
/*
 * Copyright 2004 Spencer Olson
 *
 * $Log$
 *
 */

#include "bfield.h"
#include <math.h>

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

/*
! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! Calculate the B Field for a bunch of thin current elements
! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
void thgetb(double B[4], const double r[3], const Args * f) {
      
    double x1,x2,rho,dB,rl,h1;

    double y[3],z[3],rn[3];
    double rcut = f->rcut;
      
    if (rcut < 1e-10) rcut = 1e-10;

    /* set field vector to zero */
    memset(B, 0, 4*sizeof(double));
        
    /*
     * integrate over the fields produced by the discrete
     * current elements. The implemented formula can be obtained by
     * analytically integrating the field produced by a linear 
     * current using Biot-Savart's law. 
     *
     * Some definitions:
     *  B(r) == returned bfield (where r is the passed in vector).
     *  rl   == length of thin wire segment (m).
     *  rn   == unit vector from point a to point b of thin wire segment.
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
 
        /* Increment the field vector B(3) by the field produced
         * by the presently considered current element.         
         */
        if (h1 > 0) {
            for (int j = X; j <= Z; j++) {
                B[j] += dB * (z[j]/h1);
            }
        }

    }/* for */

    /* add in a background */
    B[X] += f->bg[X];
    B[Y] += f->bg[Y];
    B[Z] += f->bg[Z];

    /* Get the magnitude of B. */
    B[Z+1] = sqrt( DOTP(B,B) );
}

} /* namespace */
