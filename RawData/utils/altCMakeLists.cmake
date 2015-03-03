set(RawData_utils_HEADERS
	LArRawInputDriver.h
	LArRawInputDriverJP250L.h
	LArRawInputDriverLongBo.h
	LArRawInputDriverShortBo.h
	)

add_library(RawData_utils SHARED
	${RawData_utils_HEADERS}
	LArRawInputDriver.cxx
	LArRawInputDriverJP250L.cxx
	LArRawInputDriverLongBo.cxx
	LArRawInputDriverShortBo.cxx
	)

target_link_libraries(RawData_utils
	RawData
	art::art_Framework_Core
	art::art_Framework_IO_Sources
	art::art_Framework_Principal
	art::art_Framework_Services_Registry
	art::art_Framework_Services_Optional
	art::art_Framework_Services_Optional_TFileService_service
	art::art_Persistency_Common
	art::art_Persistency_Provenance
	art::art_Utilities
	FNALCore::FNALCore
	${PQ}
	${BOOST_SERIALIZATION}
	${BOOST_DATE_TIME}
	${ROOT_CORE}
	${ROOT_CINT} 
	${ROOT_RIO}
	${ROOT_NET}
	${ROOT_HIST} 
	${ROOT_GEOM}
	${ROOT_XMLIO}
	${ROOT_GDML}
	${ROOT_GRAF}
	${ROOT_GRAF3D}
	${ROOT_GPAD}
	${ROOT_TREE}
	${ROOT_RINT}
	${ROOT_POSTSCRIPT}
	${ROOT_MATRIX}
	${ROOT_PHYSICS}
	${ROOT_MATHCORE}
	${ROOT_THREAD}
	${ROOT_EG}
	)

install(FILES ${RawData_utils_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/RawData/utils COMPONENT Development)

file(GLOB FHICL_FILES 
     [^.]*.fcl
)

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)


art_add_source(LArRawInputSource_source
	LArRawInputSource_source.cc
	)

install(TARGETS
     RawData_utils
     LArRawInputSource_source
     EXPORT lardataLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     )

