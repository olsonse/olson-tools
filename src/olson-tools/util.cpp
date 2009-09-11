// -*- c++ -*-
// $Id: util.C,v 1.1.1.1 2005/01/08 04:27:25 olsonse Exp $
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
 * $Log: util.C,v $
 * Revision 1.1.1.1  2005/01/08 04:27:25  olsonse
 * Initial import
 *
 * Revision 1.1.1.1  2003/05/18 02:05:10  olsonse
 * Initial import of atomguide into cvs.
 * This marks the change from a running version that did not really do cells
 * correctly, to one where cells will correctly be done (hopefully).
 *
 * Revision 1.7  2001/01/11 16:16:34  olsons
 * Improved plotting:  finished 3d plotting, added ability to change x-display.
 *
 * Revision 1.6  2000/10/25 18:19:46  olsons
 * Finished (almost) all of the remaining functions and menus that were missing.
 *
 * Revision 1.5  2000/06/15 19:32:56  olsons
 * Added get_Long_Comment function.
 *
 * Revision 1.4  2000/05/06 20:39:26  olsons
 * New interface design for next version of xuvmulti.
 *
 * Revision 1.3  1999/08/18 19:35:39  olsons
 * Changed util.C: getMat so that you can enter either material name or number.
 *
 * Revision 1.2  1999/08/18 13:50:41  olsons
 * *** empty log message ***
 *
 * Revision 1.1  1999/07/03 05:29:37  olsons
 * Added character-based user interface to XUVMulti
 *
 *
 */


/* TextUI Library
 * by Spencer Olson
 * copyright 1998-2004
 *
 * Free use granted as long as this notice is included.
*/


#include "util.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <string>
#include <iostream>
#include <cfloat>
#include <stdlib.h>

#define GNU_READLINE
#ifdef GNU_READLINE

extern "C" {
	char * readline( const char * );
        int add_history( const char * );
        char * rl_readline_name;
	int rl_stuff_char (int c);
}

void rl_stuff( const char * str ) {
  for( int i = 0, len = strlen(str); i < len; rl_stuff_char( str[i++] ) );
}//rl_stuff

#endif

