// -*- c++ -*-
// $Id: options.C,v 1.1.1.1 2005/01/08 04:27:24 olsonse Exp $
/*
 * Copyright 2000-2004 Spencer Eugene Olson --- All Rights Reserved
 *
 * $Log: options.C,v $
 * Revision 1.1.1.1  2005/01/08 04:27:24  olsonse
 * Initial import
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

#include "options.h"
#include <string.h>
#include <strings.h>
#include "logger.h"
#include <iostream>
#include <iterator>

const char * OptionHandler::defs::option_type_desc[] = {
    "float",
    "double",
    "boolean",
    0x0 /*"true"*/,
    0x0 /*"false"*/,
    "boolean-toggle",
    "integer",
    "unsigned-integer",
    "short-integer",
    "unsigned-short-integer",
    "long-integer",
    "unsigned-long-integer",
    "null-option",
    NULL
};

OptionHandler::OptionHandler() {}
OptionHandler::~OptionHandler() {}


OptionProcessor::OptionProcessor() {}
OptionProcessor::~OptionProcessor() {}

/**
 */
void OptionProcessor::addHandler( const OptionHandler & oh ) {
    OptionHandler * ohP = &((OptionHandler &)oh);
    handlers.push_back(ohP);
}

/**
 */
void OptionProcessor::removeHandler( const OptionHandler & oh ) {
    OptionHandler * ohP = &((OptionHandler &)oh);
    for ( std::vector<OptionHandler *>::iterator i = handlers.begin(), e = handlers.end(); i < e; i++ ) {
	if (*i == ohP) {
	    handlers.erase(i);
	    return;
	}
    }//for
}

/**
 * returns false if not successful with all options.
 */
bool OptionProcessor::processOptions( const int argc, char ** argv, const bool silent /* = false */) const {
    /* convert to a vector of options */
    std::vector<std::string> options;
    for( int i = 0; i < argc; i++ ) {
	options.push_back(argv[i]);
    }

    /* now finish with the call to the real function */
    return processOptions( options, silent );
}

/**
 * returns false if not successful with all options.
 */
bool OptionProcessor::processOptions( std::vector<std::string> & options, const bool silent /* = false */) const {
    int unhandled = 0;

    for( std::vector<std::string>::iterator i = options.begin(), e = options.end() ; i < e; i++ ) {
	bool handled = false;
	std::string & option = *i;
	size_t len = option.length();
	char * arg = new char[len + 1];
	option.copy(arg, len);
	arg[len] = '\0';

	if( arg[0] == '-' && arg[1] == '-' ) {
	    char * opt = &arg[2], * optarg = NULL;
	    char * equal_sign = index(arg, '=');

	    if( equal_sign != NULL ) {
		if( equal_sign == (arg + strlen(arg) - 1) ) {
		    if(!silent) log_severe( "unrecognized option '%s'", arg);
		    unhandled++;
		    continue;
		}

		*equal_sign = '\0';
		optarg = equal_sign + 1;
	    }

	    for( int j = handlers.size() -1; j >= 0; j-- ) {
		int success = (*(handlers[j])).handleOpt(opt, optarg, silent);

		if( success == 1 ) handled = true;
		else if( success == -1 ) {
		    /* the following 'if' is for aesthetics only */
		    if (equal_sign) *equal_sign = '=';
		    return false;
		}

	    }// for

	    if (equal_sign) *equal_sign = '=';

	    if( !handled ) {
		if(!silent) log_severe( "unrecognized option '%s'", arg);
		unhandled++;
	    }

	} else return false;

	delete[] arg;
    }// for

    if( unhandled > 0 ) return false;
    else return true;
}// OptionProcessor::processOptions

/**
 * Print out usage message and exit
 */
void OptionProcessor::usage() const {
	std::cout  << usageHeader << std::endl;

	int len = handlers.size();

	for( int i = 0; i < len; std::cout << (*(handlers[i++])).getUsage() );

}

OptionHandler::defs::standard_option::standard_option(
        std::string n /* = "" */,
        void * m /* = 0x0 */,
        enum OPTION_TYPE t /* = NOOPT */,
        std::string u /* = "" */,
        std::string h /* = "" */):
        name(n),
        mem(m),
        typ(t),
        units(u),
        help(h)
        { }

OptionHandler::defs::standard_option::standard_option(const standard_option & that) {
    *this = that;
}

OptionHandler::defs::standard_option &
OptionHandler::defs::standard_option::operator=(const OptionHandler::defs::standard_option & that) {
    this->name = that.name;
    this->mem = that.mem;
    this->typ = that.typ;
    this->units = that.units;
    this->help = that.help;
    return *this;
}

/** A routine to format a standard option into a usage string. */
std::string  OptionHandler::defs::standard_option::getUsage(const std::string & pre, const size_t & twidth) {
    /* first we check to make sure that the help message is formated for
     * the correct width. */
    std::string opthelp;
    std::string unitsStr = this->units.size()>0 ? "(" + this->units + ")" : "";
    char * opt_help = new char[this->help.size()];
    memcpy(opt_help, this->help.c_str(), this->help.size()+1);
    bool firstline = true;
    {
        char * startline = opt_help,
             * endhelp = opt_help + strlen(opt_help);

        while (startline < endhelp) {
            if ( (strlen(FORMAT_USAGE_CONT(/**/)) + strlen(startline)) > twidth ) {
                char * endline = startline + (twidth - strlen(FORMAT_USAGE_CONT(/**/)));

                /* find the nearest leading space. */
                while (endline > startline && !isspace(*endline)) endline--;

                if (endline > startline) {
                    /* split the line up if seems possible (easy). */
                    *endline = '\0';
                    if (firstline) {
                        opthelp.append(std::string(startline) + '\n');
                        firstline = false;
                    } else {
                        opthelp.append(FORMAT_USAGE_CONT(+std::string(startline)+));
                    }
                    startline = endline + 1;
                    continue;
                }/* if */
            }/* if */

            if (firstline) {
                opthelp.append(std::string(startline) + '\n');
                firstline = false;
            } else {
                opthelp.append(FORMAT_USAGE_CONT(+std::string(startline)+));
            }
            break;
        }/* while */
    }
    delete opt_help;

    /* And return the formated usage string. */
     return
        FORMAT_USAGE( + pre + (this->name
                                +(this->mem && defs::option_type_desc[this->typ] ?
                                 "=" + std::string(defs::option_type_desc[this->typ])
                                     :""
                                 )
                              )
                       +unitsStr+
                         //,/**/) + opthelp;
                         ,+opthelp+);
}/* getUsage */

