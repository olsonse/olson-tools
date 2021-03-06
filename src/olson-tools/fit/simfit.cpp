// -*- c++ -*-
// $Id$

// if QUIET is defined, normal non-error output is suppressed
#define QUIET

#include "simfit.h"
#include <cmath>
#ifndef QUIET
#  include <iostream>
#endif

namespace olson_tools{ namespace fit {

template <typename T, typename fitT>
fitT LsqFit<T,fitT>::minfunc( T p[] ) {
  fitT sum=0.0;
  for(unsigned int i=0;i<Npts;i++) {
    fitT term = fitfunc(xp[i], p) - yp[i];
    sum += term*term;
  }
  return sum;
} // LsqFit<T,fitT>::minfunc

template <typename T, typename fitT>
fitT Fit<T,fitT>::simplex( T min,
                           T max,
                           fitT tol,
                           unsigned MaxIterations){

  if (minval) {
    /* clean up from previous. */
    delete[] minval;
    delete[] maxval;
  }
  minval=new T[ndim];
  maxval=new T[ndim];
  for(unsigned int i=0;i<ndim;i++){
    minval[i] = min;
    maxval[i] = max;
  }
  return Fit<T,fitT>::simplex(tol,MaxIterations);
} // Fit<T,fitT>::simplex

template <typename T, typename fitT>
fitT Fit<T,fitT>::simplex( T min[],
                           T max[],
                           fitT tol,
                           unsigned MaxIterations ) {
  if (minval) {
    /* clean up from previous. */
    delete[] minval;
    delete[] maxval;
  }
  minval=new T[ndim];
  maxval=new T[ndim];
  for(unsigned int i=0;i<ndim;i++){
    minval[i] = min[i];
    maxval[i] = max[i];
  }
  return Fit<T,fitT>::simplex(tol,MaxIterations);
} // Fit<T,fitT>::simplex

template <typename T, typename fitT>
fitT Fit<T,fitT>::simplex( fitT tol, unsigned MaxIterations ) {
  if (minval) {
    /* clean up from previous. */
    delete[] minval;
    delete[] maxval;

    /* we won't use min/maxval here. */
    minval = maxval = NULL;
  }

  ftol = tol;
  itLimit=MaxIterations;
  // Initialize simplex
  initSimplex();
  initY();
  Amoeba();
  return GetBest();
  // Find best solution
}

///
template <typename T, typename fitT>
Fit<T,fitT>::Fit(T params[], unsigned nParams)
  : smplx(nParams), ndim(nParams), par(params) {
  y      = new fitT[nParams+1];
  pr     = new T[nParams];
  prr    = new T[nParams];
  pbar   = new fitT[nParams];
  minval = NULL;
  maxval = NULL;
  setParTol(0.2); // set default tolerance to 0.2 (sets partol)
  itLimit=10;  // safe default
  iter   =0;
  ftol   =100; // should be reset by simplex
} // Fit<T,fitT>::Fit

template <typename T, typename fitT>
void Fit<T,fitT>::setParTol(fitT x){
  partol=x;
}

///
template <typename T, typename fitT>
Fit<T,fitT>::~Fit(){
  delete[] y;
  delete[] pr;
  delete[] prr;
  delete[] pbar;
  if (minval) {
    delete[] minval;
    delete[] maxval;
  }
}

///
template <typename T, typename fitT>
void Fit<T,fitT>::Amoeba(){
  const fitT alpha=1.0;
  const fitT beta=0.5;
  const fitT gamma=2.0;
  
  // These parameters control the expansions, contractions, and maximum
  // allowed number of iterations.

  unsigned mpts, j, inhi, ilo, ihi, i;
  fitT yprr, ypr, rtol;
  unsigned itmax;
  
  itmax = itLimit;
  mpts = ndim;
  iter = 0;
  while(1){
    // Determine which point is the highest (worst), next-highest,
    // and lowest (best), by looping over the points in the simplex.
    ilo = 0;
    if (y[0] > y[1]) {
      ihi = 0;
      inhi = 1;
    } else {
      ihi = 1;
      inhi = 0;
    }
    for(i=0;i<=mpts;i++){ 
      // loop over points of the simplex
      if (y[i] < y[ilo]) ilo = i;
      if (y[i] > y[ihi]) { inhi = ihi; ihi = i; }
      else if ( (y[i] > y[inhi]) && (i != ihi) ) inhi = i;
    } // for
    // Compute the fractional range from highest to lowest and return
    // if satisfactory.
    rtol = 2.0*std::abs(y[ihi]-y[ilo])/(std::abs(y[ihi])+std::abs(y[ilo]));
    if (rtol < ftol) {
      return;
    }
    if (iter++ == itmax) {
#ifdef QUIET
      char ans='N';
#else
      cerr<<"AMOEBA - too many iterations\n";
      cerr<<"   Continue? (Y/N) : ";
      char ans;
      cin>>ans;
#endif
      if (ans == 'Y' || ans == 'y') {
        itmax = itmax + itLimit;
      } else {
        // exit gracefully
        return;
      }
    } // if itmax is reached
    for (j=0;j<ndim;j++)
      pbar[j] = fitT(0.0);
    // Begin a new iteration.  Compute the vector average of all points
    // except the highest, i.e. the center of the "face" of the simplex
    // across from the high point.  We will subsequently explore along
    // the ray from the high point through that center.
    for(i=0;i<=mpts;i++) 
      if (i != ihi)
        for (j=0;j<ndim;j++) pbar[j] = pbar[j]+smplx(i,j);
    // Extrapolate by a factor alpha through the face, i.e. reflect
    // the simplex from the high point.
    for(j=0;j<ndim;j++){
      pbar[j] = pbar[j]/ndim;
      pr[j] = T( (1.0+alpha)*pbar[j]-alpha*smplx(ihi,j) );
    }
    ypr = minfunc(pr);  /* Evaluate the function at reflected point */

    /* We'll now define the bounds penalty with respect to the first evaluation
     * of minfunc(...).  The factor of 20 is arbitrarily chosen. */
    const fitT bounds_penalty = std::abs(ypr) * 20;

    for (i=0;i<ndim;i++) {
      /* some one forgot to comment: I guess the following is a hack
       * done by someone here at BYU to make it not want to make
       * a certain layer to small.  This has been changed so as
       * to allow the user to give the simplex a minimum value
       * to want to have.  Pass it in with the itmax and other parameter.
       * Perhaps we can also change it so that we can have a maximum
       * value.
      */ 
      if( minval && ((pr[i]<minval[i]) || (pr[i]>maxval[i])) )
        ypr += bounds_penalty;
    }
    if (ypr <= y[ilo]){
      // There was an improvement, try an additional extrapolation by
      // a factor gamma and check out the function there.
      for(j=0;j<ndim;j++) prr[j] = T( gamma*pr[j]+(1.0-gamma)*pbar[j] );
      yprr = minfunc(prr);
      /* see above note about minimum values */
      for (i=0;i<ndim;i++) {
        if( minval && ((prr[i]<minval[i]) || (prr[i]>maxval[i])) )
          yprr += bounds_penalty;
      }
      if (yprr < y[ilo]){
        // The additional extrapolation succeeded, and replaces
        // the high point.
        for(j=0;j<ndim;j++) smplx(ihi,j) = prr[j];
        y[ihi] = yprr;
      } else{
        //  The additional extrapolation failed, but we can still
        // use the reflected point.
        for(j=0;j<ndim;j++) smplx(ihi,j) = pr[j];
        y[ihi] = ypr;
      }
    } // if there was improvement
    else
      if (ypr >= y[inhi]) {
        //  The reflected is worse than the second-highest.  If it's
        // better than the highest, then replace the highest, but look
        // for an intermediate lower point.
        if(ypr < y[ihi]) {
          for(j=0;j<ndim;j++) smplx(ihi,j) = pr[j];
          y[ihi] = ypr;
        }
        for(j=0;j<ndim;j++)
          // Check for intermediate lower point by performing a
          // contraction of the simplex along one dimension.  Then
          // evaluate the function.
          prr[j] = T( beta*smplx(ihi,j)+(1.0-beta)*pbar[j] );
        yprr = minfunc(prr);
        /* see above note for minimum values */
        for (i=0;i<ndim;i++) {
          if( minval && ((prr[i]<minval[i]) || (prr[i]>maxval[i])) )
            yprr += bounds_penalty;
        }
        if (yprr < y[ihi]) {
          // Contraction gives an improvement, so accept it.
          for(j=0;j<ndim;j++) smplx(ihi,j) = prr[j];
          y[ihi] = yprr;
        } else {
          // Can't seem to get rid of that high point.  Better contract
          // around the lowest (best) point.
          for(i=0;i<=mpts;i++) 
            if (i != ilo) {
              for (j=0;j<ndim;j++) {
                pr[j] = T( 0.5*(smplx(i,j)+smplx(ilo,j)) );
                smplx(i,j) = pr[j];
              }
              y[i] = minfunc(pr);
              /* see above note for minimum values */
              for (j=0;j<ndim;j++) {
                if( minval && ((pr[j]<minval[j]) || (pr[j]>maxval[j])) )
                  y[i] += bounds_penalty;
              }
            }
        }
      } else {
        // The original reflection gives a middling point.  Replace
        // the old high point and continue.
        for(j=0;j<ndim;j++) smplx(ihi,j) = pr[j];
        y[ihi] = ypr; 
      } 
  } /* while TRUE */
} /** Amoeba */

template <typename T, typename fitT>
void Fit<T,fitT>::initSimplex(){
  if(partol){
    for(unsigned int i=0;i<=ndim;i++)
      for(unsigned int j=0;j<ndim;j++){
        if (i==(j+1))
          smplx(i,j) = par[j] * T(1.0+partol);
        else
          smplx(i,j) = par[j];
      }
  } // if parameter ranges aren't set individually
} // Fit<T,fitT>::initSimplex

template <typename T, typename fitT>
void Fit<T,fitT>::setParVal(T parstep[]){
  partol=0; // flag simplex as already initialized
  for(unsigned int i=0;i<=ndim;i++)
    for(unsigned int j=0;j<ndim;j++){
      if (i==(j+1))
        smplx(i,j) = par[j] * (1.0+parstep[j]);
      else
        smplx(i,j) = par[j];
    }
} // Fit<T,fitT>::setParVal

template <typename T, typename fitT>
void Fit<T,fitT>::initY(){
  T *xary=new T[ndim];
  unsigned i,j;
  for(i=0;i<=ndim;i++){
    for(j=0;j<ndim;j++)
      xary[j] = smplx(i,j);
    y[i] = minfunc(xary);
  }
  delete[] xary;
} // Fit<T,fitT>::initY

template <typename T, typename fitT>
fitT Fit<T,fitT>::GetBest(){
  unsigned i, ibest;
  fitT besty;
  ibest = 0;
  besty = y[0];
  for(i=1;i<=ndim;i++)
    if (y[i] < besty){
      besty = y[i];
      ibest = i;
    }
  for(i=0;i<ndim;i++){
   par[i] = smplx(ibest,i);
 }
  return besty;
} // Fit<T,fitT>::GetBest

template <typename T, typename fitT>
Simplex<T,fitT>::Simplex(int maxpar){
  data = new T[maxpar*(maxpar+1)];
  size=maxpar;
#ifndef QUIET
  cout<<"Simplex size is "<<maxpar<<endl;
  cout<<"Allocated "<<maxpar*(maxpar+1)<<endl;
  cout.flush();
#endif
}

///
template <typename T, typename fitT>
Simplex<T,fitT>::~Simplex(){
  delete[] data;
}

}}/*namespace olson_tools::fit */
