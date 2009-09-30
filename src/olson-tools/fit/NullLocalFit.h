// -*- c++ -*-
// $Id$
/*
 * Copyright 199?-1998 Robert Steven Turley --- All Rights Reserved
 * Copyright 1998-2005 Spencer Eugene Olson --- All Rights Reserved
 *
 * $Log: Generation.h,v $
 * Revision 1.1  2005/06/14 02:28:19  olsonse
 * Split out some files and renamed to be more consistent with olson-tools.
 *
 * Revision 1.1.1.1  2005/01/08 04:27:25  olsonse
 * Initial import
 *
 * Revision 1.6  2000/05/06 20:37:18  olsons
 * Reorganized libfit: made it self-containing and use new definition of alleles.
 *
 * Revision 1.5  1999/10/18 18:56:34  olsons
 * Added documentation in comments to be parsed by doc++.
 *
 * Revision 1.4  1999/08/18 13:49:39  olsons
 * Fixed extra 'static class::freetoheap' declaration in memory.
 * Added an escape function to generation and geneticalg so
 * that the algorithm can be interrupted.
 *
 * Revision 1.3  1999/07/28 00:32:08  olsons
 * fixed and re-enabled class::freetoheap so that we can free the memory to the heap again.
 * found a bug and squashed it in simfit (it was in checking for minvals[] and maxvals[] -- my addition to simfit)
 *
 * Revision 1.2  1999/07/03 05:27:04  olsons
 * Genetic Algorithm is now a seperate entity from XUVMulti and is written so it
 * can be used for any purpose.
 * libgeneticalg.so was also added.
 *
 * Revision 1.1.1.1  1999/06/04 13:47:54  olsons
 * Imported as XUVMulti into cvs
 *
 * Revision 1.1.1.1  1998/10/03 16:07:52  olsons
 * Imported Genetic Algorithm
 *
 * Revision 1.5  1997/12/29  16:50:34  turley
 * using angular averages and file io
 *
 * Revision 1.5  1997/12/29  16:50:34  turley
 * using angular averages and file io
 *
 * Revision 1.4  1997/12/19  22:10:48  turley
 * added true tournament selection
 *
 * Revision 1.3  1997/12/17  18:51:26  turley
 * fixed bug, added histogram and resource allocation for diversity
 *
 * Revision 1.2  1997/12/10  22:15:41  turley
 * bug fixes
 *
 * Revision 1.1  1997/12/02  20:04:42  turley
 * Initial revision
 *
*/


/** \file
 * Definition of a Gene Simplex wrapper for the genetic algorithm code.
 *
 * Copyright 199?-1998 Robert Steven Turley --- All Rights Reserved
 *
 * Copyright 1998-2005 Spencer Eugene Olson --- All Rights Reserved
 *
 */


#ifndef olson_tools_fit_NullLocalFit_h
#define olson_tools_fit_NullLocalFit_h

#include <olson-tools/fit/Individual.h>

namespace olson_tools{
  namespace fit {

    /** This is an implemented class of the simplex algorithm
     * which will nicely find the local extrema of the
     * merit function which will be passed in through the Individual's
     * Gene.
     *@memo Simplex for Genes.
    */
    struct NullLocalFit {
      /* TYPEDEFS */
    public:
      struct Parameters { };

      /* MEMBER FUNCTIONS */
    public:
      /** simplex_fit(...) is a wrapper for the Gene\_Simplex.  It will create
       * a Gene\_Simplex and pass to it all pertinant arrays and information.
       * Perhaps we should combine this function into the constructor
       * of the Gene\_Simplex.
       *@memo Runs simplex algorithm on an Individual.
      */
      template < typename IndividualT >
      void fit( IndividualT * member, const Parameters & param ) { }

    }; // NullLocalFit class


  }/*namespace olson_tools::fit */
}/*namespace olson_tools */

#endif // olson_tools_fit_NullLocalFit_h
