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

all-local:
	@echo "Simple example of how to use the templated AddField class."

bin_PROGRAMS    = testAddField
AM_CPPFLAGS     = -I$(top_builddir)/include
LDADD           = -L$(top_builddir)/lib -lolson-tools
testAddField_SOURCES = testAddField.C
