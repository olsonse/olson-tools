
#include <olson-tools/fit/GeneticAlg.h>
#include <olson-tools/fit/GeneSimplex.h>
#include <olson-tools/fit/make_options.h>
#include <iostream>
#include <fstream>
#include <cmath>

using olson_tools::fit::Allele_struct;
using olson_tools::fit::ALLELE_DYNAMIC_CONT;
using olson_tools::fit::Gene;
using olson_tools::fit::Individual;
using olson_tools::fit::GeneticAlg;
using olson_tools::fit::GeneSimplex;
using olson_tools::fit::make_options;

#define A(a,b,c)       Allele_struct((a),(b),(c), ALLELE_DYNAMIC_CONT)

#ifndef N_GENES
#  define N_GENES 2
#endif

static int n_eval = 0;
#ifdef RECORD_EVALS
static std::ofstream frec("record");

#  define POPULATION                          10
#  define LOCAL_FIT_MAX_INDIVIDUALS_PRCTAGE  0.00
#  define REPLACE                            0.97
#else
#  define POPULATION                          10000
#  define LOCAL_FIT_MAX_INDIVIDUALS_PRCTAGE  0.0003
#  define REPLACE                            0.60
#endif

template < int scale = 1, int x0 = 0, int x1 = 0, int x2 = 0, int x3 = 0, int x4 = 0 >
struct sinc {
  merit_t operator() ( const Gene & gene ) {
    double rho = 0;

    for (int i = gene.numAlleles()-1; i>=0; --i) {
      double g = gene[i].val;
      switch (i) {
        case 0:  g += x0; break;
        case 1:  g += x1; break;
        case 2:  g += x2; break;
        case 3:  g += x3; break;
        case 4:  g += x4; break;
        default: break;
      }
      rho +=  g*g;
    }

    rho = scale * std::sqrt(rho);

    if (rho == 0)
      return 1.0;
    else
      return std::sin(rho)/rho;
  }
};

struct MeritFunctor {
  merit_t operator() ( const Gene & gene ) const {
    double m =         sinc<3, -2, 2>()(gene)
             + 100.8 * sinc<3,  4, 3>()(gene)
             + 2.4   * sinc<3,  0,-3>()(gene);

    ++n_eval;
  #ifdef RECORD_EVALS
    frec << gene << '\t' << m << '\n' << std::flush;
  #endif
    return m;
  }
};

int main() {
  typedef make_options< MeritFunctor, GeneSimplex<MeritFunctor> >::type options;
  options opts;
  opts.population = POPULATION;
  opts.local_fit_max_individuals_prctage = LOCAL_FIT_MAX_INDIVIDUALS_PRCTAGE;
  opts.localParam.tolerance = 0.0000000001;
  opts.tolerance = 1e-15;
  opts.encourage_diversity = true;
  opts.max_merit = 0.9;
  opts.replace = REPLACE;
  opts.max_generation = 1000;
  Gene dna;

  for ( int i = 0; i < N_GENES; ++i )
    dna += A( -10, 0, 10 );


  GeneticAlg<options> ga(dna, opts);

  ga.fit(&std::cout);
  Individual<MeritFunctor> indiv(dna);
  merit_t merit = indiv.Merit();

  std::cout << "global(/local) fit merit:  " <<  merit << "\t:  "
            << dna << std::endl;

  std::cout << "evals of merit function :  " << n_eval << std::endl;
  n_eval = 0;

  merit = GeneSimplex<MeritFunctor>() ( &indiv, opts.localParam );

  dna = indiv.DNA;

  std::cout << "local fit gives merit of:  " <<  merit << "\t:  "
            << dna << std::endl;
  std::cout << "evals of merit function :  " << n_eval << std::endl;
#ifdef RECORD_EVALS
  frec.close();
#endif


  {
    std::ofstream f("func.dat");
    MeritFunctor meritFunctor;

    for ( dna[0].val = -5; dna[0].val <= 5; dna[0].val += .1 ) {
      for ( dna[1].val = -5; dna[1].val <= 5; dna[1].val += .1 ) {
        double m = meritFunctor( dna );
        f << dna << '\t' << m << '\n';
      }
      f << '\n';
    }

    f.close();
  }

  return EXIT_SUCCESS;
}
