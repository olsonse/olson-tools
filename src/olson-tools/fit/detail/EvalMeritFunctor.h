/** \file
 * A merit evaluation functor--used for threaded evaluation if USE_PTHREAD is
 * true. 
 *
 * Copyright 2006 Spencer Olson
 *
 */

#ifndef olson_tools_fit_detail_EvalMeritFunctor_h
#define olson_tools_fit_detail_EvalMeritFunctor_h

#include <olson-tools/fit/Individual.h>

namespace olson_tools {
  namespace fit {
    namespace detail {

      /** Creates merit evaluation tasks that are then executed by
       * the default thread cache.
       */
      struct EvalMeritFunctor {
        /** reference to the relevant child. */
        Individual & ind;

        EvalMeritFunctor( Individual & ind ) : ind(ind) { }

        void operator() () {
          static_cast<void>( ind.Merit() );
        }
      };

    } /* namespace olson_tools::fit::detail */
  } /* namespace olson_tools::fit */
} /* namespace olson_tools */

#endif // olson_tools_fit_detail_EvalMeritFunctor_h
