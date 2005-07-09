#ifdef DUMMY_IFDEF_FOR_FORTRAN
// $Id: fmacros.h,v 1.1 2005/05/12 04:27:31 olsonse Exp $
/*
 * Copyright 2005 Spencer Eugene Olson --- All Rights Reserved
 *
 * $Log: fmacros.h,v $
 * Revision 1.1  2005/05/12 04:27:31  olsonse
 * Fixed to for Intel 8.1 compilers.
 * Found (using intel compiler) and fixed an array overflow in BField::potential.
 * Didn't find it earlier because the array is on the stack for the function.
 *
 * Added fmacros.h file to simplify mixing fortran code with others.
 * Added alias function names for Fortran interoperability.
 *
 *
 */
#endif

/** \file
 * Some helpful macros to use in fortran files.
 * Copyright 2005 Spencer Olson.
 */

#ifndef FMACROS_H
#define FMACROS_H

#if defined (F77COMP_f95) || defined (F77COMP_ifort)
#  define FFUNC_(fun) fun ## _
#else
#  define FFUNC_(fun) fun
#endif

#endif // FMACROS_H
