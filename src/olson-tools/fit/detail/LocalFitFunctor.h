/** \file
 * A local fit functor--used for threaded local fitting if USE_PTHREAD is true. 
 *
 * Copyright 2006 Spencer Olson
 *
 */

#ifndef olson_tools_fit_detail_LocalFitFunctor_h
#define olson_tools_fit_detail_LocalFitFunctor_h

#include <olson-tools/fit/Individual.h>

namespace olson_tools {
  namespace fit {
    namespace detail {

      /** Creates merit evaluation tasks that are then executed by
       * the default thread cache.
       */
      template < typename LocalFit >
      struct LocalFitFunctor {
        /* MEMBER STORAGE */
        /** Local fit instance. */
        LocalFit localFit;

        /** Local fit parameters. */
        const typename LocalFit::Parameters & params;

        /** reference to the relevant child. */
        Individual * ind;


        /* MEMBER FUNCTIONS */
        /** Constructor. */
        LocalFitFunctor( const LocalFit & localFit,
                         const typename LocalFit::Parameters & params,
                         Individual * ind )
          : localFit(localFit), params(params), ind(ind) { }

        /** Functor operation. */
        void operator() () {
          localFit( ind, params );
        }
      };

    } /* namespace olson_tools::fit::detail */
  } /* namespace olson_tools::fit */
} /* namespace olson_tools */

#endif // olson_tools_fit_detail_LocalFitFunctor_h
