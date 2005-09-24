
#include <olson-tools/readData.h>
#include <fstream>
#include <iostream>

int main() {
    std::ifstream input ("testdata.dat");

    std::vector<Data<double,4> *> retval;
    readDataBlocks(input,retval);

    std::cout << "read in " << retval.size() << " data blocks" << std::endl;

    return 0;
}

