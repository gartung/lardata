set(AnalysisAlg_HEADERS
	CalorimetryAlg.h
	Chi2PIDAlg.h
	)

add_library(AnalysisAlg SHARED
	${AnalysisAlg_HEADERS}
	CalorimetryAlg.cxx
	Chi2PIDAlg.cxx
	)

target_link_libraries(AnalysisAlg
	AnalysisBase
	RecoBase
	LArProperties_service
	DetectorProperties_service
	Utilities
	art::art_Framework_Core
	art::art_Framework_Principal
	art::art_Framework_Services_Registry
	art::art_Persistency_Common
	art::art_Persistency_Provenance
	art::art_Utilities
	${ROOT_BASIC_LIB_LIST}
	FNALCore::FNALCore
	)

install(TARGETS
     AnalysisAlg
     EXPORT lardataLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     )

install(FILES ${AnalysisAlg_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/AnalysisAlg COMPONENT Development)
	
