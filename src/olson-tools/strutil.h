/** \file
 * Some utility functions to help convert from different types to/from std::string.
 *
 * Copyright 2005 Spencer Olson.
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
