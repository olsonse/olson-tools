// -*- c++ -*-
// $Id: options.h,v 1.2 2005/01/14 14:09:31 olsonse Exp $
/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2000-2004 Spencer E. Olson
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
 * Copyright 2000-2004 Spencer Eugene Olson --- All Rights Reserved
 *
 * $Log: options.h,v $
 * Revision 1.2  2005/01/14 14:09:31  olsonse
 * Fixed documentation on memory.h, msh.h, options.h.
 * Moved new files Distribution.[hC] listutil.h readData.h from dsmc code.
 *
 * Revision 1.1.1.1  2005/01/08 04:27:24  olsonse
 * Initial import
 *
 * Revision 1.3  2004/04/09 12:29:00  olsonse
 * many smaller improvements.
 *
 * Revision 1.2  2003/08/07 09:17:41  olsonse
 * Many updates and improvments on samplers and so on.
 *
 * Revision 1.1.1.1  2003/05/18 02:05:10  olsonse
 * Initial import of atomguide into cvs.
 * This marks the change from a running version that did not really do cells
 * correctly, to one where cells will correctly be done (hopefully).
 *
 */

/** \file
 * Option handler system simpler (hmm) to understand and more modular than getopt.
 *
 * Copyright 2000-2004 Spencer Olson.
 */

/** \example options/main.C
 * Demonstrates the use of the option processor class.
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <strings.h>
#include <string>
#include <vector>
#include "logger.h"
#include <ctype.h>

namespace olson_tools {

/** Interface description for an option handler.
 * A class that inherits and completes this interface, will be able to be
 * registered with the OptionProcessor for handling command line options.
 * */
class OptionHandler {
  public:
    OptionHandler();
    virtual ~OptionHandler();

    /** Returns a command-line usage for the options specified of (this) class.
    * This declaration makes this an abstract class.
    * Inheriting classes MUST implement this routine.
    */
    virtual std::string getUsage() = 0;
#define FORMAT_USAGE_CONT(MSG) "                "MSG"\n"
#define FORMAT_USAGE(OPT, MSG) "    --"OPT"\n"FORMAT_USAGE_CONT(MSG)

    /**
    * Called by the OptionProcessor to handle options.
    *
    * This declaration makes this an abstract class.
    * Inheriting classes MUST implement this routine.
    *
    * If an option on the command line is specified in the form of
    * <code>-option=option-value</code>, then opt will be set to "option" and
    * optarg will be set to "option-value"
    * @param opt
    *    Set to the option name of an -option=value pair.
    * @param optarg
    *    Set to the option value of an -option=value pair.
    * @param silent
    *    handleOpt should not print any errors or other messages when this is
    *    true.
    * @return 1 if option was used successfully, -1 if <b>not</b> used successfully,
    * and 0 if not supported.
    */
    virtual int handleOpt(const char * opt, const char * optarg, const bool silent = false) = 0;

    /**
    * Called by the OptionProcessor::postProcess (called by user).
    *
    * This function allows easy post-processing of all options.  The default
    * function is to do nothing. 
    *
    * @param silent
    *    Boolean to indicate that the post-processing should not result in
    *    any error messages.
    * @return false if any errors were reported.
    */
    virtual bool postProcess( const bool slient = false ) { return true;}

