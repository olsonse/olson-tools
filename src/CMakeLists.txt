# define the library, its source files, and its dependencies 
hpmi_library( olson-tools
    GLOB olson-tools/*.cpp
         olson-tools/*.c
         olson-tools/nsort/*.cpp
         olson-tools/nsort/map/*.cpp
         olson-tools/nsort/tweak/*.cpp
         olson-tools/random/*.cpp
         olson-tools/xml/*.cpp
         olson-tools/fit/*.cpp
    EXCLUDE
         olson-tools/fit/simfit.cpp
    INCLUDE_DIRS .
    COMPILE_DEFINITIONS
        USE_SPENCERS_FAST_POW=1
    STATIC
)

client_requirements( olson-tools
    INCLUDE_DIRS .
    COMPILE_DEFINITIONS
        USE_SPENCERS_FAST_POW=1
)

# add sub-directory to get the unit tests added recursively
add_subdirectory( olson-tools )

