set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-misleading-indentation")
art_make(LIB_LIBRARIES lardataobj_AnalysisBase
                       lardataobj_RecoBase
                       larcore_Geometry_Geometry_service
                       larcorealg_Geometry
                       ${ART_FRAMEWORK_CORE}
                       ${ART_FRAMEWORK_SERVICES_REGISTRY}
                       canvas
                       ${FHICLCPP}
                       cetlib_except
                       ${ROOT_Core_LIBRARY}
                       ${ROOT_Physics_LIBRARY}
                       ${Boost_system_LIBRARY}
        )


install_headers()
install_fhicl()
install_source()
