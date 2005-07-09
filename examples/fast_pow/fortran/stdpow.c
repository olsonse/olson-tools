/** \file
 * This simply provides a way to call the math pow function from a
 * c-interface.
 */
extern double pow(double,double);

double stdpow_(double*x,double*y) {
    return pow(*x,*y);
}
