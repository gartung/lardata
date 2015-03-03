include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

set(RecoBase_HEADERS
	Cluster.h
	EndPoint2D.h
	Event.h
	Hit.h
	OpFlash.h
	OpHit.h
   PCAxis.h
	PFParticle.h
	Seed.h
	Shower.h
	SpacePoint.h
	Track.h
	Vertex.h
	Wire.h
	)

add_library(RecoBase SHARED
	${RecoBase_HEADERS}
	Cluster.cxx
	EndPoint2D.cxx
	Event.cxx
	Hit.cxx
	OpFlash.cxx
	OpHit.cxx
   PCAxis.cxx
	PFParticle.cxx
	Seed.cxx
	Shower.cxx
	SpacePoint.cxx
	Track.cxx
	Vertex.cxx
	Wire.cxx
	)

target_link_libraries(RecoBase
	art::art_Framework_Core
	art::art_Framework_Principal
	art::art_Persistency_Provenance
	art::art_Utilities
	${ROOT_BASIC_LIB_LIST}
	${ROOT_EG}
	FNALCore::FNALCore
	)


install(FILES ${RecoBase_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/RecoBase COMPONENT Development)

art_add_dictionary(DICTIONARY_LIBRARIES art::art_Framework_Core)

set_target_properties(RecoBase_dict RecoBase_map
                      PROPERTIES COMPILE_FLAGS -fno-var-tracking)

install(TARGETS
     RecoBase
     RecoBase_map
     RecoBase_dict
     EXPORT lardataLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     )
