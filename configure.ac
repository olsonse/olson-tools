#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# This is $Revision$ of olson-tools, not revision 588.
m4_define([GET_SVNREV], regexp([$Revision$],[\($Revision: *\)\([0-9]*\)],r\2) )

AC_PREREQ(2.59)
AC_INIT(olson-tools, GET_SVNREV, olsonse@umich.edu)
AM_INIT_AUTOMAKE(olson-tools,GET_SVNREV)
AC_CONFIG_SRCDIR([src/config.h.in])
AC_CONFIG_HEADER([src/config.h])
AX_PREFIX_CONFIG_H([src/$PACKAGE-config.h])
AC_PREFIX_DEFAULT(`pwd`)

# Docygen features
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN(olson-tools, tools.doxy, doc/api)

# Now let's make sure that we figure out what kind of platform is being
# compiled for:  
AC_CANONICAL_HOST


PRESET_CFLAGS="$CFLAGS"
PRESET_CXXFLAGS="$CXXFLAGS"
PRESET_FFLAGS="$FFLAGS"
PRESET_FCFLAGS="$FCFLAGS"


# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_F77(gfortran g77)
AC_PROG_RANLIB
AC_PROG_MAKE_SET
AC_PROG_INSTALL

# Check for pkg-config
AC_PATH_PROG(PKGCONFIG, pkg-config, false)

# find out the exe extension for this platform.
AC_EXEEXT
AC_OBJEXT

# Checks compilers
# AC_LANG(C)
AC_LANG(C++)
# AC_LANG_F77
# AC_LANG_FC
# AC_FC_FREEFORM


# Checks for libraries.
# FIXME: Replace `main' with a function in `-lc':
AC_CHECK_LIB([c], [strcpy])
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [cos])


# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fenv.h limits.h stdint.h stdlib.h string.h strings.h sys/time.h syslog.h unistd.h pthread.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_TYPE_SIGNAL
AC_FUNC_STRFTIME
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([floor gettimeofday memset pow rint sqrt strcasecmp strerror])



