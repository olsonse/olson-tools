# Copyright 2004 Spencer Olson

MAKECONF=$(top_builddir)/Makeconf
include $(MAKECONF)

lib_LIBRARIES = libolson-tools.a

libolson_tools_a_SOURCES = \
        options.C logger.c \
        rk.F power.c \
        random/random.C \
        random/randomf.f \
        3j.c \
        Distribution.C \
        fit/debug.C \
        fit/Gene.C \
        fit/Generation.C \
        fit/Histogram.C \
        fit/GeneticAlg.C \
        fit/GeneticAlgArgs.C \
        fit/Individual.C \
        fit/io.C \
        util.C \
        m_eps.C

# Make it easy to install the headers
libolson_tools_a_includedir = $(includedir)/olson-tools
nobase_libolson_tools_a_include_HEADERS = \
        ./fit/io.h \
        ./fit/debug.h \
        ./fit/GeneticAlgArgs.h \
        ./fit/Gene.h \
        ./fit/fit.h \
        ./fit/merit_def.h \
        ./fit/simfit.h \
        ./fit/Histogram.h \
        ./fit/Individual.h \
        ./fit/Generation.h \
        ./fit/GeneticAlg.h \
        ./readData.h \
        ./3j.h \
        ./rk.h \
        ./KeyedBin.h \
        ./Vector.h \
        ./createFieldFile.h \
        ./msg.h \
        ./GenericBinExtender.h \
        ./strutil.h \
        ./physical/physical.h \
        ./physical/quantity.h \
	./physical/calc/BaseCalc.h \
	./physical/calc/infix.h \
	./physical/calc/math.h \
	./physical/calc/symbol.h \
	./physical/calc/units.h \
        ./InvCylindricalDist.h \
        ./bfield.h \
        ./Fields.h \
        ./Forces.h \
        ./info.h \
        ./listutil.h \
        ./indices.h \
        ./random/MersenneTwister.h \
        ./random/random.h \
        ./options.h \
        ./util.h \
        ./logger.h \
        ./GenericBin.h \
        ./BTrapDistro.h \
        ./fmacros.h \
        ./Distribution.h \
        ./field-lookup.h \
        ./force-lookup.h \
        ./power.h \
        ./RKIntegrator.h \
        ./memory.h \
        ./ompexcept.h \
        ./m_eps.h \
        ./nothing.h \
        ./Timer.h \
        ./Timing.h \
        ./version.h \
        ./ScaleField.h \
        ./ScaleForce.h \
        ./mpi_init.h \
	./PThreadCache.h \
        ./$(PACKAGE)-config.h



# Now we have to do some trickery to make sure that libtrapfe.a is installed
# correctly without ARing or RANLIBing it (doing so disables the constructor
# code inside it).
noinst_LIBRARIES = libtrapfe.a
libtrapfe_a_SOURCES = trapfe.C

libtrapfe_adir = $(libdir)
dist_libtrapfe_a_DATA = libtrapfe.a

libtrapfe_a_CPPFLAGS_SunOS = -D__EXTENSIONS__ -I/opt/SUNWspro/WS6U2/include/cc
libtrapfe_a_CPPFLAGS = $(libtrapfe_a_CPPFLAGS_$(OS))

libtrapfe.a : $(libtrapfe_a_OBJECTS)
	-rm -f libtrapfe.a
	cp -p $(libtrapfe_a_OBJECTS) libtrapfe.a

