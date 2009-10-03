/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2004-2008 Spencer Olson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *  
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *                                                                                 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.                                                                           .
 * 
 * Questions? Contact Spencer Olson (olsonse@umich.edu) 
 */

/** \mainpage olson-tools


        <h3>Technical Reference</h3>
    - \f$\copyright\f$ 1998-2008 Spencer E. Olson<br>
      (except as noted by individual files).


    The OlsonTools package includes a variety of routines and algorithms that
    I've developed and collected over the past few years.  This collection
    represents tools that are most useful for scientific and numerical
    software.


I would really appreciate any and all technical feedback, patches,
contributions.  See http://www.umich.edu/~olsonse/ for my best contact info.  



  This manual is divided in the following sections:
  - \subpage olson_tools_intro 
  - \subpage fit "Fitting Library"
  - \subpage random "Random Routines"
  - \subpage olson_tools_licence "License/Copying Information"
*/

//-----------------------------------------------------------

/** \page olson_tools_intro Introduction
    This page introduces the user to the topic.
*/
    //Now you can proceed to the \ref advanced "advanced section".  

//-----------------------------------------------------------

/** \page random Random Routines
    There are several routines for generating random numbers of various
    distributions.  The base generator for most of these is the \ref MTRand
    MerserneTwister package from Richard Wagner.

    This document is a work in progress.

    Make sure you have first read \ref olson_tools_intro "the introduction".
*/

/** \page olson_tools_licence License/Copying Information
<p>
This software is released under the \ref olson_tools_lgpl "LGPL" license except otherwise
explicitly stated in individual files included in this package. 
</p>

    - \f$\copyright\f$ 1998-2008 Spencer E. Olson
    - (except as noted by individual files).
<p>
Relevant licenses:
    - \ref olson_tools_lgpl "LGPL"
    - \ref gpl "GPL v2" 
<p>

\section olson_tools_lgpl ""
\verbinclude lgpl.txt

\section gpl ""
\verbinclude gpl.txt

*/

