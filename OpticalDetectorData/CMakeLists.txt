IF(ALT_CMAKE)
INCLUDE(altCMakeLists.cmake)
ELSE()
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

art_make(BASENAME_ONLY
         LIBRARY_NAME OpticalDetectorData
         LIB_LIBRARIES
        )

install_headers()
install_fhicl()
install_source()
ENDIF()
