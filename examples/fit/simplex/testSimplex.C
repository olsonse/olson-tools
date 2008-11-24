
#include <olson-tools/fit/simfit.h>
#include <iostream>

using olson_tools::fit::Fit;

template <typename T>
class bob : public Fit<T> {
public:
    bob(T par[], unsigned int nParams): Fit<T>(par,nParams), n(nParams) {}
    virtual ~bob() {}
    virtual double minfunc(T par[]) {
	double retval = 0;
	for (int i = 0; i < n; i++ ) {
	    retval += par[i] * par[i] + 1.0;
	}
	return retval;
    }

    int n;
};

static float par[3] = { 1, 2, 3 };
static  bob<float> billy(par, 3);

int main() {
    std::cout << "fit merit("<<par[0] << ','<<par[1]<<','<<par[2]<<") : " << billy.minfunc(par) << std::endl;
    std::cout << "simplex   : " << billy.simplex(1e-9) << ", iter = " << billy.iterations() << std::endl;
    std::cout << "fit merit("<<par[0] << ','<<par[1]<<','<<par[2]<<") : " << billy.minfunc(par) << std::endl;
    return 0;
}
