// $Id$ 
// $Source$ 

//@HEADER
// ************************************************************************
// 
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//                                                                                 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.                                                                           .
// 
// Questions? Contact Tammy Kolda (tgkolda@sandia.gov) 
// 
// ************************************************************************
//@HEADER

/*! \file SharedCacheManager.hpp
    \brief Class definition for olson_tools::fit::appspack::SharedCacheManager
*/

#ifndef olson_tools_fit_appspack_SharedCacheManager_hpp
#define olson_tools_fit_appspack_SharedCacheManager_hpp

#include <olson-tools/SyncLock.h>

#include <appspack/APPSPACK_Cache_Manager.hpp>
#include <appspack/APPSPACK_Parameter_List.hpp>
#include <appspack/APPSPACK_Vector.hpp>

#include <iostream>

namespace olson_tools {
  namespace fit {
    namespace appspack {

      /** Wrapper for cache manager class to provide shared access to a single
       * real implementation underneath.
       *
       * @param SingleCahceManager
       *    Type of cache manager used underneath the hood.
       *
       * @param id
       *    Id of SharedCacheManager class allows the programmer to create
       *    multiple shared managers with their own copies of the same
       *    underlying cache manager class. 
       */
      template < typename SingleCacheManager = APPSPACK::Cache::Manager,
                 unsigned int id = 0 >
      class SharedCacheManager {
        /* TYPEDEFS */
      private:
        typedef APPSPACK::Parameter::List ParamList;
        typedef APPSPACK::Vector Vector;
        /** Auto unlocking syncronize object. */
        typedef olson_tools::Synchronize<SharedCacheManager> Sync;

        /* STATIC STORAGE */
      private:
        static SingleCacheManager * impl;
        static ParamList usedParams;
        static Vector    usedScaling;


        /* STATIC FUNCTIONS */
      public:
        /** Creates the shared cache using the given parameters and scaling.
         * Subsequent calls to createCache will not do anything except to check
         * that the scaling is the same.
         */
        static void createCache( ParamList & params, const Vector & scaling ) {
          Sync sync;/* auto unlocking syncronize object. */
          if (impl) {
            if ( scaling != usedScaling )
              std::cerr << "one of the users of the shared cache "
                           "requires a different scaling"
                        << std::endl;
          } else {
            usedScaling = scaling;
            usedParams = params;
            impl = new SingleCacheManager( params, scaling );
          }
        }

        static void destroyCache() {
          Sync sync;/* auto unlocking syncronize object. */
          if ( impl ) {
            delete impl;
            impl = NULL;
          }
        }


        /* MEMBER FUNCTIONS */
      public:
        /** This Constructor creates a shared cache if it has not already been
         * created.
         *
         * @see createCache.
         */
        SharedCacheManager(ParamList & params, const Vector & scaling) {
          createCache(params, scaling);
        }

        /** This Destructor DOES NOTHING.  User needs to delete the underlying
         * cache explicitly by calling detroyCache. */
        ~SharedCacheManager() { }

        //! Add the given point to the cache.
        bool insert(const Vector & x, const Vector & f) {
          Sync sync;/* auto unlocking syncronize object. */
          return impl->insert( x, f );
        }
        
        //! Return true if x is cached and fill in the function value.
        bool isCached(const Vector & x, Vector & f) {
          Sync sync;/* auto unlocking syncronize object. */
          return impl->isCached( x, f );
        }

      };

      template < typename sing, unsigned int id >
      sing * SharedCacheManager<sing,id>::impl = NULL;

      template < typename sing, unsigned int id >
      APPSPACK::Parameter::List SharedCacheManager<sing,id>::usedParams;

      template < typename sing, unsigned int id >
      APPSPACK::Vector SharedCacheManager<sing,id>::usedScaling;

    } /* namespace olson_tools::fit::appspack */
  } /* namespace olson_tools::fit */
} /* namespace olson_tools */

#endif // olson_tools_fit_appspack_SharedCacheManager_hpp
