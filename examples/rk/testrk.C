
#include <iostream>
#include <fstream>
#include <olson-tools/logger.h>
#include <olson-tools/rk.h>
#include <olson-tools/random.h>
#define X 0
#define Y 1
#define Z 2

#define VX 3
#define VY 4
#define VZ 5
class Particle {
  public:
   double pos[3];
   double vel[3];
   static int ndim;
} __attribute__ ((packed));
int Particle::ndim = 6;

std::ostream & operator<<(std::ostream & output, Particle & p) {
    return output
	   << p.pos[X] << '\t' << p.pos[Y] << '\t' << p.pos[Z] << '\t'
	   << p.vel[X] << '\t' << p.vel[Y] << '\t' << p.vel[Z];
}

const double k_m[] = {
    9869604.4 /*(rad/s)^2 = (2 pi 500  Hz)^2 */,
    9869604.4 /*(rad/s)^2 = (2 pi 500  Hz)^2 */,
    9869604.4 /*(rad/s)^2 = (2 pi 500  Hz)^2 */
};

const double V_dx[] = {
    0.1 /*m/s */,
    0.1 /*m/s */,
    0.1 /*m/s */
};

const double V_w[] = {
    0.25*3141 /*rad/s */,
    0 /*rad/s */,
    0 /*rad/s */
};

void getderivs(const Particle *p, const double *time, double *F, void *args) {
    /* copy over the dx/dt, dy/dt, and dz/dt */
    F[X] = p->vel[X];
    F[Y] = p->vel[Y];
    F[Z] = p->vel[Z];

    /* and accelerations for a simple harmonic oscillator (kind of simple) */
    F[VX] = - k_m[X] * (p->pos[X] + V_dx[X]*( sin( V_w[X] * (*time) ) ) );
    F[VY] = - k_m[Y] * (p->pos[Y] + V_dx[Y]*( sin( V_w[Y] * (*time) ) ) );
    F[VZ] = - k_m[Z] * (p->pos[Z] + V_dx[Z]*( sin( V_w[Z] * (*time) ) ) );
}

int main() {
    std::cout << "machine precision has been measured to " << getMachineEPS() << std::endl;
    Particle p1;

    setLogProgramName("RK Test");
    //double sigma_pos = 10e-3;
    //double sigma_vel = 50e-2;

    p1.pos[X] = 0.1;//gauss_deviate(&sigma_pos);
    p1.pos[Y] = 0.1;//gauss_deviate(&sigma_pos);
    p1.pos[Z] = 0.1;//gauss_deviate(&sigma_pos);
    p1.vel[X] = 0.0;//gauss_deviate(&sigma_vel);
    p1.vel[Y] = 0.0;//gauss_deviate(&sigma_vel);
    p1.vel[Z] = 0.0;//gauss_deviate(&sigma_vel);

    const double T = 0.03L;
    double dt = 1e-4L,
           dt_step = 1e-5L;
    const double errmax = 0.28e-6L;
    std::ofstream fout ("/tmp/out.dat");

    std::cout << "before rk: "<< p1 << '\t' << dt_step << std::endl;
    for (double t  = 0; t < T; t+= dt) {
	rk_adapt_driver((double*)&p1, &Particle::ndim, &t, &dt, &dt_step, (derivativesFunction)getderivs, NULL, &errmax);
	fout << p1 << '\t' << dt_step << '\n';
    }
    std::cout << "after rk: "<< p1 << '\t' << dt_step << std::endl;
    fout.flush();
    fout.close();

    return 0;
}