#                      BEGIN FAST_POW CHECK
# Check for inclusion of the fast_pow function
# If fast pow is not going to be used, a #define will be used to shortcut it
# to pow.
probe_for_fast_pow(){
    local saved_CPPFLAGS="$CPPFLAGS"
    CPPFLAGS="$CPPFLAGS -I$srcdir/src -I`pwd`/src"

    local fast_pow_enabled=0
    printf "Testing to see if compiler can use fast_pow... "
#   Make sure that olson-tools-config.h at least exists so that power.h can include it.
    AS_MKDIR_P(src)
    touch src/olson-tools-config.h
    AC_COMPILE_IFELSE(
        [AC_LANG_PROGRAM(
[[
#define OLSON_TOOLS_USE_SPENCERS_FAST_POW 1
#include "$srcdir/src/power.c"
]],
            [[fast_pow (0.5,0.5);]])],
        [
            AC_DEFINE(USE_SPENCERS_FAST_POW, 1, [Enable Spencer's fast_pow function.])
            fast_pow_enabled="1"
        ],
        [
            fast_pow_enabled="0"
        ]
    )

    AM_CONDITIONAL([FAST_POW_ENABLED], [test "$fast_pow_enabled" = "1"])
    if test "$fast_pow_enabled" = "1" ; then
        echo "yes"
        echo "fast_pow enabled"
    else
        echo "no"
        echo "fast_pow not enabled"
    fi

    CPPFLAGS="$saved_CPPFLAGS"
}

AC_ARG_ENABLE(fast_pow,
    AC_HELP_STRING(
        [--disable-fast_pow],
        [Disable use of Spencer's fast_pow function.  
         See examples/fast_pow to test it's accuracy
         and speed.]
    ),
[ if test "$enableval" = "yes" ; then
    probe_for_fast_pow
  else
    echo "fast_pow disabled"
  fi
],[
    probe_for_fast_pow
])
#                        END FAST_POW CHECK



#                      BEGIN CAN_ALIAS CHECK
# Check to see whether aliasing of symbols works.  It is a pity that more
# platforms/compilers do not support this nice feature of ELF.
probe_alias_attribute(){
    local alias_supported=0
    printf "Testing to see if aliasing of symbols is supported... "
    AC_RUN_IFELSE(
        [AC_LANG_PROGRAM(
[[
extern "C" {
    int fun() {return 0;}
}
int funalias () __attribute__ (( alias ("fun") )) ;
]],
[[
funalias ();
]])     ],
        [
            AC_DEFINE(CAN_ALIAS, 1,
                [ELF aliasing of symbols is supported using GCC type __attribute__.])
            alias_supported="1"
            echo "yes"
        ],
        [
            alias_supported="0"
            echo "no"
        ]
    )
    return $alias_supported
}
probe_alias_attribute
# TODO:  Check to see whether you can define an alias in one object code but
# define its target in a separately compiled object code.
#                        END CAN_ALIAS CHECK



# ############# BEGIN OPENMP CHECKS ###########

AC_ARG_WITH(
    [openmp-cppflag],
    AC_HELP_STRING(
        [--with-openmp-cppflag=FLAG],
        [Specify the compiler flag to enable OpenMP.]
    ),
    [
        # --with-openmp-cppflag given
        OPENMP_CPPFLAG="$withval -D_REENTRANT"
        CPPFLAGS="$CPPFLAGS $OPENMP_CPPFLAG"
        AC_SUBST(OPENMP_CPPFLAG,"$OPENMP_CPPFLAG")
    ],
    [
        # TODO:  In the future, perhaps we can probe for this flag.
        echo "No OpenMP Flag given"
    ]
)

# #############   END OPENMP CHECKS ###########



# ############# BEGIN PTHREAD CHECK ###########
AC_ARG_ENABLE(pthread,
    AC_HELP_STRING(
        [--enable-pthread],
        [Enable use of pthreads.  Mostly this puts mutexes in place for
        memory.h.]
    ),
[ if test "$enableval" = "yes" ; then
    printf "Checking to see whether pthreads was found... "
    AC_COMPILE_IFELSE(
        [AC_LANG_PROGRAM(
[[
#ifdef HAVE_PTHREAD_H
    /* pthreads where already found */
#  include <pthread.h>
#else
   We cannot find pthread support
#endif
]],
            [[pthread_exit (NULL);]])],
        [
            AC_DEFINE(USE_PTHREAD, 1, [Enable use of pthread library.])
            AC_SUBST([PTHREAD_LIB],"-lpthread")
            echo "yes"
        ],
        [
            echo "no"
        ]
    )
  else
    echo "Use of pthread library disabled."
  fi
])
# #############   END PTHREAD CHECK ###########



# Now let's set the CFLAGS and such variables:
# Set CFLAGS
if test -z "$PRESET_CFLAGS"; then
    CFLAGS="\$(OT_CFLAGS0) \$(OPTIM_CC)"
else
    CFLAGS="\$(OT_CFLAGS0) $PRESET_CFLAGS"
fi

# Set CXXFLAGS
if test -z "$PRESET_CXXFLAGS"; then
    CXXFLAGS="\$(OT_CXXFLAGS0) \$(OPTIM_CXX)"
else
    CXXFLAGS="\$(OT_CXXFLAGS0) $PRESET_CXXFLAGS"
fi

# Set FFLAGS
if test -z "$PRESET_FFLAGS"; then
    FFLAGS="\$(OT_FFLAGS0) \$(OPTIM_F77)"
else
    FFLAGS="\$(OT_FFLAGS0) $PRESET_FFLAGS"
fi

# Set FCFLAGS
if test -z "$PRESET_FCFLAGS"; then
    FCFLAGS="\$(OT_FCFLAGS0) \$(OPTIM_FC)"
else
    FCFLAGS="\$(OT_FCFLAGS0) $PRESET_FCFLAGS"
fi

# Set CPPFLAGS
CPPFLAGS="$CPPFLAGS \$(OT_CPPFLAGS)"


# set the path of the makeconf file
AC_SUBST([MAKECONF_PATH],[$includedir/olson-tools])

AC_CONFIG_FILES([Makeconf
                 Makefile
                 src/Makefile
                 examples/Makefile
                 examples/bin/Makefile
                 examples/bin/generic/Makefile
                 examples/bin/keyed/Makefile
                 examples/bin/extended_2d/Makefile
                 examples/bin/extended_2d/generic/Makefile
                 examples/bin/extended_2d/keyed/Makefile
                 examples/fast_pow/Makefile
                 examples/fast_pow/c/Makefile
                 examples/fast_pow/fortran/Makefile
                 examples/field/Makefile
                 examples/field/lookup/Makefile
                 examples/field/addfield/Makefile
                 examples/fit/Makefile
                 examples/fit/ga/Makefile
                 examples/fit/simplex/Makefile
                 examples/invertdistro/Makefile
                 examples/invertdistro/1D/Makefile
                 examples/invertdistro/2D/Makefile
                 examples/memory/Makefile
                 examples/options/Makefile
                 examples/random/Makefile
                 examples/readdata/Makefile
                 examples/rk/Makefile
                 examples/timer/Makefile
                 examples/timing/Makefile
                 examples/trapfe/Makefile
                 olson-tools.pc])
AC_OUTPUT

