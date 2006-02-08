// -*- c++ -*-
// $Id: GeneticAlgArgs.C,v 1.1 2005/06/14 02:28:19 olsonse Exp $
/*
 * Copyright 1998-2005 Spencer Eugene Olson --- All Rights Reserved
 *
 * $Log: GeneticAlgArgs.C,v $
 * Revision 1.1  2005/06/14 02:28:19  olsonse
 * Split out some files and renamed to be more consistent with olson-tools.
 *
 *
 */

#include "GeneticAlgArgs.h"

void GeneticAlgArgs::setParam (const std::string & name, double & var) {
	if (name == "population")
		population = (int)var;
	else if (name == "tolerance")
		tolerance = var;
	else if (name == "max_merit")
		max_merit = var;
	else if (name == "replace")
		replace = var;
	else if (name == "local_fit_max_individuals_prctage")
		local_fit_max_individuals_prctage = var;
	else if (name == "local_fit_tolerance")
		local_fit_tolerance = var;
	else if (name == "local_fit_max_iteration")
		local_fit_max_iteration = (int)var;
	else if (name == "crossprob")
		crossprob = var;
	else if (name == "mutprob")
		mutprob = var;
	else if (name == "maxgeneration")
		maxgeneration = (int)var;
	else if (name == "encourage_diversity")
		encourage_diversity = (int)var;

	else if (name == "diversity_grid_cols")
		diversity_grid_cols = (int)var;

	else if (name == "seed_fraction")
		seed_fraction = (int)var;
}
