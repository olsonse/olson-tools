// -*- c++ -*-
// $Id$
/*
 * Copyright 2006 Spencer Olson
 *
 * $Log$
 *
 */

/** \file
 * Some care to use printf(...)/exit(1) instead of throw(...).  Macros are
 * defined to make it easier to allow for OpenMP code, where C++ exceptions
 * are either not supported or have restrictions placed on their use.
 */

/** \macro THROWS(exceptlist)
 * Macro to help exceptions be politely changed/commented out for use with
 * OpenMP code.  This macro should be used for declaring the throw list.  The
 * name THROWS instead of the C++ throw follows the Java semantics (maybe) and
 * is just because I don't think you can overload macros.
 * @param exceptlist
 *     The comma separated list of exception to be thrown (if OpenMP is not enabled).
 */

/** \macro THROW(except,msg)
 * Macro to help exceptions be politely changed/commented out for use with
 * OpenMP code.
 * @param except
 *     The exception to throw (if OpenMP is not enabled).
 * @param msg
 *     The argument to except (or the message to print if OpenMP IS enabled).
 */

/** \macro TRY
 * Macro to help exceptions be politely changed/commented out for use with
 * OpenMP code.  If OpenMP is used, this is a comment; else a try statement is
 * issued.
 */

/** \macro CATCH(except,code)
 * Macro to help exceptions be politely changed/commented out for use with
 * OpenMP code.  If OpenMP is used, this is a comment; else a catch statement is
 * issued with the specified exception and code.
 */

#ifndef OPMEXCEPT_H
#define OPMEXCEPT_H

#ifdef _OPENMP
#  define THROWS(exceptlist)
#  define THROW(except,msg) { std::cerr << "exceptions disabled, exiting:  " # msg << std::endl; exit(1);}
#  define TRY
#  define CATCH(except,code)
#else
#  define THROWS(exceptlist)    throw (exceptlist)
#  define THROW(except,msg)     throw except(msg)
#  define TRY                   try
#  define CATCH(except,code)    catch (except) { code }
#endif

#endif // OPMEXCEPT_H
