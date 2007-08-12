// -*- c++ -*-
// $Id$
/*
 * Copyright 2004-2007 Spencer E. Olson --- All Rights Reserved
 */

/** \file
 * Machine precision calculation.
 *
 * Copyright 2004-2007 Spencer E. Olson --- All Rights Reserved
 */


#ifndef M_EPS_H
#define M_EPS_H


extern "C" {
    double getMachineEPS ();
    static double M_EPS = getMachineEPS();
}

#endif // M_EPS_H
