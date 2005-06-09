
#include <olson-tools/GeneticAlg.h>
#include <iostream>
#include <math.h>

#define A(a,b,c)       Allele_struct((a),(b),(c), ALLELE_DYNAMIC_CONT)

merit_t meritfnc( const Gene & gene, void * nothing ) {
    double rho = 0;

    for (int i = gene.Numalleles()-1; i>=0; i--) {
        rho +=  gene[i].val * gene[i].val;
    }

    rho = sqrt(rho);

    if (rho == 0) return 1.0;
    else return sin(rho)/rho;
}

int main() {
    GeneticAlgArgs ga_args;
    ga_args.meritfnc = (void*)meritfnc;
    ga_args.population = 10000;
    ga_args.local_fit_max_individuals_prctage = 0.0003;
    ga_args.local_fit_tolerance = 0.0000000001;
    ga_args.tolerance = 1e-15;
    ga_args.encourage_diversity = true;
    ga_args.replace = 0.97;
    ga_args.maxgeneration = 1000;
    Gene dna;

    dna += A( -10, 0, 10 );
    dna += A( -10, 0, 10 );
    dna += A( -10, 0, 10 );
    dna += A( -10, 0, 10 );
    dna += A( -10, 0, 10 );
    dna += A( -10, 0, 10 );
    dna += A( -10, 0, 10 );
    dna += A( -10, 0, 10 );
    dna += A( -10, 0, 10 );
    dna += A( -10, 0, 10 );


    GeneticAlg ga(dna, ga_args);

    ga.fit(&std::cout);

    Individual indiv(dna, (void*)meritfnc);
    double merit = simplex_fit( &indiv,
                                ga_args.local_fit_max_iteration,
                                ga_args.local_fit_tolerance);

    dna = indiv.DNA;

    std::cout << "local fit gives merit of:  " <<  merit << '\t'
              << dna << std::endl;
    return 0;
}
