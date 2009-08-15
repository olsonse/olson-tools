
#include <olson-tools/fit/simfit.h>
#include <iostream>

using olson_tools::fit::Fit;

template <typename T>
class bob : public Fit<T> {
  /* MEMBER STORAGE */
  /** Number of parameters. */
  int n;

  /** Number of evaluations. */
  int nevals;

public:
  /** Constructor. */
  bob(T par[], unsigned int nParams)
    : Fit<T>(par,nParams), n(nParams), nevals(0) {}

  const int & evaluations() const { return nevals; }

  virtual ~bob() {}

  virtual double minfunc(T par[]) {
    ++nevals;
    double retval = 0;
    for (int i = 0; i < n; i++ ) {
      retval += par[i] * par[i] + 1.0;
    }
    return retval;
  }
};

static float par[3] = { 1, 2, 3 };
static  bob<float> billy(par, 3);

int main() {
  using std::cout;
  using std::endl;
  cout << "fit merit("<<par[0] << ','<<par[1]<<','<<par[2]<<") : " << billy.minfunc(par) << endl;
  cout << "simplex   : " << billy.simplex(1e-9);
  cout << ", iter = " << billy.iterations();
  cout << ", minfunc() evaluations:  " << (billy.evaluations()-1) << endl;
  cout << "fit merit("<<par[0] << ','<<par[1]<<','<<par[2]<<") : " << billy.minfunc(par) << endl;
    return 0;
}
