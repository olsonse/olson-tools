/** Demonstrates the use of the "registry" mechanism to store a list of
 * variable names (in std::string format) linked to a particular object in
 * memory.
 *
 * This mechanism is not yet currently in use in the initialization of the
 * units/constants physical::Quantity items.  
 * */


#include <iostream>
#include <olson-tools/physical/quantity.h>

int main() {
    physical::Quantity a = physical::Quantity(physical::unit::m,"","physical::unit::m");

    std::cout << "a  = " << a << "\n"
                 "&a = " << &a << "\n"
                 "&m = " << physical::Quantity::registry::list["physical::unit::m"] << "\n"
              << std::flush;
    return 0;
}
