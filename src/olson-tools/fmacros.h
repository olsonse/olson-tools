#ifdef DUMMY_IFDEF_FOR_FORTRAN
// $Id: fmacros.h,v 1.1 2005/05/12 04:27:31 olsonse Exp $
/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2004-2008 Spencer Olson
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
/*
 * $Log: fmacros.h,v $
 * Revision 1.1  2005/05/12 04:27:31  olsonse
 * Fixed to for Intel 8.1 compilers.
 * Found (using intel compiler) and fixed an array overflow in BField::potential.
 * Didn't find it earlier because the array is on the stack for the function.
 *
 * Added fmacros.h file to simplify mixing fortran code with others.
 * Added alias function names for Fortran interoperability.
 * Took it back out.
 *
 *
 */
#endif

/** \file
 * Some helpful macros to use in fortran files.
 * Copyright 2004-2008 Spencer Olson.
 */

#ifndef FMACROS_H
#define FMACROS_H


#endif /* FMACROS_H */
