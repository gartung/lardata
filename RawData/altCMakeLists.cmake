
set(RawData_HEADERS
	AuxDetDigit.h
	BeamInfo.h
	DAQHeader.h
	ExternalTrigger.h
	OpDetPulse.h
	RawDigit.h
	TriggerData.h
	raw.h
	)

add_library(RawData SHARED
	${RawData_HEADERS}
	AuxDetDigit.cxx
	BeamInfo.cxx
	DAQHeader.cxx
	ExternalTrigger.cxx
	OpDetPulse.cxx
	RawDigit.cxx
	TriggerData.cxx
	raw.cxx
	)

target_link_libraries(RawData
	art::art_Framework_Core
	art::art_Framework_Principal
	art::art_Persistency_Provenance
	art::art_Utilities
	${ROOT_BASIC_LIB_LIST}
	${ROOT_EG}
	FNALCore::FNALCore
	)

install(FILES ${RawData_HEADERS} DESTINATION
${CMAKE_INSTALL_INCLUDEDIR}/RawData COMPONENT Development)

art_add_dictionary(DICTIONARY_LIBRARIES art::art_Framework_Core)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

install(TARGETS
     RawData
     RawData_map
     RawData_dict
     EXPORT lardataLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     )


add_subdirectory(utils)


