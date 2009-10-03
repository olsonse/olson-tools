
#ifndef LOOKUP_COMMON_H
#define LOOKUP_COMMON_H

#include <olson-tools/bfield.h>
#include <physical/physical.h>
#include <olson-tools/force-lookup.h>

using namespace physical::constants::si;
using namespace physical::units;

const double I = 300.0 * Ampere;
const double mass = 87.0 * amu; /* mass of 87Rb */
const double delta_B = 0.1 * microns;

namespace obf = olson_tools::BField;

/** Guide wires without the knee. */
obf::ThinCurrentElement wires[] = {
    /* source for wire 2. */
    obf::ThinCurrentElement(-0.0032567, 0.0, -2.0, -0.0012567, 0.0, 4.0,I),

    /* source for wire 1. */
    obf::ThinCurrentElement( 0.0032567, 0.0, -2.0,  0.0012567, 0.0, 4.0,I),

    /* end marker. */
    obf::ThinCurrentElement(0,0,0,0,0,0,0)
};

/** Function for easily adding ThiWireBSrc elements to a ThinWireSrc. */
template <class ThinWireBSrc>
inline void addwires(ThinWireBSrc & bsrc) {
    for (int i = 0; fabs(wires[i].I) > 0; bsrc.currents.push_back(wires[i++]));
}

typedef olson_tools::AddForce< obf::BCalcs< obf::ThinWireSrc >, olson_tools::Gravity > BFieldForce;

typedef olson_tools::ForceTableWrapper< BFieldForce > BFieldForceTableSrc;

#define FIELD_FILENAME "field.dat"

#define ERR_FILE "error.dat"

#endif // LOOKUP_COMMON_H
