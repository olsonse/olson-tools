// -*- c++ -*-
// $Id$
/*
 * Copyright 1998-2005 Spencer Eugene Olson --- All Rights Reserved
 *
 * $Log: GeneticAlgArgs.h,v $
 * Revision 1.1  2005/06/14 02:28:19  olsonse
 * Split out some files and renamed to be more consistent with olson-tools.
 *
 *
 */

/** \file
 * Collection (into a class) of arguments of the genetic algorithm code.
 *
 * Copyright 1998-2005 Spencer Eugene Olson --- All Rights Reserved
 *
 */



#ifndef GENETICALGARGS_H
#define GENETICALGARGS_H

#include <olson-tools/fit/merit_def.h>
#include <olson-tools/fit/Individual.h>
#include <olson-tools/fit/NullLocalFit.h>

#include <string>
#include <limits>

namespace olson_tools {
  namespace fit {

    /** Options for genetic algorithm.
     * @param _LocalFit
     *    FUnctor that will provide the local fit [Default NullLocalFIt].
     */
    template <
               typename _LocalFit = NullLocalFit
    >
    struct make_options {
      struct type {
        /* TYPEDEFS */
        typedef _LocalFit LocalFit;

        ///
        type()
          : population(400),
            tolerance(1e-3),
            max_merit(std::numeric_limits<merit_t>::infinity()),
            replace(0.6),
            crossprob(0.8),
            mutprob(0.3),
            max_generation(200),
            encourage_diversity(true),
            diversity_grid_cols(100),
            seed_fraction(-1),
            createind(create_Individual),
            meritfnc(NULL),
            exterior_pointer( NULL ),
            generation_delay(0),
            local_fit_max_individuals_prctage(0),
            local_fit_random(false),
            localParam()
        #ifdef USE_PTHREAD
            ,num_pthreads(-1)
        #endif
          {}

        /** Number of individuals to use during for a generation. */
        int      population;
        float    tolerance;
        merit_t  max_merit;
        /** Fraction of each generation to be replaced(.6). */
        float    replace;
        /** Crossover probability(.8). */
        float    crossprob;
        /** Mutation probability(.3). */
        float    mutprob;
        int      max_generation;

        /** Introduce a need to maintain diversity by competition for similar
         * Alleles [Default true].
         */
        bool     encourage_diversity;

        /** Number of different values to diversify amongst for continuous
         * parameters. */
        int      diversity_grid_cols;

        /** The fraction of the population is seeded by the initial gene. */
        float    seed_fraction;

        /// Creator function for the Individuals.
        CREATE_IND_FUNC   createind;
        void *   meritfnc;
        void *   exterior_pointer;

        /** Wait after each generation (in fractions of seconds).
         * -1 means to wait for user input. */
        double generation_delay;


        /* PARAMETERS RELATED TO LOCAL FIT */
        /** Percentage of population to use fitting algorithm on. */
        float    local_fit_max_individuals_prctage;

        /** Choice to do local fit randomly through the population (otherwise
         * locally fit only the top member of each generation) [Default false].
         */
        bool local_fit_random;

        /** Local fit parameters. */
        typename LocalFit::Parameters localParam;

        #ifdef USE_PTHREAD
        /** Number of threads to use for evaluating merit functions and
         * performing fits [Default -1].  This value will only be used if >= 1.
         * */
        int num_pthreads;
        #endif

      };/* type */

    };/* make_options */

  }/*namespace olson_tools::fit */
}/*namespace olson_tools */

#endif //  GENETICALGARGS_H
