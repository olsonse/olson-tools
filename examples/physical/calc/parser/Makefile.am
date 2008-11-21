# $Id: Makefile,v 1.1.1.1 2005/01/08 04:27:26 olsonse Exp $
#
# Copyright 1997-2008 Spencer Olson
#

MAKECONF=$(top_builddir)/Makeconf
NO_OPTIM=$(srcdir)/Makefile.extra
include $(MAKECONF)
include $(NO_OPTIM)

bin_PROGRAMS    = testParser
LDADD           = -L$(top_builddir)/lib -lolson-tools $(BOOST_LDFLAGS) -lboost_regex
AM_CPPFLAGS     = -I$(top_builddir)/include           $(BOOST_CPPFLAGS)
testParser_SOURCES = testParser.C
