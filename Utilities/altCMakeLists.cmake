set (Utilities_HEADERS
     AssociationUtil.h
     BulkAllocator.h
     ClockConstants.h
     CountersMap.h
     DatabaseUtil.h
     Dereference.h
     DetectorProperties.h
     ElecClock.h
     FileCatalogMetadataExtras.h
     GeometryUtilities.h
     LArFFT.h
     LArProperties.h
     NestedIterator.h
     PxHitConverter.h
     PxUtils.h
     Range.h
     SeedCreator.h
     SignalShaping.h
     SimpleTimeService.h
     StatCollector.h
     SumSecondFunction.h
     TimeService.h
     TimeServiceException.h
     UniqueRangeSet.h
     UtilException.h
     VectorMap.h
     sparse_vector.h
     )

add_library(Utilities SHARED
     ${Utilities_HEADERS}
	ElecClock.cxx
	GeometryUtilities.cxx
     PxHitConverter.cxx
     Range.cxx     
	SignalShaping.cxx
	SimpleTimeService.cxx
     UniqueRangeSet.cxx
     UtilException.cxx
	)

target_link_libraries(Utilities
     RecoBase
     larsoft::Geometry
     art::art_Framework_Core
     art::art_Framework_IO_Sources
     art::art_Framework_Principal
     art::art_Persistency_Provenance
     art::art_Utilities
     FNALCore::FNALCore
     ${ROOT_BASIC_LIB_LIST}
     )


art_add_service(TimeService_service
	TimeService_service.cc
	)

art_add_service(DatabaseUtil_service
	DatabaseUtil_service.cc
	)

art_add_service(DetectorProperties_service
	DetectorProperties_service.cc
	)

art_add_service(FileCatalogMetadataExtras_service
	FileCatalogMetadataExtras_service.cc
	)


art_add_service(LArFFT_service
	LArFFT_service.cc
)


art_add_service(LArProperties_service
	LArProperties_service.cc
)


art_add_service(MemoryPeakReporter_service
	MemoryPeakReporter_service.cc
	)


art_add_module(ComputePi_module
	ComputePi_module.cc
	)

install(FILES ${Utilities_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Utilities COMPONENT Development)

file(GLOB FHICL_FILES 
     [^.]*.fcl
)

install(FILES ${FHICL_FILES} DESTINATION job COMPONENT Runtime)




install(TARGETS
     Utilities
     TimeService_service
     DatabaseUtil_service
     DetectorProperties_service
     FileCatalogMetadataExtras_service
     LArFFT_service
     LArProperties_service
     MemoryPeakReporter_service
     ComputePi_module
     EXPORT lardataLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     )


add_subdirectory(test)
