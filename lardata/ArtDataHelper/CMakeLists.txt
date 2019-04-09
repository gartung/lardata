art_make( LIB_LIBRARIES lardataobj_RawData
                        lardataobj_RecoBase
                        lardataobj_AnalysisBase
                        larcorealg_Geometry
                        larcore_Geometry_Geometry_service
                        ${ART_FRAMEWORK_CORE}
                        ${ART_FRAMEWORK_PRINCIPAL}
                        ${ART_FRAMEWORK_SERVICES_REGISTRY}
                        art_Persistency_Common
                        art_Persistency_Provenance
                        art_Utilities
                        canvas
                        cetlib_except
                        ${ROOT_Core_LIBRARY}
                        ${Boost_system_LIBRARY}
        )

add_subdirectory(Dumpers)

install_headers()
install_fhicl()
install_source()
