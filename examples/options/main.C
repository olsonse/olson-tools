// -*- c++ -*-
// $Id: main.C,v 1.1 2005/01/08 04:27:26 olsonse Exp $
/*
 * Copyright 2004 Spencer Olson
 *
 * $Log: main.C,v $
 * Revision 1.1  2005/01/08 04:27:26  olsonse
 * Initial revision
 *
 */

#include "main.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <olson-tools/options.h>
#include <olson-tools/logger.h>

const char * PROGRAM = "A Good Program";

/******************* BEGIN BOOT OPTIONHANDLER *******************/
class BootOptionHandler : public OptionHandler {
  public:
    BootOptionHandler() {
        help_requested = false;

        /* add standard type options */
        stdopts.push_back(
            defs::standard_option("help",&help_requested,defs::BOOL_TRUE, "",
                "Print usage information.  This is really not all that usefull for "
                "such a demo program, but at least it allows me to demo or test "
                "the various features of this package.  "
                "For instance, right now, I'm demoing how additional help lines "
                "will be automatically fitted to the screen output, provided it "
                "is not too difficult to do."
            ));
        stdopts.push_back(
            defs::standard_option("float",&v0,defs::FLOAT, "seconds",
                "Set a float value."
            ));
        stdopts.push_back(
            defs::standard_option("double",&v1,defs::DOUBLE, "meter",
                "Set a double value."
            ));
        stdopts.push_back(
            defs::standard_option("uint",&v2,defs::UINT, "number particles",
                "Set a uint value."
            ));
    }
    virtual ~BootOptionHandler() {}

    /** Usage
     * return usage information for the options supported by this option
     * handler.
     * */
    virtual std::string getUsage() {
	return 
            OptionHandler::getStdOptsUsage("",stdopts) +
            std::string(
		FORMAT_USAGE("include=filename","Read additional options from filename.")
		FORMAT_USAGE_CONT("This file should contain a series of valid seperated by white space.")
		FORMAT_USAGE("loglevel=level","Specify log level from the following:")
		FORMAT_USAGE_CONT("severe, warning, info, config, fine, finer, finest.")
		FORMAT_USAGE("logfile=filename","Append log information to filename.")
		FORMAT_USAGE("print-vals","Print out the standard type options' values")
	);
    }
    
    float v0;
    double v1;
    unsigned int v2;

    /** Handle Opt
     * Handle the options as parsed by the optionProcessor.
     */
    virtual int handleOpt(const char * opt, const char * optarg, const bool silent = false) {
	if (OptionHandler::handleStdOpts(stdopts,opt, optarg, silent) == 1) {
	    return 1;
	} else if (strcasecmp(opt, "print-vals") == 0) {
	    if(!silent) {
		log_info("Parameters:");
		printStdOpts("", stdopts);
	    }
	    return 1;
	} else if (strcasecmp(opt, "include") == 0) {
	    if (optarg == NULL) {
		log_severe("include option expects a filename");
		return -1;
	    }
	    std::ifstream cfile(optarg);

	    if (!cfile) {
		log_severe("include option expects a GOOD filename");
		return -1;
	    }

	    std::string option;
	    while (cfile>>option) {
		options.push_back(option);
	    }

	    return 1;
	} else if (strcasecmp(opt, "loglevel") == 0) {
	    if (optarg == NULL) {
		log_severe("loglevel option expects a value");
		return -1;
	    }

	    if (strcasecmp(optarg, "severe") == 0) LogLevel = LLOG_SEVERE;
	    else if (strcasecmp(optarg, "warning") == 0) LogLevel = LLOG_WARNING;
	    else if (strcasecmp(optarg, "info") == 0) LogLevel = LLOG_INFO;
	    else if (strcasecmp(optarg, "config") == 0) LogLevel = LLOG_CONFIG;
	    else if (strcasecmp(optarg, "fine") == 0) LogLevel = LLOG_FINE;
	    else if (strcasecmp(optarg, "finer") == 0) LogLevel = LLOG_FINER;
	    else if (strcasecmp(optarg, "finest") == 0) LogLevel = LLOG_FINEST;
	    else {
		log_severe("cannot set loglevel to '%s'", optarg);
		return -1;
	    }

	    return 1;
	} else if (strcasecmp(opt, "logfile") == 0) {
	    FILE * lfp = NULL;
	    if (optarg == NULL) {
		log_severe("logfile option expects a filename");
		return -1;
	    }

	    lfp = fopen(optarg, "a");
	    
	    if (!lfp) {
		log_severe("cannot open log file '%s'", optarg);
		return -1;
	    }

	    LogFileP = lfp;
	    return 1;
	} else return 0;
    }

    bool helpRequested() const { return help_requested; }

    defs::stdopt_vector stdopts;
    std::vector<std::string> options;
  private:
    bool help_requested;
};

/******************* END BOOT OPTIONHANDLER *******************/

BootOptionHandler bootOptionHandler;


/******************** BEGIN MAIN ******************************/
int main(int argc, char *argv[]) {

    setLogProgramName((char*)PROGRAM);
    OptionProcessor optProcessor;
    optProcessor.setUsageHeader(
	"Test Program: Release 0.1\n"
	"Copyright (C) 2002-2004 Spencer Olson\n\n"
	"This program is to demonstrate the use of the olson-tools package"
    );

    optProcessor.addHandler(bootOptionHandler);

    if ((!optProcessor.processOptions(argc-1,argv+1) ||
	 bootOptionHandler.helpRequested()
	) || 
	(bootOptionHandler.options.size() > 0 &&
	 (!optProcessor.processOptions(bootOptionHandler.options) ||
	  bootOptionHandler.helpRequested()
	 )
	)
       ) {
	optProcessor.usage();
    }

    return 0;
}// main
/********************  END MAIN *******************************/

