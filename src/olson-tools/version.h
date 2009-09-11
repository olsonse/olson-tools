// -*- c++ -*-
// $Id$
/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2007-2008 Spencer E. Olson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *  
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *                                                                                 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.                                                                           .
 * 
 * Questions? Contact Spencer Olson (olsonse@umich.edu) 
 */

/** \file Just a (dumb) set of routines to make sure that version info gets
 * compiled into whatever is compiled (avoiding optimization removal).
 *
 * Olson Tools version:  > r588
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
