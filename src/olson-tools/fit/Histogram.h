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


#ifndef olson_tools_fit_Histogram_h
#define olson_tools_fit_Histogram_h

#include <olson-tools/fit/Individual.h>

namespace olson_tools{
  namespace fit {

    ///
    class Histogram {
      /* MEMBER STORAGE */
    private:
      ///
      int **histtable;///pointer to an array of accumulation arrays

      /** refers to grid spacing of histogram for coordinate system gene. */
      float grid_spacing;

      int rows;
      int *colsize;

      /** factor to coerce smaller populations to have larger diversity. */
      merit_t mfact;

      /* MEMBER FUNCTIONS */
    public:
      ///
      template < typename MF >
      inline Histogram(int population, int cont_grid_cols, const Individual<MF> &);

      ///
      inline ~Histogram();

      /// return 0 if no error, <>0 otherwise
      template < typename MF >
      inline void update(Individual<MF> & member);

      /// return merit factor
      template < typename MF >
      inline merit_t meritfact(Individual<MF> & member);

      ///
      friend inline std::ostream & operator<<(std::ostream &,const Histogram &);
    }; // Histogram


    ///The Histogram print function.
    inline std::ostream & operator<<(std::ostream &, const Histogram &);

  }/*namespace olson_tools */
}/*namespace olson_tools::fit */

#include <olson-tools/fit/Histogram.cpp>

#endif // olson_tools_fit_Histogram_h
