# Copyright 2004 Spencer Olson
include ../Makeconf
CFLAGS:=$(CFLAGS) -DBUILD_TOOLS

OBJ=options.o logger.o rk.o power.o \
	random/random.o \
	random/randomf.o \
        3j.o bfield.o \
        Distribution.o

LIBNAME=libolson-tools$(EXT)

LIBS=-lstdc++ $(FLIBS) -lm

all:  ${LIBNAME}.a \
      $(LIBTRAPFE) \
      #${LIBNAME}.so \

${LIBNAME}.so: ${OBJ}
	rm -f $(LIBNAME).so;
	$(CXX) -shared ${CFLAGS} ${LDFLAGS} -o $(LIBNAME).so ${OBJ} ${LIBS}

${LIBNAME}.a: ${OBJ}
	rm -f $(LIBNAME).a;
	${AR} -rs ${LIBNAME}.a ${OBJ}

TRAPFE_FLAGS_SunOS_sparc=-D__EXTENSIONS__ -I/opt/SUNWspro/WS6U2/include/cc

TRAPFE_FLAGS=$(TRAPFE_FLAGS_$(OS)_$(PROCESSOR))

$(LIBTRAPFE) : trapfe.c
	$(CC) $(CFLAGS) $(TRAPFE_FLAGS) -o $@  -c $<

clean:
	rm -f ${OBJ} ${LIBNAME}.so ${LIBNAME}.a $(LIBTRAPFE)
