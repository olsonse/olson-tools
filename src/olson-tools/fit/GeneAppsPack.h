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
 * Definition of a Gene APPSPACK wrapper for the genetic algorithm code.
 *
 * Copyright 199?-1998 Robert Steven Turley --- All Rights Reserved
 *
 * Copyright 1998-2005 Spencer Eugene Olson --- All Rights Reserved
 *
 */


#ifndef olson_tools_fit_GeneAppsPack_h
#define olson_tools_fit_GeneAppsPack_h

#include <olson-tools/fit/Individual.h>
#include <olson-tools/fit/detail/GeneMinFunc.h>
#include <olson-tools/fit/appspack/ThreadedExecutor.hpp> // <-- Provides user-defined custom executor
#include <olson-tools/ompexcept.h>

#include <appspack/APPSPACK_Parameter_List.hpp> // <-- Provides APPSPACK::Vector 
#include <appspack/APPSPACK_Vector.hpp>   // <-- Provides APPSPACK::Vector 
#include <appspack/APPSPACK_Solver.hpp>   // <-- Provides APPSPACK::Vector 
#include <appspack/APPSPACK_Constraints_Linear.hpp>      // <-- Provides APPSPACK::Constraints::Linear

#include <stdexcept>

namespace olson_tools{
  namespace fit {

    /** This is an implemented class of the simplex algorithm
     * which will nicely find the local extrema of the
     * merit function which will be passed in through the Individual's
     * Gene.
     *@memo AppsPack for Genes.
    */
    class GeneAppsPack {
      /* TYPEDEFS */
    public:
      struct Parameters : APPSPACK::Parameter::List {
        Parameters() {
          sublist("Solver").setParameter("Debug", static_cast<int>(3));
          sublist("Solver").setParameter("Initial Step", static_cast<double>(0.25));
          sublist("Solver").setParameter("Step Tolerance", static_cast<double>(1e-3));
          sublist("Solver").setParameter("Bounds Tolerance", static_cast<double>(1e-3));
          //sublist("Solver").setParameter("Cache Output File", std::string("eval_output"));
          //sublist("Solver").setParameter("Cache Input File", std::string("eval_output"));
        }

        Parameters( const Parameters & p )
          : APPSPACK::Parameter::List(p) { }
      };

      /* MEMBER FUNCTIONS */
    public:
      /**  Runs simplex algorithm on an Individual.  */
      merit_t operator() ( Individual * member,
                           const Parameters & param ) const {
        /* make a copy of the parameters to work with */
        Parameters lparam(param);

        int nalleles = member->DNA.numAlleles( ALLELE_DYNAMIC_CONT );

        if(!nalleles)
          THROW( std::runtime_error,
                 "GeneAppsPack::operator(): DNA has zero dynamic alleles(bad!)."
          );

        // *** Create the MinFunc
        detail::GeneMinFunc minFunc(member);

        // *** get initial point and bounds
        APPSPACK::Vector initialx, minvals, maxvals;
        for ( int i = 0; i < nalleles; ++i ) {
          initialx.push_back( member->DNA.Allele( i, ALLELE_DYNAMIC_CONT ).val );
          minvals .push_back( member->DNA.Allele( i, ALLELE_DYNAMIC_CONT ).min );
          maxvals .push_back( member->DNA.Allele( i, ALLELE_DYNAMIC_CONT ).max );
        }//for

        // *** get the initial function value.
        merit_t bmerit = member->Merit();
        APPSPACK::Vector initialf(1, -bmerit );

        // *** set the initial values.
        lparam.sublist("Solver").setParameter("Initial X", initialx);
        lparam.sublist("Solver").setParameter("Initial F", initialf);
        lparam.sublist("Linear").setParameter("Lower", minvals);
        lparam.sublist("Linear").setParameter("Upper", maxvals);

        // *** Instantiate the bounds ***
        APPSPACK::Constraints::Linear linear(lparam.sublist("Linear"));

        // *** Instantiate the custom executor ***
        typedef olson_tools
              ::fit::appspack::ThreadedExecutor<detail::GeneMinFunc> Executor;
        Executor executor(minFunc);
        
        // *** Create the solver ***
        APPSPACK::Solver solver(lparam.sublist("Solver"), executor, linear);
        
        // *** Run the solver ***
        /* APPSPACK::Solver::State state = */ solver.solve();
  

        /* get results. */
        std::vector<double> bestX = solver.getBestX();
        merit_t amerit = -solver.getBestF();

        if( bmerit > amerit ) {//compare to see if local minimizer worked
          #ifdef DEBUG
          if ( debug_level == GA )
            gaerr<<"GeneAppsPack: AppsPack went goofy\n"
                 <<"\tmerit before= "<<bmerit
                 <<"\n\tmerit after= "<<amerit<<std::endl;
          #endif //DEBUG
        } else { //only regene if AppsPack helped the design
          for ( int i = 0; i < nalleles; ++i )
            member->DNA.Allele( i, ALLELE_DYNAMIC_CONT ).val = bestX[i];

          member->setMerit( amerit );
        }

        return amerit;
      }

    }; // GeneAppsPack class


  }/*namespace olson_tools::fit */
}/*namespace olson_tools */

#endif // olson_tools_fit_GeneAppsPack_h
