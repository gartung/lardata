set(AnalysisBase_HEADERS
	Calorimetry.h
	CosmicTag.h
	FlashMatch.h
	ParticleID.h
	)

add_library(AnalysisBase SHARED
	${AnalysisBase_HEADERS}
	Calorimetry.cxx
	CosmicTag.cxx
	FlashMatch.cxx
	ParticleID.cxx
	)

target_link_libraries(AnalysisBase
	art::art_Framework_Core
	art::art_Framework_Principal
	art::art_Persistency_Provenance
	art::art_Utilities
	${ROOT_BASIC_LIB_LIST}
	FNALCore::FNALCore
	)

art_add_dictionary(DICTIONARY_LIBRARIES art::art_Framework_Core)

install(TARGETS
     AnalysisBase
     AnalysisBase_map
     AnalysisBase_dict
     EXPORT lardataLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     )

install(FILES ${AnalysisBase_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/AnalysisBase COMPONENT Development)

