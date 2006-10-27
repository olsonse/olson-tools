/** This is a simple program to demonstrate <some> of the use of the Vector
 * class.
 */


#include <olson-tools/Vector.h>

int main () {

    double ve[3] = {0,0,0};
    const double I[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
    SquareMatrix<double,3> m3((const double [3][3]){{1,0,0},{0,1,0},{0,0,1}});
    Vector <double, 3> i(ve);
    Vector <double, 3> j;
    j = (const double[3]) {1,2,3};
    Vector <double, 3> l((const double[3]){1,2,3});
    i=ve;
    i*=1;
    i-=1;
    i+=1;
    SquareMatrix<double,3> m1(I);
    SquareMatrix<double,3> m2;

    double k = i*(2*j*2 - 1);
    Vector <double,3> r = m2*m1*(i - j)/3.0;

    Vector <double,3> & h = *((Vector<double,3>*)ve);

    SquareMatrix<double,3> rz ( (const double [3][3])
                         {{ 1,    1,    0   },
                          {-1,    1,    0   },
                          {   0,         0,       1   }}
    );

    i[0]  = 1*h*(m3*r);
    i[1]  = k*r*l;
    return 0;
}
