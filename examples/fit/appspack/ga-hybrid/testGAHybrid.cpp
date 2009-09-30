
#include <olson-tools/fit/GeneticAlg.h>
#include <olson-tools/fit/GeneAppsPack.h>
#include <olson-tools/fit/make_options.h>
#include <olson-tools/memory.h>

#include <iostream>
#include <fstream>
#include <cmath>

using olson_tools::fit::Allele_struct;
using olson_tools::fit::ALLELE_DYNAMIC_CONT;
using olson_tools::fit::Gene;
using olson_tools::fit::Individual;
using olson_tools::fit::GeneticAlg;
using olson_tools::fit::make_options;
typedef olson_tools::fit::GeneAppsPack<> GeneAppsPack;

#define A(a,b,c)       Allele_struct((a),(b),(c), ALLELE_DYNAMIC_CONT)

#ifndef N_GENES
#  define N_GENES 2
#endif

#define RECORD_EVALS

static int n_eval = 0;
#ifdef RECORD_EVALS
static std::ofstream frec("record");

# ifdef USE_PTHREAD
static olson_tools::MemGooLock memLock;
# endif

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
    #  ifdef USE_PTHREAD
    memLock.lock();
    #  endif
    frec << gene << '\t' << m << '\n' << std::flush;
    #  ifdef USE_PTHREAD
    memLock.unlock();
    #  endif
    #endif
    return m;
  }
};


int main() {
  typedef make_options< MeritFunctor, GeneAppsPack>::type options;
  options opts;
  opts.population = 10;
  opts.local_fit_max_individuals_prctage = 0.50;
  opts.tolerance = 1e-15;
  //opts.encourage_diversity = false;
  opts.max_merit = 50.0;
  opts.max_generation = 1000;
  opts.localParam.sublist("Solver").setParameter("Cache Output File", std::string("eval_output"));
  //opts.localParam.sublist("Solver").setParameter("Cache Input File", std::string("eval_output"));
  opts.localParam.sublist("Solver").setParameter("Debug", static_cast<int>(0));
  opts.localParam.sublist("Solver").setParameter("Step Tolerance", static_cast<double>(0.05));
  opts.localParam.sublist("Solver").setParameter("Cache Comparison Tolerance", static_cast<double>(0.01));
  opts.localParam.sublist("Solver").setParameter("Initial Step", static_cast<double>(0.50));
  //opts.replace = 0.60;
  //opts.mutprob = .3;
  //opts.crossprob = .8;


  Gene dna;

  for ( int i = 0; i < N_GENES; ++i )
    dna += A( -10, 0, 10 );


  GeneticAlg<options> ga(dna, opts);

  merit_t ga_merit = ga.fit(&std::cout);
  Individual<MeritFunctor> indiv(dna);
  merit_t merit = indiv.Merit();

  std::cout << "global(/local) fit merit:  " << merit << "\t:  "
            << dna << std::endl;

  if ( ga_merit > merit ) {
    std::cout << "NOTE:   ga returned " << ga_merit << " as merit "
                 "and re-evaluated merit returned " << merit << "\n"
                 "The reason for this is because the local AppsPack optimizer\n"
                 "uses a cache with a given cache comparison tolerance\n"
                 "By decreasing the 'Cache Comparison Tolerance', you can get\n"
                 "appspack to return merit values closer to what corresponds\n"
                 "for a given set of parameters."
              << std::endl;
  }

  std::cout << "evals of merit function :  " << n_eval << std::endl;
  n_eval = 0;

  opts.localParam.sublist("Solver").setParameter("Initial Step", static_cast<double>(0.10));
  opts.localParam.sublist("Solver").setParameter("Step Tolerance", static_cast<double>(0.005));
  APPSPACK::Cache::Point::setStaticTolerance(0.005);
  merit = GeneAppsPack() ( &indiv, opts.localParam );

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

    for ( dna[0].val = dna[0].min; dna[0].val <= dna[0].max; dna[0].val += .1 ) {
      for ( dna[1].val = dna[0].min; dna[1].val <= dna[0].max; dna[1].val += .1 ) {
        double m = meritFunctor( dna );
        f << dna << '\t' << m << '\n';
      }
      f << '\n';
    }

    f.close();
  }

  return EXIT_SUCCESS;
}
