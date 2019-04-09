# ======================================================================
#  lardata main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/lardata/ups/setup_for_development <-d|-p>
#  cmake [-DCMAKE_INSTALL_PREFIX=/install/path]
#        -DCMAKE_BUILD_TYPE=$CETPKG_TYPE
#        $CETPKG_SOURCE
#  make
#  make test
#  make install
#  make package (builds distribution tarfile)
# ======================================================================


# use cmake 3.11 or later
cmake_minimum_required (VERSION 3.11)

project(lardata VERSION 1.1.1 LANGUAGES CXX)

set(fcl_dir job)
set(gdml_dir gdml)
find_package( cetmodules )
include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS CAUTIOUS
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -pedantic
)

cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
find_package( PostgreSQL REQUIRED )
find_package( Range-v3 )
find_package( ROOT REQUIRED COMPONENTS Core GenVector FFTW Cling RIO Net Hist Geom XMLIO Gdml Graf Graf3d Gpad Tree EG Thread MathCore Physics Postscript Rint)
find_package(Boost REQUIRED COMPONENTS date_time filesystem graph program_options
serialization regex system unit_test_framework)
find_package( lardataobj REQUIRED)
find_package( lardataalg REQUIRED)
find_package( larcore  REQUIRED)
find_package( nutools  REQUIRED)

#  Find all the libraries needed by our dependent CMakeList.txt files
cet_find_library( NUSIMDATA_SIMULATIONBASE NAMES nusimdata_SimulationBase  )
find_library(SQLITE3 NAMES sqlite3 REQUIRED)

# macros for artdaq_dictionary and simple_plugin
include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)

# source
add_subdirectory(lardata)

# tests
add_subdirectory(test)
add_subdirectory(cmakeConfig)
# ups - table and config files
#add_subdirectory(ups)

# packaging utility
#include(UseCPack)
