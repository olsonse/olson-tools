# Copyright 2004 Spencer Olson
include ../Makeconf

OBJ=options.o logger.o rk.o power.o \
	random/random.o \
	random/randomf.o \
        3j.o bfield.o

LIBNAME=libolson-tools$(EXT)

LIBS=-lstdc++ $(FLIBS) -lm

all:  ${LIBNAME}.a \
      #${LIBNAME}.so 

${LIBNAME}.so: ${OBJ}
	rm -f $(LIBNAME).so;
	$(CXX) -shared ${CFLAGS} ${LDFLAGS} -o $(LIBNAME).so ${OBJ} ${LIBS}

${LIBNAME}.a: ${OBJ}
	rm -f $(LIBNAME).a;
	${AR} -rs ${LIBNAME}.a ${OBJ}

clean:
	rm -f ${OBJ} ${LIBNAME}.so ${LIBNAME}.a
