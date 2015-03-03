set(MCBase_HEADERS
	MCBaseException.h
	MCDataHolder.h
	MCHit.h
	MCHitCollection.h
	MCLimits.h
	MCShower.h
	MCWire.h
	MCWireCollection.h
        MCLimits.h
        MCStep.h
        MCTrack.h
	)

add_library(MCBase SHARED
	${MCBase_HEADERS}
	MCBaseException.cxx
	MCHit.cxx
	MCHitCollection.cxx
	MCWire.cxx
	MCWireCollection.cxx
        MCShower.cxx
        MCTrack.cxx
	)

target_link_libraries(MCBase
	art::art_Framework_Core
	art::art_Framework_Principal
	art::art_Persistency_Provenance
	art::art_Utilities
	${SIMULATIONBASE}
	${ROOT_BASIC_LIB_LIST}
	${ROOT_EG}
	FNALCore::FNALCore
	)

art_add_dictionary(DICTIONARY_LIBRARIES MCBase nutools::SimulationBase )

install(TARGETS
     MCBase
     MCBase_map
     MCBase_dict
     EXPORT lardataLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     )

install(FILES ${MCBase_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/MCBase COMPONENT Development)

