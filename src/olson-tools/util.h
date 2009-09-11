// -*- c++ -*-
// $Id: util.h,v 1.1.1.1 2005/01/08 04:27:25 olsonse Exp $
/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 1998-2004 Spencer E. Olson
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

/*
 * Copyright 1998-2004 Spencer Eugene Olson --- All Rights Reserved
 *
 * $Log: util.h,v $
 * Revision 1.1.1.1  2005/01/08 04:27:25  olsonse
 * Initial import
 *
 * Revision 1.1.1.1  2003/05/18 02:05:10  olsonse
 * Initial import of atomguide into cvs.
 * This marks the change from a running version that did not really do cells
 * correctly, to one where cells will correctly be done (hopefully).
 *
 * Revision 1.6  2001/01/11 16:16:34  olsons
 * Improved plotting:  finished 3d plotting, added ability to change x-display.
 *
 * Revision 1.5  2000/10/25 18:19:46  olsons
 * Finished (almost) all of the remaining functions and menus that were missing.
 *
 * Revision 1.4  2000/06/15 19:32:56  olsons
 * Added get_Long_Comment function.
 *
 * Revision 1.3  2000/05/06 20:39:26  olsons
 * New interface design for next version of xuvmulti.
 *
 * Revision 1.2  1999/08/18 13:50:41  olsons
 * *** empty log message ***
 *
 * Revision 1.1  1999/07/03 05:29:37  olsons
 * Added character-based user interface to XUVMulti
 *
 *
 */

#ifndef UTIL_H
#define UTIL_H

/** \file
 * TextUI Library
 * by Spencer Olson
 * copyright 1998-2004
 *
 * Free use granted as long as this notice is included.
*/

#include <string>



namespace olson_tools {


// the following reads from stdin to find the max & min and value of a
// certain parameter named string. 
void text_getMinValMax(double &min,double &val,double &max,bool &stat,const std::string & qstring);

void text_getMinValMax(int &min,int &val,int &max,bool &stat, const std::string & qstring);

void text_getMinValMax(float &min,float &val,float &max,bool &stat, const std::string & qstring);

//query the user for some specific value
double text_getDouble(double min, double defaultval, double max, const std::string & qstring);

int text_getInt(int min, int defaultval, int max, const std::string & qstring);

float text_getFloat(float min, float defaultval, float max, const std::string & qstring);

bool text_getBool(bool defaultval,const std::string & qstring);//ask a y/n question

//returns the selection
int text_menu( int itemsz, const char * items[], const std::string & title );

//calls menu with only the enabled items
int text_menu( int nitems, const char * items[], bool enabled[],
          const std::string & title );

// a pretty trivial function which just gets a string from the user 
std::string & text_getStr(std::string & str, const std::string & qstring);

std::string & text_get_Filename( std::string & defaultval, const std::string & prompt );

std::string & text_get_Long_Comment( std::string & defaultval, const std::string & prompt );

void text_alert( const std::string & message );

void text_alert( const char * message );

// just to create a pause; "Press <Enter> to continue."
int pausetext() ;


}/* namespace olson_tools */

#endif // UTIL_H
