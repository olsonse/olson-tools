# $Id: Makefile,v 1.1 2005/06/07 20:38:37 olsonse Exp $
#
# Copyright 1997-2004 Spencer Olson
#
# $Log: Makefile,v $
# Revision 1.1  2005/06/07 20:38:37  olsonse
# Added genetic algorithm test/example code.
#
# Revision 1.1.1.1  2005/01/08 04:27:26  olsonse
# Initial import
#
#

MAKECONF=$(top_builddir)/Makeconf
include $(MAKECONF)

bin_PROGRAMS    = testGA
AM_CPPFLAGS     = -I$(top_builddir)/include
LDADD           = -L$(top_builddir)/lib -lolson-tools
testGA_SOURCES  = testGA.C
