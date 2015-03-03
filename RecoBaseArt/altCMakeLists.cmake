include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

set(RecoBaseArt_HEADERS
     FindAllP.h
     HitCreator.h
     HitUtils.h
     WireCreator.h
     )

add_library(RecoBaseArt SHARED
	${RecoBase_HEADERS}
     HitCreator.cxx
     HitUtils.cxx
     WireCreator.cxx
	)

target_link_libraries(RecoBaseArt
	art::art_Framework_Core
	art::art_Framework_Principal
	art::art_Persistency_Provenance
	art::art_Utilities
	${ROOT_BASIC_LIB_LIST}
	${ROOT_EG}
	FNALCore::FNALCore
	)


install(FILES ${RecoBaseArt_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/RecoBaseArt COMPONENT Development)

install(TARGETS
     RecoBaseArt
     EXPORT lardataLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     )
