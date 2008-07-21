// -*- c++ -*-
// $Id$
/*
 * Copyright 2007-2008 Spencer E. Olson
 *
 */

/** \file Just a (dumb) set of routines to make sure that version info gets
 * compiled into whatever is compiled (avoiding optimization removal).
 *
 * Olson Tools version:  > r580
 */


#ifndef OLSON_TOOLS_VERSION_DUMMIES_H
#define OLSON_TOOLS_VERSION_DUMMIES_H

#include <vector>
#include <string>
#include <sstream>
#include <iterator>

/** For lack of a better name of a namespace. */
namespace olson_tools {

static std::vector<std::string> rcsid_vector;

/** Record the rcs id. */ 
int add_rcsid(std::string rcsid) {
    rcsid_vector.push_back(rcsid);
    return 1;
}

/** Retrieve a c++ string that lists all rcs IDs that were
 * recorded by add_rcsid.  Each rcs ID is on a separate line.  Each line in
 * the list is prepended by a '#' for use as a comment in a data file. 
 */
std::string get_rcsid_strings() {
    std::ostringstream ostr;
    ostr << "# ";
    std::copy(rcsid_vector.begin(),
              rcsid_vector.end(),
              std::ostream_iterator<std::string>(ostr, "\n# "));
    ostr << "end of rcsid list";
    return ostr.str();
}

/** A dummy variable to allow us to add this rcsid in this header (don't have
 * to add it to "main()" or whatever. */
int version_dummy = add_rcsid("$Id$");

} /* namespace */

#endif // OLSON_TOOLS_VERSION_DUMMIES_H
