// -*- c++ -*-
// $Id: simfit.h,v 1.1.1.1 2005/01/08 04:27:25 olsonse Exp $

/*
 * Copyright 2000-2004 Spencer Eugene Olson --- All Rights Reserved
 *
 * $Log: simfit.h,v $
 * Revision 1.1.1.1  2005/01/08 04:27:25  olsonse
 * Initial import
 *
 *
 */

/** \file
 * Various local fitting routines based on a simplex algoritim implemented in c++.
 * Some of this is:\n
 * Copyright 2000-2004 Spencer Olson,\n
 * and some is not.  Not yet sure which is.  Can't remember.
 */

/** \example fit/simplex/testSimplex.C
 * Simple test to show local optimization using the simplex.
 * @see Simplex,Fit
*/

#ifndef SIMFIT_H
#define SIMFIT_H

#include <unistd.h>


namespace olson_tools{ namespace fit {

/** An internal class for storing the simplex. */
template <typename T = double, typename fitT = double>
class Simplex{
  public:
    //
    Simplex(int maxpar);
    //
    ~Simplex();
    //
    inline T& operator () (int row, int col){ return *(data+row*size+col); }
  private:
    //
    T *data;
    //
    int size;
}; // Simplex

/** An implementation of a simplex local optimizer.
*/
template <typename T = double, typename fitT = double>
class Fit{
  public:
    /**
     * After passing the array to the
     * class, fitting procedures will update the array any time leastsq
     * or simplex is called.
     *
     * @param par[] The parameter to be fit.
     * @param nParams Number of parameters or length of par[].
     * */
    Fit(T par[], unsigned nParams);
    ///
    virtual ~Fit();

    /** Reset tolerance to constant fraction. */
    void setParTol(fitT tolerance);

    /** Set length scales by value for each parameter. */
    void setParVal(T parstep[]);

    /** returns number of minimum value of minfunc after simplex minimization. */
    virtual fitT simplex(fitT ftol, unsigned MaxIterations = 2000);

    ///
    virtual fitT simplex(T min, T max, fitT tol, 
                           unsigned MaxIterations = 2000);

    ///
    virtual fitT simplex(T min[], T max[], fitT tol,
                           unsigned MaxIterations = 2000);

    /** Function that is to be minimized.
     * The following function needs to be defined by classes doing this
     * minimization.
     */
    virtual fitT minfunc(T p[])=0;

    ///
    inline int iterations() const {return iter;}
  private:
    /** Downhill simplex method from Numerical Recipes.
     * */
    void Amoeba();
    void initSimplex();
    void initY();
    fitT GetBest();
    Simplex<T,fitT> smplx;
    fitT *y;
    T *pr, *prr;
    fitT *pbar;
    unsigned ndim;
    fitT ftol;
    unsigned iter;
    unsigned itLimit;
    fitT partol;
    T *par;  /// parameters to fit
    T *minval,*maxval;  // min and max values of parameters
}; // Fit

/** A least squares fit using a simplex optimizer. */
template <typename T = double, typename fitT = double>
class LsqFit: public Fit<T,fitT>{
  public:
    /** Constructor.
     * @param ppar
     *     Not sure yet.
     * @param nParams
     *     Length of the ppar array.
     * @param x,yy
     *     Arrays of the data to be fit.
     * @param nData
     *     Length of the x and y arrays.
     **/
    LsqFit(T ppar[], unsigned nParams,
	   T x[], fitT yy[], unsigned nData
	  ):
	    Fit<T,fitT>(ppar, nParams),xp(x),yp(yy),Npts(nData){ }

    /** Destructor. */
    virtual ~LsqFit() {}

    /** Minimizer merit func. */
    virtual fitT minfunc(T p[]);

    /** Fit function (to be defined by inheriting classes).
     *
     * This member function needs to be defined by classes
     * doing this minimization.  It should return the value of the fit
     * function at the point xp given the parmeter values p.
     * */
    virtual fitT fitfunc(T xp, T p[])=0;
  private:
    ///
    T *xp;
    fitT *yp;
    ///
    unsigned Npts;
}; // LsqFit

}}/*namespace olson_tools::fit */

#include "simfit.cpp"

#endif // SIMFIT_H

