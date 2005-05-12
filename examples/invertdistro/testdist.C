#include <math.h>
#include <stdlib.h>
#include <olson-tools/physical.h>
#include <olson-tools/random.h>
#include <iostream>
#include <fstream>
#include <olson-tools/Distribution.h>

using namespace physical::constants;

inline double max(const double & a, const double & b) {
    return a > b ? a : b;
}

template <class T>
inline T SQR( const T & a) {return a * a;}

void nextvel(double p[3]) {
    static double betaw = sqrt(2.0 * K_B * 500.0 * uK / (87.0 * amu) );

    double ran = max(1.e-8,MTRNGrand());
    double B   = sqrt(-log(ran));

    double A   = 2.0 * pi * MTRNGrand();
    double uu  = B * sin(A) * betaw;
    double ww  = B * cos(A) * betaw;

           ran = max(1.e-8, MTRNGrand());
    double vv  = sqrt(-log(ran)) * betaw;

    p[0] = -vv;
    p[1] = uu;
    p[2] = ww;
}

struct{
    double distrib (const double & v) const {
        return v*v * exp(-0.5 * 87.0 * amu * v*v / ( K_B * 500.0 * uK ) );
    }
} ThermalDistrib3d ;

struct {
    double distrib (const double & v) const {
        return v * exp(-0.5 * 87.0 * amu * v*v / ( K_B * 500.0 * uK ) );
    }
} ThermalDistrib2d;

void nextvelb(double p[3]) {
    /* get the speed from a maxwellian distribution. */
    static double sigma_v = sqrt(3.0 * K_B * 500.0 * uK /(87.0 * amu));
    //double speed = fabs(gauss_deviate(&sigma_v));

    static Distribution distro = Distribution(ThermalDistrib3d, 0.0, 6*sigma_v, 1000);
    double speed = distro();
    /* now get the direction:  cosine distribution in \theta and
     * uniform in \phi */
    double sintheta2 = MTRNGrand();
    double sintheta  = sqrt(sintheta2);
    double costheta  = sqrt(1 - sintheta2);
    double phi       = 2.0 * pi * MTRNGrand();
    double cosphi;
    double sinphi;

#if (__amd64__ == 1 || __i386__ == 1)
    asm ("fsincos" : "=t" (cosphi), "=u" (sinphi) : "0" (phi));
#else
    cosphi = cos(phi);
    sinphi = sin(phi);
#endif

    p[2] = speed*(cosphi * sintheta);
    p[1] = speed*(sinphi * sintheta);
    p[0] = -speed*(costheta);
}

void nextvelc(double p[3]) {
    /* get the speed from a maxwellian distribution. */
    static double sigma_v = sqrt(K_B * 500.0 * uK /(87.0 * amu));
    p[0] =  gauss_deviate(&sigma_v);
    p[1] =  gauss_deviate(&sigma_v);
    p[2] =  gauss_deviate(&sigma_v);
}

void nextveld(double p[3]) {
    /* get the speed from a maxwellian distribution. */
    static double sigma_v_xyz = sqrt(K_B * 500.0 * uK /(87.0 * amu));
    //double speed = fabs(gauss_deviate(&sigma_v));

    static Distribution distro = Distribution(ThermalDistrib2d, 0.0, 6*sigma_v_xyz, 100);
    p[2] = gauss_deviate(&sigma_v_xyz);
    p[1] = gauss_deviate(&sigma_v_xyz);
    p[0] = -distro();
}

/** A flat distribution for use.
*/
struct {
    /** Return 0.5.
     */
    inline double distrib (const double & x) const {
        return 0.5;
    }
} FlatDistribution;

void nextvele(double p[3]) {
    /* get the speed from a maxwellian distribution. */
    static double sigma_v_xyz = sqrt(K_B * 500.0 * uK /(87.0 * amu));
    //double speed = fabs(gauss_deviate(&sigma_v));

    static Distribution distro = Distribution(FlatDistribution,-0.001, 0.0, 1000);
    p[2] = gauss_deviate(&sigma_v_xyz);
    p[1] = gauss_deviate(&sigma_v_xyz);
    p[0] = distro();
}

int main() {
    std::ofstream outf("/tmp/tmpdist.dat");
    for (int i = 0; i < 100000; i++) {
        double p[3];
        nextveld(p);

        outf      << p[0] << '\t'
                  << p[1] << '\t'
                  << p[2] << std::endl;
    }
    return 0;
}

