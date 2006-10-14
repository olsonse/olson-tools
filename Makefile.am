# And you'll need to mention it in your Makefile.am file, so that it gets
# installed and distributed. For instance:
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = olson-tools.pc

makeconfdir = @MAKECONF_PATH@
makeconf_DATA = Makeconf

SUBDIRS = src

tests :
	cd examples; make

doc :
	cd src; doxygen tools.doxy
