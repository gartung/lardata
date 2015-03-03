set(RecoObjects_HEADERS
	BezierCurveHelper.h
	BezierTrack.h
     Cluster3D.h
	InteractPlane.h
	Interactor.h
	KETrack.h
	KFitTrack.h
	KGTrack.h
	KHit.h
	KHitBase.h
	KHitContainer.h
	KHitContainerWireX.h
	KHitGroup.h
	KHitMulti.h
	KHitTrack.h
	KHitWireX.h
	KHitsTrack.h
	KTrack.h
	KalmanLinearAlgebra.h
	PropXYZPlane.h
	PropYZPlane.h
	Propagator.h
	SurfPlane.h
	SurfWireX.h
	SurfXYZPlane.h
	SurfYZPlane.h
	Surface.h
	)

add_library(RecoObjects SHARED
	${RecoObjects_HEADERS}
	BezierCurveHelper.cxx
	BezierTrack.cxx
     Cluster3D.cxx
	InteractPlane.cxx
	Interactor.cxx
	KETrack.cxx
	KFitTrack.cxx
	KGTrack.cxx
	KHitBase.cxx
	KHitContainer.cxx
	KHitContainerWireX.cxx
	KHitGroup.cxx
	KHitMulti.cxx
	KHitTrack.cxx
	KHitWireX.cxx
	KHitsTrack.cxx
	KTrack.cxx
	PropXYZPlane.cxx
	PropYZPlane.cxx
	Propagator.cxx
	SurfPlane.cxx
	SurfWireX.cxx
	SurfXYZPlane.cxx
	SurfYZPlane.cxx
	Surface.cxx
	)

target_link_libraries(RecoObjects
	AnalysisAlg
	AnalysisBase
	RecoBase
	LArProperties_service
	DetectorProperties_service
	larsoft::Geometry
	art::art_Framework_Core
	art::art_Framework_Principal
	art::art_Framework_Services_Registry	
	art::art_Persistency_Provenance
	art::art_Utilities
	${ROOT_BASIC_LIB_LIST}
	${ROOT_EG}
	FNALCore::FNALCore
	)


install(FILES ${RecoObjects_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/RecoObjects COMPONENT Development)


install(TARGETS
     RecoObjects
     EXPORT lardataLibraries
     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
     COMPONENT Runtime
     )
