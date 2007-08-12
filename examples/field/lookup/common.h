
#ifndef LOOKUP_COMMON_H
#define LOOKUP_COMMON_H

#include <olson-tools/bfield.h>
#include <olson-tools/physical.h>
#include <olson-tools/force-lookup.h>

using namespace physical::constants;
using namespace physical::units;

const double I = 300.0 * Ampere;
const double mass = 87.0 * amu; /* mass of 87Rb */
const double delta_B = 0.1 * microns;

/** Guide wires without the knee. */
BField::ThinCurrentElement wires[] = {
    /* source for wire 2. */
    BField::ThinCurrentElement(-0.0032567, 0.0, -2.0, -0.0012567, 0.0, 4.0,I),

    /* source for wire 1. */
    BField::ThinCurrentElement( 0.0032567, 0.0, -2.0,  0.0012567, 0.0, 4.0,I),

    /* end marker. */
    BField::ThinCurrentElement(0,0,0,0,0,0,0)
};

/** Function for easily adding ThiWireBSrc elements to a ThinWireSrc. */
template <class ThinWireBSrc>
inline void addwires(ThinWireBSrc & bsrc) {
    for (int i = 0; fabs(wires[i].I) > 0; bsrc.currents.push_back(wires[i++]));
}

typedef AddForce< BField::BCalcs< BField::ThinWireSrc >, Gravity > BFieldForce;

typedef ForceTableWrapper< BFieldForce > BFieldForceTableSrc;

#define FIELD_FILENAME "field.dat"

#define ERR_FILE "error.dat"

#endif // LOOKUP_COMMON_H
