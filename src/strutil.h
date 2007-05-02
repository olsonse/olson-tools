/** \file
 * Some utility functions to help convert from different types to/from std::string.
 *
 * Copyright 2005 Spencer Olson.
 */

#ifndef STRUTIL_H
#define STRUTIL_H

#include <string>
#include <sstream>

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
    return ReturnValue;
}

inline std::string tolower( const std::string & ToLower) {
    std::string retval;
    std::transform(ToLower.begin(), ToLower.end(),
                   retval.begin(),
                   static_cast<int(*)(int)>(std::tolower));
    return retval;
}

#endif // STRUTIL_H
