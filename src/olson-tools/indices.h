// -*- c++ -*-
// $Id$
/*
 * Copyright 2005-2006 Spencer Olson
 *
 * $Log$
 *
 */

/** \file
 * Definitions of X, Y, Z, RHO, PHI, VX, VY, and VZ macros.
 * Copyright 2005-2006 Spencer Olson.
 */

#ifndef INDICES_H
#define INDICES_H

#ifdef __cplusplus
namespace olson_tools { namespace indices {
#endif


#ifndef X
#  ifndef __cplusplus
#    define X 0
#  else
    const int X  = 0;
#  endif
#elif defined(X) && (X != 0)
#  error X already defined and it is not == 0!
#endif

#ifndef Y
#  ifndef __cplusplus
#    define Y 1
#  else
    const int Y  = 1;
#  endif
#elif defined(Y) && (Y != 1)
#  error Y already defined and it is not == 1!
#endif

#ifndef Z
#  ifndef __cplusplus
#    define Z 2
#  else
    const int Z  = 2;
#  endif
#elif defined(Z) && (Z != 2)
#  error Z already defined and it is not == 2!
#endif

#ifndef VX
#  ifndef __cplusplus
#    define VX 3
#  else
    const int VX  = 3;
#  endif
#elif defined(VX) && (VX != 3)
#  error VX already defined and it is not == 3!
#endif

#ifndef VY
#  ifndef __cplusplus
#    define VY 4
#  else
    const int VY  = 4;
#  endif
#elif defined(VY) && (VY != 4)
#  error VY already defined and it is not == 4!
#endif

#ifndef VZ
#  ifndef __cplusplus
#    define VZ 5
#  else
    const int VZ  = 5;
#  endif
#elif defined(VZ) && (VZ != 5)
#  error VZ already defined and it is not == 5!
#endif

#ifndef RHO
#  ifndef __cplusplus
#    define RHO X
#  else
    const int RHO  = X;
#  endif
#elif defined(RHO) && (RHO != X)
#  error RHO already defined and it is not == X!
#endif

#ifndef PHI
#  ifndef __cplusplus
#    define PHI Y
#  else
    const int PHI  = Y;
#  endif
#elif defined(PHI) && (PHI != Y)
#  error PHI already defined and it is not == Y!
#endif



#ifdef __cplusplus
}} /* namespace olson_tools::indices */
#endif

#endif // INDICES_H
