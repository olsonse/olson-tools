# $Id: Makefile,v 1.1.1.1 2005/01/08 04:27:26 olsonse Exp $
#
# Copyright 1997-2008 Spencer Olson
#

MAKECONF=$(top_builddir)/Makeconf
include $(MAKECONF)

bin_PROGRAMS    = testDataOnly
LDADD           = -L$(top_builddir)/lib -lolson-tools
AM_CPPFLAGS     = -I$(top_builddir)/include
testDataOnly_SOURCES = testDataOnly.C
