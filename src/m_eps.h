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

#ifdef __cplusplus
namespace olson_tools {
extern "C" {
#endif 

    double getMachineEPS ();
    static double M_EPS = getMachineEPS();

#ifdef __cplusplus
}
} /*namespace olson_tools*/
#endif 

#endif // M_EPS_H
