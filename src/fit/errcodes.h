// -*- c++ -*-
// $Id: errcodes.h,v 1.1 2005/01/08 04:27:26 olsonse Exp $
/*
 * $Log: errcodes.h,v $
 * Revision 1.1  2005/01/08 04:27:26  olsonse
 * Initial revision
 *
 * Revision 1.1  2000/06/15 19:38:56  olsons
 * Added this file as a part of the redef of debug_level.
 *
 *
 */

#ifndef ERRCODES_H
#define ERRCODES_H

const int LIBFIT_ERRCODE_LEN = 100;

typedef char LIBFIT_ERRCODE [ LIBFIT_ERRCODE_LEN ];

const LIBFIT_ERRCODE CROSSOVER_MISMATCH = "crossover:  Number of alleles of both chromosomes does not match.";
const LIBFIT_ERRCODE CHROMOSOME_MUTATE_TOO_MANY_INVALID_LOOPS = "mutate:  Too many invalid loops.";
const LIBFIT_ERRCODE GENE_RANDGENE_NO_DYNAMIC_ALLELES = "Gene::randgene(): Too many iterations.  Nothing can be modified";
const LIBFIT_ERRCODE GENE_ALLELE_UNDERRUN = "Allele_t &Gene[]: Allele index too low.";
const LIBFIT_ERRCODE GENE_ALLELE_OVERRUN = "Allele_t &Gene[]: Allele index too high.";
const LIBFIT_ERRCODE GENE_ALLELE_OUT_OF_INDEX = "Gene::Allele(): Allele index invalid";
const LIBFIT_ERRCODE EMPTY_DYNAMIC_DNA = "DNA has zero dynamic alleles(bad!).";

#endif //ERRCODES_H

