# $Id: Makefile,v 1.1.1.1 2005/01/08 04:27:26 olsonse Exp $
#
# Copyright 1997-2004 Spencer Olson
#
#

MAKECONF=$(top_builddir)/Makeconf
NO_OPTIM=$(srcdir)/Makefile.extra
include $(MAKECONF)
include $(NO_OPTIM)

bin_PROGRAMS    = testXML
#LDADD           = -L$(top_builddir)/lib -lolson-tools $(BOOST_LDFLAGS) -lboost_regex
LDADD           = $(BOOST_LDFLAGS) -lboost_regex
AM_LDFLAGS	= $(shell echo `xml2-config --libs`)
AM_CPPFLAGS     = -I$(top_builddir)/include $(shell xml2-config --cflags) $(BOOST_CPPFLAGS)
testXML_SOURCES = testXML.C

EXTRA_DIST      = file.xml

builddir=$(shell pwd)

all-local : $(builddir)/file.xml instructions

$(builddir)/file.xml :
	$(shell ln -sf $(srcdir)/file.xml $(builddir)/file.xml)

instructions :
	@echo
	@echo
	@echo After doing 'make testXML', do  './testXML "xpath-query'
	@echo
	@echo
	@echo

