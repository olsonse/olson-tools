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

#ifndef X
#  define X 0
#elif defined(X) && (X != 0)
#  error X already defined and it is not == 0!
#endif

#ifndef Y
#  define Y 1
#elif defined(Y) && (Y != 1)
#  error Y already defined and it is not == 1!
#endif

#ifndef Z
#  define Z 2
#elif defined(Z) && (Z != 2)
#  error Z already defined and it is not == 2!
#endif

#ifndef VX
#  define VX 3
#elif defined(VX) && (VX != 3)
#  error VX already defined and it is not == 3!
#endif

#ifndef VY
#  define VY 4
#elif defined(VY) && (VY != 4)
#  error VY already defined and it is not == 4!
#endif

#ifndef VZ
#  define VZ 5
#elif defined(VZ) && (VZ != 5)
#  error VZ already defined and it is not == 5!
#endif

#ifndef RHO
#  define RHO X
#elif defined(RHO) && (RHO != X)
#  error RHO already defined and it is not == X!
#endif

#ifndef PHI
#  define PHI Y
#elif defined(PHI) && (PHI != Y)
#  error PHI already defined and it is not == Y!
#endif

#endif // INDICES_H