namespace olson_tools {

using namespace std;


void text_getMinDoubleMax(double &min,double &val,double &max,
                  bool &stat, const string & qstring ) {
  // reads from stdin to find the max & min and value of a
  // certain parameter named parname. 

  string tmpqstr = string( "Do you want the parameter \'" )
                   + qstring + string( "\' to be static? ");

  double tmpmin(-1e10),tmpmax(1e10);
  if( ! ( stat = text_getBool( stat, tmpqstr ) ) ) {
    while(1) {
      tmpmin=text_getDouble(-1e10,min,1e10,"Minimum Value: ");
      tmpmax=text_getDouble(-1e10,max,1e10,"Maximum Value: ");
      if(tmpmin<=tmpmax) {min=tmpmin;max=tmpmax;break;}
      cout << "Error: minimum value greater than maximum value.\n";
    }//while
  }//if not static
  val = text_getDouble( tmpmin, val, tmpmax, qstring );
}//getMinDoubleMax

void text_getMinIntMax(int &min,int &val,int &max,
                  bool &stat, const string & qstring ) {
  // reads from stdin to find the max & min and value of a
  // certain parameter named parname. 

  string tmpqstr = string( "Do you want the parameter \'" )
                   + qstring + string( "\' to be static? ");

  int tmpmin(-60000),tmpmax(60000);
  if( !( stat = text_getBool( stat, tmpqstr ) ) ) {
    while(1) {
      tmpmin = text_getInt(-60000,min,60000,"Minimum Value: ");
      tmpmax = text_getInt(-60000,max,60000,"Maximum Value: ");
      if(tmpmin<=tmpmax) {min=tmpmin;max=tmpmax;break;}
      cout<<"Error: minimum value greater than maximum value.\n";
    }//while
  }//if not static
  val = text_getInt( tmpmin, val, tmpmax, qstring );
}//getMinIntMax

void text_getMinFloatMax( float &min, float &val, float &max,
                   bool &stat, const string & qstring ) {
  // reads from stdin to find the max & min and value of a
  // certain parameter named parname. 

  string tmpqstr = string( "Do you want the parameter \'" )
                   + qstring + string( "\' to be static? ");

  float tmpmin(-1e10),tmpmax(1e10);
  if( !( stat = text_getBool( stat, tmpqstr ) ) ) {
    while(1) {
      tmpmin=text_getFloat(float(-1e10),min,float(1e10),"Minimum Value: ");
      tmpmax=text_getFloat(float(-1e10),max,float(1e10),"Maximum Value: ");
      if(tmpmin<=tmpmax) {min=tmpmin;max=tmpmax;break;}
      cout<<"Error: minimum value greater than maximum value.\n";
    }//while
  }//if not static
  val=text_getFloat(tmpmin,val,tmpmax,qstring);
}//getMinFloatMax

double text_getDouble( double min, double defaultval, double max,
               const string & qstring) {
  //query the user for some specific value
  char buf[100];
  double selection=-FLT_MAX;
  while((selection<min)||(selection>max)) {
    cout<<qstring<<"["<<defaultval<<"] ";
    cin.getline(buf,100);
    if(strlen(buf)==0 &&(defaultval>=min)&&(defaultval<=max)) return defaultval;
    else if(sscanf(buf,"%lf",&selection)
       &&(selection>=min)&&(selection<=max)) {
       return selection;
    }//if
    selection=-FLT_MAX;
    cout<<"Value must be between "<<min<<" and "<<max<<".\n";
  }//while
  return 0;//this line of code should never be reached
}//getDouble

int text_getInt( int min, int defaultval, int max, const string & qstring ) {
  //query the user for some specific value
  char buf[100];
  int selection=min-1;
  while((selection<min)||(selection>max)) {
    cout<<qstring<<"["<<defaultval<<"] ";
    cin.getline(buf,100);
    if(strlen(buf)==0) sprintf(buf,"%d",defaultval);
    if(sscanf(buf,"%d",&selection)&&(selection>=min)&&(selection<=max))
       return selection;
    selection=min-1;
    cout<<"Value must be between "<<min<<" and "<<max<<".\n";
  }//while
  return 0;//this line of code should never be reached
}//getInt

float text_getFloat(float min, float defaultval, float max, const string & qstring) {
  //query the user for some specific value
  char buf[100];
  float selection=-FLT_MAX;
  while((selection<min)||(selection>max)) {
    cout<<qstring<<"["<<defaultval<<"] ";
    cin.getline(buf,100);
    if(strlen(buf)==0) sprintf(buf,"%f",defaultval);
    if(sscanf(buf,"%f",&selection)
       &&(selection>=min)&&(selection<=max)) {
       return selection;
    }//if
    selection=-FLT_MAX;
    cout<<"Value must be between "<<min<<" and "<<max<<".\n";
  }//while
  return 0;//this line of code should never be reached
}//getFloat

bool text_getBool( bool defaultval, const string & qstring ) {
  //ask a y/n question
  char selection[100];selection[0]='\0';
  char defchar; if(defaultval)defchar='y'; else defchar='n';
  while(selection[0]=='\0') {
    cout<<qstring<<"["<<defchar<<"] ";
    cin.getline(selection,100);
    if(strlen(selection)==0) return defaultval;
    if((selection[0]=='y')||(selection[0]=='Y'))return true;
    else if((selection[0]=='n')||(selection[0]=='N'))return false;
    selection[0]='\0';
    cout<<"You must enter y or n.\n";
  }//while
  return false;//this line of code should never be reached
}//getBool

int text_menu(int nitems, const char * items[], const string & title ) {
  //returns the selection
  int i,selection=0;
  static char buf[100];
  while(selection==0) {
    cout<<title<<endl;
    for(i=1;i<=nitems;i++) cout<<"\t"<<i<<".\t"<<items[i-1]<<endl;
    cout<<"\t\tYour Choice: ";
    cin.getline(buf,100);
    sscanf(buf,"%d",&selection);
    if((selection>0)&&(selection<=nitems)) 
      return selection-1;//-1 because we will return index of option
    selection=0;
    cout<<"Please select one of the following items"
        <<" by entering the adjacent index.\n";
  }//while
  return selection;//this line of code never should be reached
}//text_menu

int text_menu( int nitems, const char * items[], bool enabled[], const string & title ) {
  //returns the selection
  int j, msize, selection;
  char ** enableditems = new char*[nitems];

  for( j = 0, msize = 0; j < nitems; j++ ) {
    if( enabled[ j ] ) {
      enableditems[ msize ] = new char[ strlen( items[ j ] ) + 1 ];
      strcpy( enableditems[ msize++ ], items[ j ] );
    }//
  }//for

  selection = text_menu( msize, (const char **)enableditems, title );

  for(j=0;selection>=0;j++) {
    if(enabled[j]) selection--;
  }//for
  selection = j-1;

  for( j = 0; j < msize; delete[] enableditems[j++] );
  delete[] enableditems;
  return selection;
}//text_menu

string & text_getStr(string & str, const string & qstring ) {
  // a pretty trivial function which just gets a string from the user 
#ifndef GNU_READLINE
  string buf;
  cout << qstring << "[" << str << "] ";
  getline( cin, buf );
  if( buf.length () != 0 ) str = buf;
#else
  char * buf;
  rl_stuff ( str.c_str() );
  buf = readline( qstring.c_str() );
  if( buf ) {
    add_history( buf ), str = buf;
    free(buf);
  }//if
  else str = "";
#endif
  return str;
}//getStr

void text_alert( const string & message ) {
  cout << message << endl;
  pausetext();
}//text_alert

void text_alert( const char * message ) {
  cout << message << endl;
  pausetext();
}//text_alert

int pausetext(){
  char c;
  cout<<"Press <Enter> to continue.";
  cout.flush();
  cin.get( c );
  return int( c );
} // pausetext

string & text_get_Filename( string & defaultval, const string & prompt ){
  return text_getStr( defaultval, prompt );
} // text_get_Filename

string & text_get_Long_Comment( string & str, const string & prompt ){
  // a pretty trivial function which just gets a string from the user 
  string buf;
  cout << prompt << endl << "[" << str << "]" << endl
       << "Please leave a blank line to end (or press return now to keep previous value)"
       << endl;
  getline( cin, buf );
  if( buf.length () != 0 ) {
    str = buf;
    getline( cin, buf );
    while( buf.length () != 0 ) {
      str += '\n';
      str += buf;
      getline( cin, buf );
    }//while
  }//if
  return str;
} // text_get_Long_Comment


}/* namespace olson_tools */
