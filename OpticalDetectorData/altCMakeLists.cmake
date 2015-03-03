include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

set(OpticalDetectorData_HEADERS
	ChannelData.h
	ChannelDataGroup.h
	FIFOChannel.h
	OpticalRawDigit.h
	OpticalTypes.h
	PMTTrigger.h
	)

add_library(OpticalDetectorData SHARED
	${OpticalDetectorData_HEADERS}
	OpticalDetectorData.cxx
	)

target_link_libraries(OpticalDetectorData
	art::art_Framework_Core
	art::art_Framework_Principal
	art::art_Persistency_Provenance
	art::art_Utilities
	${ROOT_BASIC_LIB_LIST}
	FNALCore::FNALCore
	)

art_add_dictionary(DICTIONARY_LIBRARIES art::art_Framework_Core)

install(TARGETS
     OpticalDetectorData
     OpticalDetectorData_map
     OpticalDetectorData_dict
     EXPORT lardataLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     )

install(FILES ${OpticalDetectorData_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/OpticalDetectorData COMPONENT Development)