    public: class defs {
      public:
	/** The various types of standard options supported.
	 * @see handleStdOpts.
	 * */
	enum OPTION_TYPE {
	    FLOAT=0,
	    DOUBLE=1,
	    BOOL=2,
	    BOOL_TRUE=3, /**< just set to true; doesn't require option value */
	    BOOL_FALSE=4, /**< just set to false; doesn't require option value */
	    TOGGLE_BOOL=5, /**< just toggle; doesn't require option value */
	    INT=6,
	    UINT=7,
	    SHORT=8,
	    USHORT=9,
	    LONG=10,
	    ULONG=11,
            STRING=12,
	    NOOPT=13
	};

        static const char * option_type_desc[];

	/** Standard option description.
	 * Handlers that want to use handleStdOpts must create an array of
	 * this type to describe the various possible standard options.
	 *
	 * The last element of this array should be initialized to:
	 * \verbatim
	     {
		 name:	(char*)0,
		 mem:	(void*)0,
		 typ:	OptionHandler::defs::NOOPT,
		 units:	(char*)0
	     }
	   \endverbatim
	 * */
	class standard_option {
          public:
            /** Default constructor also allows to set everything.
             * @param n
             *     name of standard option.
             * @param m
             *     (void *):  pointer to memory for saving option value.
             * @param t
             *     Option type.
             * @param u
             *     Description of units when appropriate.
             * @param h
             *     Lengthier description of option.
             * @see enum OPTION_TYPE.
             * */
            standard_option(std::string n = "",
                             void * m = 0x0,
                             enum OPTION_TYPE t = NOOPT,
                             std::string u = "",
                             std::string h = "");
            /** Copy constructor. */
            standard_option(const standard_option &);

            /** Assignment operator. */
            standard_option & operator=(const standard_option &);

            std::string name;
	    void * mem;
	    enum OPTION_TYPE typ;
            std::string units;
            std::string help;

            /** A routine to format a standard option into a usage string. */
            std::string getUsage(const std::string & pre, const size_t & text_width = 60);

	};

        typedef std::vector<standard_option> stdopt_vector;
        typedef std::vector<standard_option>::iterator stdopt_iterator;
    };

    static std::string getStdOptsUsage(const std::string & pre, defs::stdopt_vector & stdopts, const size_t & twidth = 60) {
        std::string retval;
	for ( defs::stdopt_iterator i = stdopts.begin(); i < stdopts.end(); i++) {
            retval.append((*i).getUsage(pre,twidth));
	}//for
        return retval;
    }

    /** A routine to log the various standard options used by a handler.
     * This method is just to make it easier for handlers to log
     * standard options.
     * */
    static void printStdOpts(const char * pre, defs::stdopt_vector & stdopts) {
	unsigned int max_spaces = 0;
	int nspaces = 0;
	const char * spaces =
	       "                                             "
	       "                                             "
	       "                                             "
	       "                                             "
	       "                                             "
	       "                                             "
	       "                                             "
	       "                                             ";

	for ( defs::stdopt_iterator i = stdopts.begin(); i < stdopts.end(); i++) {
	    if ((*i).name.size() >= max_spaces) {
		max_spaces = (*i).name.size() + 1;
	    }
	}//for

	for ( defs::stdopt_iterator i = stdopts.begin(); i < stdopts.end(); i++) {
	    nspaces = (max_spaces - (*i).name.size());
	    switch((*i).typ) {
		case defs::FLOAT: {
		    float fval = *((float*)(*i).mem);
		    logger::log_info("\t%s%s%.*s= %g %s", pre, (*i).name.c_str(), nspaces, spaces, fval, (*i).units.c_str());
		    break;
		}

		case defs::DOUBLE: {
		    double fval = *((double*)(*i).mem);
		    logger::log_info("\t%s%s%.*s= %lg %s", pre, (*i).name.c_str(), nspaces, spaces, fval, (*i).units.c_str());
		    break;
		}

		case defs::BOOL_TRUE:
		case defs::BOOL_FALSE:
		case defs::TOGGLE_BOOL:
		case defs::BOOL: {
		    bool val = *((bool*)(*i).mem);
		    logger::log_info("\t%s%s%.*s= %s %s", pre, (*i).name.c_str(), nspaces, spaces, (val?"true":"false"), (*i).units.c_str());
		    break;
		}

		case defs::INT: {
		    int val = *((int*)(*i).mem);
		    logger::log_info("\t%s%s%.*s= %i %s", pre, (*i).name.c_str(), nspaces, spaces, val, (*i).units.c_str());
		    break;
		}

		case defs::UINT: {
		    unsigned int val = *((unsigned int*)(*i).mem);
		    logger::log_info("\t%s%s%.*s= %u %s", pre, (*i).name.c_str(), nspaces, spaces, val, (*i).units.c_str());
		    break;
		}

		case defs::SHORT: {
		    short val = *((int*)(*i).mem);
		    logger::log_info("\t%s%s%.*s= %hi %s", pre, (*i).name.c_str(), nspaces, spaces, val, (*i).units.c_str());
		    break;
		}

		case defs::USHORT: {
		    unsigned short val = *((unsigned short*)(*i).mem);
		    logger::log_info("\t%s%s%.*s= %hu %s", pre, (*i).name.c_str(), nspaces, spaces, val, (*i).units.c_str());
		    break;
		}

		case defs::LONG: {
		    long val = *((long*)(*i).mem);
		    logger::log_info("\t%s%s%.*s= %li %s", pre, (*i).name.c_str(), nspaces, spaces, val, (*i).units.c_str());
		    break;
		}

		case defs::ULONG: {
		    unsigned long val = *((unsigned long*)(*i).mem);
		    logger::log_info("\t%s%s%.*s= %lu %s", pre, (*i).name.c_str(), nspaces, spaces, val, (*i).units.c_str());
		    break;
		}

		case defs::STRING: {
		    std::string val = *((std::string*)(*i).mem);
		    logger::log_info("\t%s%s%.*s= %s %s", pre, (*i).name.c_str(), nspaces, spaces, val.c_str(), (*i).units.c_str());
		    break;
		}

		case defs::NOOPT:
		default:
		    logger::log_warning("programmer user error!!!! (with the option printer)");
		    break;
	    }//switch
	}//for
    }

