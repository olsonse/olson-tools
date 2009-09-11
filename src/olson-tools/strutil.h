/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2005-2008 Spencer E. Olson
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

/** \file
 * Some utility functions to help convert from different types to/from std::string.
 *
 * Copyright 2005-2008 Spencer Olson.
 */

#ifndef STRUTIL_H
#define STRUTIL_H

#include <string>
#include <sstream>
#include <algorithm>
#include <stdexcept>

namespace olson_tools {

struct string_error : std::runtime_error {
    typedef std::runtime_error super;
    string_error(const std::string & s) : super(s) {}
};

template< class T>
inline std::string to_string( const T & Value)
{
    std::stringstream streamOut;
    streamOut << Value;
    return streamOut.str( );
}

//  specialization for string.
template< >
inline std::string to_string( const std::string & Value)
{
    return Value;
}

template< class T>
inline T from_string( const std::string & ToConvert)
{
    std::stringstream streamIn( ToConvert);
    T ReturnValue = T( );
    streamIn >> ReturnValue;
    if (!streamIn)
        throw string_error("could not convert from string");
    return ReturnValue;
}

inline std::string tolower( const std::string & ToLower) {
    std::string retval;
    std::transform(ToLower.begin(), ToLower.end(),
                   retval.begin(),
                   static_cast<int(*)(int)>(std::tolower));
    return retval;
}

}/* namespace olson_tools */

#endif // STRUTIL_H
