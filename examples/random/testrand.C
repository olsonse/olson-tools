#include <olson-tools/random.h>
#include <iostream>
#include <fstream>
using std::cin;
using std::cout;
using std::flush;
using std::ofstream;
extern int errno;

int main() {
    double sigma = 1.0;
    int N = 100;

    cout << "enter sigma value:  " << flush;
    cin >> sigma;
    cout << "enter total iterations:  " << flush;
    cin >> N;

    ofstream outfile;
    outfile.open("test.dat");

    for ( int i = 0; i < N; i++) {
	outfile << gauss_deviate(&sigma) << '\n';
    }

    outfile.flush();
    outfile.close();
}