    /** Standard option handler.
     * May be called by handleOpt for a specific implementation to handle some
     * standard type options:  bool, double, float, int, uint
     * If an option on the command line is specified in the form of
     * <code>-option=option-value</code>, then opt will be set to "option" and
     * optarg will be set to "option-value"
     * @return 1 if option was used successfully, -1 if <b>not</b> used successfully,
     * and 0 if not supported.
     * */
    static int handleStdOpts(defs::stdopt_vector stdopts,
		    	     const char * opt, const char * optarg, const bool silent = false) {

	int retval = 0;
	/* find all options that correspond to this one and process it */
	for ( defs::stdopt_iterator i = stdopts.begin(); i < stdopts.end(); i++) {
	    if (strcasecmp(opt, (*i).name.c_str()) == 0) {
		if ((*i).typ == defs::TOGGLE_BOOL) {
		    /* set the bool value to what ever it isn't (:-) */
		    *((bool *)(*i).mem) = !(*((bool *)(*i).mem));
		    retval = 1;
		    /* now go onto the next option */
		    continue;
		} else if ((*i).typ == defs::BOOL_TRUE) {
		    /* set the bool value to what ever it isn't (:-) */
		    *((bool *)(*i).mem) = true;
		    retval = 1;
		    /* now go onto the next option */
		    continue;
		} else if ((*i).typ == defs::BOOL_FALSE) {
		    /* set the bool value to what ever it isn't (:-) */
		    *((bool *)(*i).mem) = false;
		    retval = 1;
		    /* now go onto the next option */
		    continue;
		} else if (optarg == NULL) {
		    if(!silent) logger::log_severe("%s option requires a value", (*i).name.c_str());
		    return -1;
		}
		switch((*i).typ) {
		    case defs::FLOAT:
			if (sscanf(optarg, "%f", (float*)(*i).mem) != 1) {
			    if(!silent) logger::log_severe("%s could not be converted to a float", (*i).name.c_str());
			    return -1;
			}
			retval = 1;
			break;

		    case defs::DOUBLE:
			if (sscanf(optarg, "%lf", (double*)(*i).mem) != 1) {
			    if(!silent) logger::log_severe("%s could not be converted to a double", (*i).name.c_str());
			    return -1;
			}
			retval = 1;
			break;

		    case defs::BOOL:
			if (*optarg=='t' || *optarg=='T') {
			    *((bool *)(*i).mem) = true;
			} else if (*optarg=='f' || *optarg=='F') {
			    *((bool *)(*i).mem) = false;
			} else {
			    if(!silent) logger::log_severe("%s could not be converted to a boolean", (*i).name.c_str());
			    return -1;
			}
			retval = 1;
			break;

		    case defs::INT:
			if (sscanf(optarg, "%i", (int*)(*i).mem) != 1) {
			    if(!silent) logger::log_severe("%s could not be converted to an int", (*i).name.c_str());
			    return -1;
			}
			retval = 1;
			break;

		    case defs::UINT:
			if (sscanf(optarg, "%u", (unsigned int *)(*i).mem) != 1) {
			    if(!silent) logger::log_severe("%s could not be converted to an unsigned int", (*i).name.c_str());
			    return -1;
			}
			retval = 1;
			break;

		    case defs::SHORT:
			if (sscanf(optarg, "%hi", (short *)(*i).mem) != 1) {
			    if(!silent) logger::log_severe("%s could not be converted to a short", (*i).name.c_str());
			    return -1;
			}
			retval = 1;
			break;

		    case defs::USHORT:
			if (sscanf(optarg, "%hu", (unsigned short *)(*i).mem) != 1) {
			    if(!silent) logger::log_severe("%s could not be converted to an unsigned short", (*i).name.c_str());
			    return -1;
			}
			retval = 1;
			break;

		    case defs::LONG:
			if (sscanf(optarg, "%li", (long *)(*i).mem) != 1) {
			    if(!silent) logger::log_severe("%s could not be converted to a long", (*i).name.c_str());
			    return -1;
			}
			retval = 1;
			break;

		    case defs::ULONG:
			if (sscanf(optarg, "%lu", (unsigned long *)(*i).mem) != 1) {
			    if(!silent) logger::log_severe("%s could not be converted to an unsigned long", (*i).name.c_str());
			    return -1;
			}
			retval = 1;
			break;

		    case defs::STRING:
			(*((std::string *)(*i).mem)) = optarg;
			retval = 1;
			break;

		    case defs::NOOPT:
		    default:
			logger::log_warning("programmer user error!!!! (with the option handler)");
			break;
		}//switch
	    }//if name
	}//for

	return retval;
    }

}; // class OptionHandler

