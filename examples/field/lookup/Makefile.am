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

CPP_TIMED_RUN=$(shell if test "$(TIMED_RUN)" = "1"; then echo "-DTIMED_RUN=1"; else echo ""; fi;)

all-local:
	@echo "do 'make TIMED_RUN=1' to do a timed difference"
	@echo "between the lookup table and runtime computation."

CLEANFILES      = error.dat field.dat
bin_PROGRAMS    = createfieldfile testfield
noinst_HEADERS  = common.h
AM_CPPFLAGS     = -I$(top_builddir)/include $(CPP_TIMED_RUN)
LDADD           = -L$(top_builddir)/lib -lolson-tools
createfieldfile_SOURCES = createfieldfile.C
testfield_SOURCES = testfield.C
