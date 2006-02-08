// -*- c++ -*-
// $Id: GeneticAlgArgs.h,v 1.1 2005/06/14 02:28:19 olsonse Exp $
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

#include <string>
#include "merit_def.h"
#include "Individual.h"

///The args for GeneticAlg class.
class GeneticAlgArgs {
  public:
    ///
    GeneticAlgArgs():population(400), tolerance(1e-3), max_merit(1e100), replace(0.9),
                     local_fit_max_individuals_prctage(0),
                     local_fit_tolerance(1e-3),
                     local_fit_max_iteration(100), crossprob(0.85),
                     mutprob(0.35), maxgeneration(200),
                     encourage_diversity(false),
                     diversity_grid_cols(100),
                     seed_fraction(-1),
                     createind(create_Individual), meritfnc(NULL),
                     exterior_pointer( NULL )
                     {}
    /// Genetic Algorithm arguments copy constructor.
    GeneticAlgArgs(const GeneticAlgArgs & that) { *this = that; }
    ~GeneticAlgArgs() {}

    void setParam (const std::string & name, double & var);
    /**The actual variable arguments for the Genetic Algorithm.
    */
    //@{
    int      population;
    float    tolerance;
    merit_t  max_merit;
    float    replace;
    float    local_fit_max_individuals_prctage;
    float    local_fit_tolerance;
    int      local_fit_max_iteration;
    float    crossprob;
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

    CREATE_IND_FUNC   createind;
    void *   meritfnc;
    void *   exterior_pointer;
    //@}

};//class GeneticAlgArgs

#endif //  GENETICALGARGS_H

