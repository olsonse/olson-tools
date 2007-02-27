// -*- c++ -*-
// $Id: main.C,v 1.1.1.1 2005/01/08 04:27:26 olsonse Exp $
/*
 * Copyright 2004 Spencer Olson
 *
 * $Log: main.C,v $
 * Revision 1.1.1.1  2005/01/08 04:27:26  olsonse
 * Initial import
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
    BootOptionHandler() : v0(0.0), v1(0.0), v2(0), logfile(""), loglevel("") {
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
        stdopts.push_back(
            defs::standard_option("logfile",&logfile,defs::STRING, "filename",
                "Append log information to filename."
            ));
        stdopts.push_back(
            defs::standard_option("loglevel",&loglevel,defs::STRING, "",
		"Specify log level from the following:  "
		"severe, warning, info, config, fine, finer, finest."
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
		FORMAT_USAGE("print-vals","Print out the standard type options' values")
	);
    }
    
    float v0;
    double v1;
    unsigned int v2;
    std::string logfile;
    std::string loglevel;

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
	} else return 0;
    }

    virtual bool postProcess(const bool silent = false) {
        bool error = false;
	if (logfile.length() > 0) {
	    FILE * lfp = NULL;
	    lfp = fopen(logfile.c_str(), "a");
	    
	    if (!lfp) {
		log_severe("cannot open log file '%s'", logfile.c_str());
                error = true;
	    }

            if (LogFileP != NULL) fclose(LogFileP);
	    LogFileP = lfp;
        }

	if (loglevel.length() > 0) {
            std::transform(loglevel.begin(), loglevel.end(),
                           loglevel.begin(), std::tolower);
	    if (loglevel == "severe")           LogLevel = LLOG_SEVERE;
	    else if (loglevel == "warning")     LogLevel = LLOG_WARNING;
	    else if (loglevel == "info")        LogLevel = LLOG_INFO;
	    else if (loglevel == "config")      LogLevel = LLOG_CONFIG;
	    else if (loglevel == "fine")        LogLevel = LLOG_FINE;
	    else if (loglevel == "finer")       LogLevel = LLOG_FINER;
	    else if (loglevel == "finest")      LogLevel = LLOG_FINEST;
	    else {
		log_severe("cannot set loglevel to '%s'", loglevel.c_str());
		error = true;
	    }
        }

        return !error;
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
    bool opt_error = !optProcessor.processOptions(argc-1,argv+1);
    if (bootOptionHandler.options.size() > 0)
        opt_error |= !optProcessor.processOptions(bootOptionHandler.options);

    optProcessor.postProcess();

    if (opt_error || bootOptionHandler.helpRequested())
	optProcessor.usage();

    return 0;
}// main
/********************  END MAIN *******************************/