/** The option processor.
 * OptionHandler\s must be first registered with this class and then allow for
 * OptionProcessor::processOptions() to be called.
 * */
class OptionProcessor {
  public:
    OptionProcessor();
    ~OptionProcessor();

    /** Register an OptionHandler. */
    void addHandler(const OptionHandler & oh);

    /** Unregister an OptionHandler. */
    void removeHandler(const OptionHandler & oh);

    /** Process all of the options.
     * @param argc
     *    Number of options.
     * @param argv
     *    Array of c-strings of the options.
     * @param silent
     *    Boolean to indicate that the option handling should not result in
     *    any error messages.
     * @param ignore_nonopt
     *    Boolean:  specify that command line options that do not begin with
     *    '--' are ignored [Default false].
     * @return false if all options were not successfully used.
     */
    bool processOptions( const int argc, char ** argv,
                         const bool silent = false,
                         const bool ignore_nonopt = false ) const;

    /** Process all of the options.
     * @param options
     *    Vector of each of the options in the form of --option=value.
     * @param silent
     *    Boolean to indicate that the option handling should not result in
     *    any error messages.
     * @param ignore_nonopt
     *    Boolean:  specify that command line options that do not begin with
     *    '--' are ignored [Default false].
     * @return false if all options were not successfully used.
     */
    bool processOptions( std::vector<std::string> & options,
                         const bool silent = false,
                         const bool ignore_nonopt = false ) const;

    /** Post process.
     *
     * This hooks into all handlers to allow post-processing.
     *
     * @param silent
     *    Boolean to indicate that the post-processing should not result in
     *    any error messages.
     * @return false if any errors were reported.
     */
    bool postProcess( const bool slient = false ) const ;

    /** Send to stdout all of the usage information.
     * The result of this function call will vary as the OptionHandler\s are
     * varied.
     * */
    void usage() const;

    /** Set header to print out when usage is printed
     */
    inline void setUsageHeader(std::string header) { usageHeader = header; }

  private:
    std::vector<OptionHandler *> handlers;
    std::string usageHeader;
}; // class OptionProcessor

}/* namespace olson_tools */

#endif // OPTIONS_H
