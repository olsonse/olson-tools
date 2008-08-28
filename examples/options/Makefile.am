# $Id: Makefile,v 1.1.1.1 2005/01/08 04:27:26 olsonse Exp $
#
# Copyright 1997-2004 Spencer Olson
#
# $Log: Makefile,v $
# Revision 1.1.1.1  2005/01/08 04:27:26  olsonse
# Initial import
#
#

MAKECONF=$(top_builddir)/Makeconf
include $(MAKECONF)

CLEANFILES      = logfile
bin_PROGRAMS    = main
LDADD           = -L$(top_builddir)/lib -lolson-tools
AM_CPPFLAGS     = -I$(top_builddir)/include
main_SOURCES    = main.C
noinst_HEADERS  = main.h

EXTRA_DIST      = config

builddir=$(shell pwd)

all-local : $(builddir)/config instructions

$(builddir)/config :
	$(shell ln -sf $(srcdir)/config $(builddir)/config)

instructions :
	@echo
	@echo
	@echo Execute \'main --help\' for example usage.
	@echo The included \'config\' file is intended to be used with the
	@echo --include=[file]  option.
	@echo
	@echo
	@echo

