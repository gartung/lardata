# ======================================================================
#  lardata main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/lardata/ups/setup_for_development <-d|-p>
#  buildtool [-I /install/path] -bt[i] -jN
# ======================================================================


# use cmake 3.14 or later
cmake_minimum_required (VERSION 3.12...3.14 FATAL_ERROR)

project(lardata LANGUAGES CXX)

# cetbuildtools contains our cmake modules
SET ( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
IF (NOT CETBUILDTOOLS_VERSION)
    MESSAGE (FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules")
ENDIF()

set(CMAKE_MODULE_PATH $ENV{CANVAS_ROOT_IO_DIR}/Modules
		      $ENV{CETBUILDTOOLS_DIR}/Modules
		      ${CMAKE_MODULE_PATH})

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS CAUTIOUS
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -pedantic -I $ENV{CLANG_FQ_DIR}/include/c++/v1
)

cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
find_ups_product( nusimdata )
find_ups_product( larcoreobj )
find_ups_product( larcorealg )
find_ups_product( lardataobj )
find_ups_product( lardataalg )
find_ups_product( larcore  )
find_ups_product( art  )
find_ups_product(art_root_io)
find_ups_product( postgresql  )
find_ups_product( range )
find_ups_product( cetbuildtools )
find_ups_product( fftw )

#  Find all the libraries needed by our dependent CMakeList.txt files
# cet_find_library( NUSIMDATA_SIMULATIONBASE NAMES nusimdata_SimulationBase PATHS ENV NUSIMDATA_LIB NO_DEFAULT_PATH )
cet_find_library( PQ                  NAMES pq                  PATHS ENV POSTGRESQL_LIBRARIES NO_DEFAULT_PATH )

# macros for artdaq_dictionary and simple_plugin
include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)

# source
add_subdirectory(lardata)

# tests
add_subdirectory(test)

# ups - table and config files
add_subdirectory(ups)

# packaging utility
include(UseCPack)
