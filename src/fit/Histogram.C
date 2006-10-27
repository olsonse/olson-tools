// -*- c++ -*-
// $Id: Generation.h,v 1.1 2005/06/14 02:28:19 olsonse Exp $
/**
 * Definition of a Gene for the genetic algorithm code.
 *
 * Copyright 199?-1998 Robert Steven Turley --- All Rights Reserved
 *
 * Copyright 1998-2005 Spencer Eugene Olson --- All Rights Reserved
 *
 */

/*
 * Copyright 199?-1998 Robert Steven Turley --- All Rights Reserved
 * Copyright 1998-2005 Spencer Eugene Olson --- All Rights Reserved
 */


#include "Histogram.h"
#include <stdexcept>
#include <math.h>
#include "../ompexcept.h"

Histogram::Histogram(int population, int cont_grid_cols, const Individual & member):
    grid_spacing(0.0), mfact(1.0 - 1.0/sqrt(float(population))) {
    // allocate some space for the accumulation arrays
    if(!(rows = member.DNA.numAlleles(ALLELE_DYNAMIC))) {
      THROW(std::runtime_error,"DNA has zero dynamic alleles(bad!).");
    }
    histtable = new int*[rows];
    colsize = new int[rows];
    // create and initialize accumulation arrays to zero
    for(int i=0;i<rows;i++) {
        int cols = member.DNA.Allele( i, ALLELE_DYNAMIC ).allele_type;
        if(cols==ALLELE_DYNAMIC_CONT) {
            cols = cont_grid_cols;
            grid_spacing = (  member.DNA.Allele( i, ALLELE_DYNAMIC ).max
                            - member.DNA.Allele( i, ALLELE_DYNAMIC ).min )
                         / cols;
        } else {
            cols = int(member.DNA.Allele( i, ALLELE_DYNAMIC ).max
                 - member.DNA.Allele( i, ALLELE_DYNAMIC ).min );
        }
        colsize[i] = cols;
        histtable[i] = new int[cols];//create and initialize to zero
        for(int j=0;j<cols;histtable[i][j++]=0); // initialize to zero
    } // for -- create and init accuml. arrays
} // Histogram constructor

Histogram::~Histogram() {
  for(int i = 0; i<rows; delete[] histtable[i++]);
  delete[] histtable;
  delete[] colsize;
} // Histogram destructor

merit_t Histogram::meritfact(Individual& member){
    // Compute a merit factor based on competition for resources.  In
    // other words, if this member uses resources that are also used a
    // lot by others return a lower number than otherwise.  This should
    // put pressure on the population to increase diversity.
    merit_t factor=1;
    //  Go through the accuml. arrays and get a total factor
    // based upon how many total members use the same genes as this member
    for(int i=0; i<rows; i++) {
        // now we need to find the second index
        /*  Get the exact alleletype. */
        int j = member.DNA.Allele( i, ALLELE_DYNAMIC ).allele_type;

        // in the following, we shouldn't need to '% colsize', but just in case

        if( j == ALLELE_DYNAMIC_CONT) {//if match, set j to second index
            j = int(
                      (member.DNA.Allele( i, ALLELE_DYNAMIC ).max - member.DNA.Allele( i, ALLELE_DYNAMIC ).val)
                      / grid_spacing
                   ) % colsize[i];
        } else {
            j = int(   member.DNA.Allele( i, ALLELE_DYNAMIC ).max
                     - member.DNA.Allele( i, ALLELE_DYNAMIC ).val
                   ) % colsize[i];
        }
        factor /= histtable[i][j];
    }
    return pow(mfact, factor);
} // Histogram::meritfact

void Histogram::update(Individual& member) {
    // Go through the accumul. arrays and accumulate! (histogram this guy's gene)
    for(int i=0; i<rows; i++) {
        int j = member.DNA.Allele( i, ALLELE_DYNAMIC).allele_type;
        // in the following, we shouldn't need to '% colsize', but just in case
        if(j==ALLELE_DYNAMIC_CONT) {
            j = int(
                      (member.DNA.Allele( i, ALLELE_DYNAMIC ).max - member.DNA.Allele( i, ALLELE_DYNAMIC ).val)
                      / grid_spacing
                   ) % colsize[i];
        } else {
            j = int(   member.DNA.Allele( i, ALLELE_DYNAMIC ).max
                     - member.DNA.Allele( i, ALLELE_DYNAMIC ).val
                   ) % colsize[i];
        }
        histtable[i][j]++;
    }
} // Histogram::update


std::ostream & operator<<(std::ostream &output, const Histogram & hist) {
  // display accumulated histogram
  output<<"Histogram\n";
  //for(int i=0; i<20; i++)
  output<<std::endl;
  return output;
} // operator<< Histogram

