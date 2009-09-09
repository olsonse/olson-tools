// $Id$ 

/*!
  \file testThreadedExecutor.cpp

*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <list>

#ifdef USE_PIPE
#  define PROB_FILE "./prob-pipe-io.py"
#  include <olson-tools/fit/PExecFunc.hpp>   // <-- Provides user-defined custom executor
typedef olson_tools::fit::PExecFunc MinFunc;
#else // USE_FILE
#  define PROB_FILE "./prob-file-io.py"
#  include <olson-tools/fit/ExecFunc.hpp>    // <-- Provides user-defined custom executor
typedef olson_tools::fit::ExecFunc MinFunc;
#endif

#include <olson-tools/fit/appspack/ThreadedExecutor.hpp> // <-- Provides user-defined custom executor
#include <appspack/APPSPACK_Solver.hpp>                  // <-- Provides APPSPACK::Solver
#include <appspack/APPSPACK_Vector.hpp>                  // <-- Provides APPSPACK::Vector 
#include <appspack/APPSPACK_Constraints_Linear.hpp>      // <-- Provides APPSPACK::Constraints::Linear
#include <appspack/APPSPACK_Float.hpp>

int main(int argc, char* argv[]) {
  // *** Create and fill the parameter list ***
  APPSPACK::Parameter::List params;

  // *** Create the MinFunc
  MinFunc minFunc(PROB_FILE);

  int n = 2;
  APPSPACK::Vector initialx(n, 1.0);
  APPSPACK::Vector initialf;
  minFunc(initialx.getStlVector(), initialf, 0);
  params.sublist("Solver").setParameter("Initial X", initialx);
  params.sublist("Solver").setParameter("Initial F", initialf);

  // Be sure to do casts on any ambiguous constants, such as in this case
  params.sublist("Solver").setParameter("Debug", static_cast<int>(3));
  params.sublist("Solver").setParameter("Initial Step", static_cast<double>(0.25));
  params.sublist("Solver").setParameter("Step Tolerance", static_cast<double>(1e-3));
  params.sublist("Solver").setParameter("Bounds Tolerance", static_cast<double>(1e-3));
  params.sublist("Solver").setParameter("Cache Output File", std::string("eval_output"));
  params.sublist("Solver").setParameter("Cache Input File", std::string("eval_output"));


  APPSPACK::Vector lowerbounds(n, -10.0);
  APPSPACK::Vector upperbounds(n,  10.0);
  params.sublist("Linear").setParameter("Lower", lowerbounds);
  params.sublist("Linear").setParameter("Upper", upperbounds);

  APPSPACK::Matrix ineq_matrix;
  ineq_matrix.addRow( std::vector<double>(n, 1.0) );
  APPSPACK::Vector ineq_upper(1, 3);
  APPSPACK::Vector ineq_lower(1, APPSPACK::dne());
  params.sublist("Linear").setParameter("Inequality Matrix", ineq_matrix);
  params.sublist("Linear").setParameter("Inequality Upper", ineq_upper);
  params.sublist("Linear").setParameter("Inequality Lower", ineq_lower);
  

  // *** Instantiate the bounds ***
  APPSPACK::Constraints::Linear linear(params.sublist("Linear"));

  // *** Instantiate the custom executor ***
  olson_tools::fit::appspack::ThreadedExecutor<MinFunc> executor(minFunc);
  
  
  // *** Create the solver ***
  APPSPACK::Solver solver(params.sublist("Solver"), executor, linear);
  
  // *** Run the solver ***
  APPSPACK::Solver::State state = solver.solve();
  
  return EXIT_SUCCESS;
}
