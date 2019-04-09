art_make(NO_PLUGINS
         EXCLUDE LArRawInputDriverUBooNE.cxx
         LIB_LIBRARIES lardataobj_RawData
                       larcorealg_Geometry
                       larcoreobj_SummaryData
                       ${ART_FRAMEWORK_CORE}
                       ${ART_FRAMEWORK_IO_SOURCES}
                       ${ART_FRAMEWORK_PRINCIPAL}
                       art_Persistency_Common
                       art_Persistency_Provenance
                       canvas
                       cetlib_except
                       ${PostgreSQL_LIBRARY}
                       ${Boost_system_LIBRARY}
                       ${Boost_serialization_LIBRARY}
                       ${Boost_date_time_LIBRARY}
                       ${ROOT_Core_LIBRARY}
                       ${ROOT_RIO_LIBRARY}
                       ${ROOT_Tree_LIBRARY}
        )

simple_plugin(LArRawInputSource "source"
  lardataobj_RawData
  lardata_RawData_utils
  ${ART_FRAMEWORK_IO_SOURCES}
)

install_headers()
install_fhicl()
install_source()
