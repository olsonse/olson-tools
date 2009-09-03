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

#include <string>
#include <limits>

namespace olson_tools {
  namespace fit {

    ///The args for GeneticAlg class.
    template < typename LocalFitT = NullLocalFit >
    struct make_options {
      struct type {
        typedef LocalFitT LocalFit;

        ///
        type()
          : population(400),
            tolerance(1e-3),
            max_merit(std::numeric_limits<merit_t>::infinity()),
            replace(0.9),
            local_fit_max_individuals_prctage(0),
            crossprob(0.85),
            mutprob(0.35),
            maxgeneration(200),
            encourage_diversity(false),
            diversity_grid_cols(100),
            seed_fraction(-1),
            createind(create_Individual),
            meritfnc(NULL),
            exterior_pointer( NULL ),
            generation_delay(0),
            localParam()
          {}

        /** Number of individuals to use during for a generation. */
        int      population;
        float    tolerance;
        merit_t  max_merit;
        /** Fraction of each generation to be replaced(.5). */
        float    replace;
        /** Percentage of population to use fitting algorithm on. */
        float    local_fit_max_individuals_prctage;
        /** Crossover probability(.75). */
        float    crossprob;
        /** Mutation probability(.05). */
        float    mutprob;
        int      maxgeneration;

        /** Introduce a need to maintain diversity by competition for similar
         * Alleles.
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

        /** Local fit parameters. */
        typename LocalFit::Parameters localParam;

      };/* type */

    };/* make_options */

  }/*namespace olson_tools::fit */
}/*namespace olson_tools */

#endif //  GENETICALGARGS_H
