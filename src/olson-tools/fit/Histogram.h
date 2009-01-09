// -*- c++ -*-
// $Id: Histogram.h,v 1.1 2005/06/14 02:28:19 olsonse Exp $
/*
 * Copyright 199?-1998 Robert Steven Turley --- All Rights Reserved
 * Copyright 1998-2005 Spencer Eugene Olson --- All Rights Reserved
 *
 * $Log: Histogram.h,v $
 * Revision 1.1  2005/06/14 02:28:19  olsonse
 * Split out some files and renamed to be more consistent with olson-tools.
 *
 *
*/

/** \file
 * Histogramming function to heighten competition for resources in genetic algorithm code.
 *
 * Copyright 199?-1998 Robert Steven Turley --- All Rights Reserved
 *
 * Copyright 1998-2005 Spencer Eugene Olson --- All Rights Reserved
 *
 */


#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "Individual.h"

namespace olson_tools{ namespace fit {

///
class Histogram{
public:
  ///
  Histogram(int population, int cont_grid_cols, const Individual &);
  ///
  ~Histogram();
  /// return 0 if no error, <>0 otherwise
  void update(Individual& member);
  /// return merit factor
  merit_t meritfact(Individual& member);
  ///
  friend std::ostream & operator<<(std::ostream &,const Histogram &);
private:
  ///
  int **histtable;///pointer to an array of accumulation arrays

  /** refers to grid spacing of histogram for coordinate system gene. */
  float grid_spacing;

  int rows;
  int *colsize;

  /** factor to coerce smaller populations to have larger diversity. */
  merit_t mfact;
}; // Histogram


///The Histogram print function.
std::ostream & operator<<(std::ostream &, const Histogram &);

}}/*namespace olson_tools::fit */

#endif // HISTOGRAM_H
