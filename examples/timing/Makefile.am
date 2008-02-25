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
$(shell ln -sf $(srcdir)/plot.plt plot.plt)

CLEANFILES      = gravity-force.dat scalar-field.dat vector-field.dat plot.plt
bin_PROGRAMS    = testtiming
LDADD           = -L$(top_builddir)/lib -lolson-tools
AM_CPPFLAGS     = -I$(top_builddir)/include
testtiming_SOURCES = testtiming.C
